#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

void Translator::TranslateDMLStatement(const NodePtr<INode>& dml_statement) {
  switch (dml_statement->stmt_type) {
    case StmtType::kInsertStmt:
      TranslateInsert(statement);
      break;
    case StmtType::kDeleteStmt:
      TranslateDelete(statement);
      break;
    case StmtType::kUpdateStmt:
      TranslateUpdate(statement);
      break;
    default:
      LOGE << "unknown DML statement";
      end(EXIT_FAILURE);
  }
}

void Translator::TranslateInsert(const NodePtr<INode>& node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "insert statement without body";
    end(EXIT_FAILURE);
  }
}
void Translator::TranslateDelete(const NodePtr<INode>& node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "delete statement without body";
    end(EXIT_FAILURE);
  }
}
void Translator::TranslateUpdate(const NodePtr<INode>& node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "update statement without body";
    end(EXIT_FAILURE);
  }
}

} // scc::translator
