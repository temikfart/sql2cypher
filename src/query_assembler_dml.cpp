#include "SCC/query_assembler.h"

void QueryAssembler::TranslateDMLStatement(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(TRACE, "empty DML query");
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
      LOG(ERROR, "unknown DML statement");
      end(EXIT_FAILURE);
  }
}

void QueryAssembler::TranslateInsert(std::shared_ptr<Node> node) {}
void QueryAssembler::TranslateDelete(std::shared_ptr<Node> node) {}
void QueryAssembler::TranslateUpdate(std::shared_ptr<Node> node) {}
