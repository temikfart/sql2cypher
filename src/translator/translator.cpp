#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;
namespace fs = std::filesystem;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

Translator::Translator(NodePtr<INode> ast, const fs::path& out_path)
    : ast_(std::move(ast)), schema_path_(out_path) {}

void Translator::Translate(bool translate_schema) {
  LOGI << "Translation is started";
  if (ast_ == nullptr || !HasChildren(ast_)) {
    LOGI << "Translation is ended. Nothing to translate";
    return;
  }

  if (!translate_schema) {
    LOGW << "Currently only translation of schema migration queries is supported."
         << " Use --translate-schema flag to translate schema";
    return;
  } else {
    schema_path_.replace_extension(".json");
    out_.open(schema_path_);
  }

  ValidateIsCorrectStmtType(ast_, StmtType::kProgram);
  for (unsigned i = 0; HasChildren(ast_, i + 1); ++i) {
    auto query = ast_->Child(i);
    ValidateIsCorrectStmtType(query, StmtType::kQuery);
    TranslateQuery(query);
  }
  LOGI << "Translation is ended";

  LOGI << "Serializing Neo4j schema to json...";
  out_ << schema_.ToJsonString() << std::endl;
  LOGI << "Serialized Neo4j schema saved at " << schema_path_.string();
}

void Translator::WriteCypherQuery(const std::string& query) {
  out_ << query << ";\n";
}
void Translator::FinishCypherQueriesGroup() {
  out_ << std::endl;
}

void Translator::TranslateQuery(const NodePtr<INode>& query) {
  if (!HasChildren(query)) {
    LOGD << "Empty query";
    return;
  }

  auto statement_type = query->Child(0);
  ValidateHasChildren(statement_type);
  auto statement = statement_type->Child(0);
  ValidateHasChildren(statement);
  switch (statement_type->stmt_type) {
    case StmtType::kDdlStmt:
      TranslateDDLStatement(statement);
      break;
    case StmtType::kDmlStmt:
      TranslateDMLStatement(statement);
      break;
    default:
      throw translation_error(format("Unknown query statement type: \'{}\'",
                                     statement_type->stmt_type.ToString()));
  }

  FinishCypherQueriesGroup();
}

} // scc::translator
