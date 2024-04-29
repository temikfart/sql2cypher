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
  TranslateProgram(ast_);

  LOGI << "Translation is ended";
}

void Translator::TranslateProgram(const NodePtr<INode>& program) {
  auto query = program->get_child(0);
  ValidateIsCorrectStmtType(query, StmtType::kQuery);
  TranslateQuery(query);

  if (HasChildren(program, 2)) {
    auto other_queries = program->get_child(1);
    ValidateIsCorrectStmtType(other_queries, StmtType::kSemicolonDelimiter);
    if (HasChildren(other_queries)) {
      TranslateProgram(other_queries);
    }
  }
}
void Translator::TranslateQuery(const NodePtr<INode>& query) {
  if (!HasChildren(query)) {
    LOGD << "Empty query";
    return;
  }

  auto statement_type = query->get_child(0);
  ValidateHasChildren(statement_type);
  auto statement = statement_type->get_child(0);
  ValidateHasChildren(statement);
  switch (statement_type->stmt_type) {
    case StmtType::kDdlStmt:
      TranslateDDLStatement(statement);
      break;
    case StmtType::kDmlStmt:
      TranslateDMLStatement(statement);
      break;
    default:
      throw translation_error("Unknown query statement type: \'"
                                  + statement_type->stmt_type.ToString() + "\'");
  }
}

} // scc::translator
