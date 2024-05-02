#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

void Translator::TranslateDMLStatement(const NodePtr<INode>& dml_statement) {
  switch (dml_statement->stmt_type) {
    case StmtType::kInsertStmt:
      TranslateInsertStatement(dml_statement);
      break;
    case StmtType::kDeleteStmt:
      TranslateDeleteStatement(dml_statement);
      break;
    case StmtType::kUpdateStmt:
      TranslateUpdateStatement(dml_statement);
      break;
    default:
      throw translation_error("Unknown DML statement type: \'"
                                  + dml_statement->stmt_type.ToString() + "\'");
  }
}

void Translator::TranslateInsertStatement(const NodePtr<INode>& insert_stmt) {
  ValidateHasChildren(insert_stmt, 1, "Empty \'INSERT\' statement");
}
void Translator::TranslateDeleteStatement(const NodePtr<INode>& delete_stmt) {
  ValidateHasChildren(delete_stmt, 1, "Empty \'DELETE\' statement");
}
void Translator::TranslateUpdateStatement(const NodePtr<INode>& update_stmt) {
  ValidateHasChildren(update_stmt, 1, "Empty \'UPDATE\' statement");
}

} // scc::translator
