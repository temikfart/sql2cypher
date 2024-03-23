#include "SCC/query_assembler.h"

void QueryAssembler::TranslateDMLStatement(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGD << "empty DML query";
  }

  auto statement = node->get_child(0);
  switch (statement->stmt_type) {
    case StmtType::insertStatement:
      this->TranslateInsert(statement);
      break;
    case StmtType::deleteStatement:
      this->TranslateDelete(statement);
      break;
    case StmtType::updateStatement:
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
