#include "SCC/query_assembler.h"

void QueryAssembler::Translate(std::shared_ptr<Node> AST) {
  LOG_OLD(INFO, "starting translation...");

  ast_ = std::move(AST);

  if (ast_ == nullptr) {
    LOG_OLD(INFO, "translation is ended: nothing to translate");
    return;
  }

  if (ast_->get_st_type() == StatementType::Program) {
    if (ast_->get_children_amount() > 0) {
      this->TranslateProgram(ast_);
    } else {
      LOG_OLD(INFO, "translation is ended: only one node in AST");
      return;
    }
  } else {
    LOG_OLD(ERROR,
        "invalid AST: root should be with \'Program\' statement type");
    end(EXIT_FAILURE);
  }

  LOG_OLD(INFO, "translation is ended");
}

void QueryAssembler::TranslateProgram(std::shared_ptr<Node> node) {
  auto query = node->get_child(0);
  if (query->get_st_type() == StatementType::query) {
    this->TranslateQuery(query);
  } else {
    LOG_OLD(ERROR, "first child is not a query");
    end(EXIT_FAILURE);
  }

  if (node->get_children_amount() > 1) {
    auto other_queries = node->get_child(1);
    if (other_queries->get_st_type() == StatementType::delimiter_semicolon) {
      if (other_queries->get_children_amount() > 0) {
        this->TranslateProgram(other_queries);
      }
    } else {
      LOG_OLD(ERROR, "invalid delimiter between queries");
      end(EXIT_FAILURE);
    }
  }
}
void QueryAssembler::TranslateQuery(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOG_OLD(TRACE, "empty query");
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
      LOG_OLD(ERROR, "unknown query data language");
      end(EXIT_FAILURE);
  }
}
