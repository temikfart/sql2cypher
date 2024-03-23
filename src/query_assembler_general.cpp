#include "SCC/query_assembler.h"

QueryAssembler::QueryAssembler(const std::filesystem::path& out_path)
: out_(out_path) {}

bool QueryAssembler::CloseOutputFile() {
  if (out_.is_open()) {
    out_.close();
    if (out_.good()) {
      ;
    } else {
      std::cerr << "output file close error" << std::endl;
      return false;
    }
  }
  return true;
}

void QueryAssembler::Translate(std::shared_ptr<INode> AST) {
  LOGI << "starting translation...";

  ast_ = std::move(AST);

  if (ast_ == nullptr) {
    LOGI << "translation is ended: nothing to translate";
    return;
  }

  if (ast_->stmt_type == StmtType::Program) {
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

void QueryAssembler::TranslateProgram(std::shared_ptr<INode> node) {
  auto query = node->get_child(0);
  if (query->stmt_type == StmtType::query) {
    this->TranslateQuery(query);
  } else {
    LOGE << "first child is not a query";
    end(EXIT_FAILURE);
  }

  if (node->get_children_amount() > 1) {
    auto other_queries = node->get_child(1);
    if (other_queries->stmt_type == StmtType::delimiter_semicolon) {
      if (other_queries->get_children_amount() > 0) {
        this->TranslateProgram(other_queries);
      }
    } else {
      LOGE << "invalid delimiter between queries";
      end(EXIT_FAILURE);
    }
  }
}
void QueryAssembler::TranslateQuery(std::shared_ptr<INode> node) {
  if (node->get_children_amount() == 0) {
    LOGD << "empty query";
    return;
  }

  auto data_language = node->get_child(0);
  switch (data_language->stmt_type) {
    case StmtType::ddlStatement:
      this->TranslateDDLStatement(data_language);
      break;
    case StmtType::dmlStatement:
      this->TranslateDMLStatement(data_language);
      break;
    default:
      LOGE << "unknown query data language";
      end(EXIT_FAILURE);
  }
}
