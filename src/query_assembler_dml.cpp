#include "SCC/query_assembler.h"

namespace scc::query_assembler {

using namespace ast;

void QueryAssembler::TranslateDMLStatement(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGD << "empty DML query";
  }

  auto statement = node->get_child(0);
  switch (statement->stmt_type) {
    case StmtType::kInsertStmt:
      this->TranslateInsert(statement);
      break;
    case StmtType::kDeleteStmt:
      this->TranslateDelete(statement);
      break;
    case StmtType::kUpdateStmt:
      this->TranslateUpdate(statement);
      break;
    default:
      LOGE << "unknown DML statement";
      end(EXIT_FAILURE);
  }
}

void QueryAssembler::TranslateInsert(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "insert statement without body";
    end(EXIT_FAILURE);
  }
}
void QueryAssembler::TranslateDelete(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "delete statement without body";
    end(EXIT_FAILURE);
  }
}
void QueryAssembler::TranslateUpdate(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "update statement without body";
    end(EXIT_FAILURE);
  }
}

} // scc::query_assembler
