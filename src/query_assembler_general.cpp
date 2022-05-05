#include "SCC/query_assembler.h"

void QueryAssembler::Translate(std::shared_ptr<Node> AST) {
  LOG(INFO, "Starting translation...");

  ast_ = std::move(AST);

  if (ast_ == nullptr) {
    LOG(TRACE, "empty AST, nothing to translate");
    LOG(INFO, "Translation is ended");
    return;
  }

  if (ast_->get_st_type() == StatementType::Program) {
    if (ast_->get_children_amount() > 0) {
      this->TranslateProgram(ast_);
    } else {
      LOG(TRACE, "root of AST does not have children");
      LOG(INFO, "Translation is ended");
      return;
    }
  } else {
    LOG(ERROR,
        "invalid AST: root should be with \'Program\' statement type");
    exit(EXIT_FAILURE);
  }

  LOG(INFO, "Translation is ended");
}

void QueryAssembler::TranslateProgram(std::shared_ptr<Node> node) {
  auto query = node->get_child(0);
  if (query->get_st_type() == StatementType::query) {
    this->TranslateQuery(query);
  } else {
    LOG(ERROR, "first child of program node is not a query");
    exit(EXIT_FAILURE);
  }

  if (node->get_children_amount() > 1) {
    auto other_queries = node->get_child(1);
    if (other_queries->get_st_type() == StatementType::delimiter_semicolon) {
      if (other_queries->get_children_amount() > 0) {
        this->TranslateProgram(other_queries);
      }
    } else {
      LOG(ERROR, "invalid delimiter between queries");
      exit(EXIT_FAILURE);
    }
  }
}
void QueryAssembler::TranslateQuery(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG(TRACE, "empty query");
    return;
  }

  auto data_language = node->get_child(0);
  switch (data_language->get_st_type()) {
    case StatementType::ddlStatement:
      this->TranslateDDLStatement(data_language);
      break;
    case StatementType::dmlStatement:
      this->TranslateDMLStatement(data_language);
      break;
    default:
      LOG(ERROR, "unknown query data language");
      exit(EXIT_FAILURE);
  }
}
