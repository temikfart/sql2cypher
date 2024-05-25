#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

Translator::Translator(NodePtr<INode> ast, const std::filesystem::path& out_path)
    : ast_(std::move(ast)), out_(out_path) {}

void Translator::Translate() {
  LOGI << "Translation is started";
  if (ast_ == nullptr || !HasChildren(ast_)) {
    LOGI << "Translation is ended. Nothing to translate";
    return;
  }

  ValidateIsCorrectStmtType(ast_, StmtType::kProgram);
  for (unsigned i = 0; HasChildren(ast_, i + 1); ++i) {
    auto query = ast_->Child(i);
    ValidateIsCorrectStmtType(query, StmtType::kQuery);
    TranslateQuery(query);
  }

  LOGI << "Translation is ended";
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
