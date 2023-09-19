#include "SCC/query_assembler.h"

void QueryAssembler::Translate(std::shared_ptr<Node> AST) {
  LOGI << "starting translation...";

  ast_ = std::move(AST);

  if (ast_ == nullptr) {
    LOGI << "translation is ended: nothing to translate";
    return;
  }

  if (ast_->get_st_type() == StatementType::Program) {
    if (ast_->get_children_amount() > 0) {
      this->TranslateProgram(ast_);
    } else {
      LOGI << "translation is ended: only one node in AST";
      return;
    }
  } else {
    LOGE <<
        "invalid AST: root should be with \'Program\' statement type";
    end(EXIT_FAILURE);
  }

  LOGI << "translation is ended";
}

void QueryAssembler::TranslateProgram(std::shared_ptr<Node> node) {
  auto query = node->get_child(0);
  if (query->get_st_type() == StatementType::query) {
    this->TranslateQuery(query);
  } else {
    LOGE << "first child is not a query";
    end(EXIT_FAILURE);
  }

  if (node->get_children_amount() > 1) {
    auto other_queries = node->get_child(1);
    if (other_queries->get_st_type() == StatementType::delimiter_semicolon) {
      if (other_queries->get_children_amount() > 0) {
        this->TranslateProgram(other_queries);
      }
    } else {
      LOGE << "invalid delimiter between queries";
      end(EXIT_FAILURE);
    }
  }
}
void QueryAssembler::TranslateQuery(std::shared_ptr<Node> node) {
  if (node->get_children_amount() == 0) {
    LOGD << "empty query";
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
      LOGE << "unknown query data language";
      end(EXIT_FAILURE);
  }
}
