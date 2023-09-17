#include "SCC/query_assembler.h"

void QueryAssembler::TranslateDMLStatement(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGT << "empty DML query";
  }

  auto statement = node->get_child(0);
  switch (statement->get_st_type()) {
    case StatementType::insertStatement:
      this->TranslateInsert(statement);
      break;
    case StatementType::deleteStatement:
      this->TranslateDelete(statement);
      break;
    case StatementType::updateStatement:
      this->TranslateUpdate(statement);
      break;
    default:
      LOGE << "unknown DML statement";
      end(EXIT_FAILURE);
  }
}

void QueryAssembler::TranslateInsert(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "insert statement without body";
    end(EXIT_FAILURE);
  }
}
void QueryAssembler::TranslateDelete(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "delete statement without body";
    end(EXIT_FAILURE);
  }
}
void QueryAssembler::TranslateUpdate(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGE << "update statement without body";
    end(EXIT_FAILURE);
  }
}
