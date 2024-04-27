#include "SCC/query-assembler/query_assembler.h"

namespace scc::query_assembler {

using namespace ast;

QueryAssembler::QueryAssembler(std::shared_ptr<INode> ast, const std::filesystem::path& out_path)
    : ast_(std::move(ast)), out_(out_path) {}

void QueryAssembler::Translate() {
  LOGI << "starting translation...";
  if (ast_ == nullptr) {
    LOGI << "translation is ended: nothing to translate";
    return;
  }

  if (ast_->stmt_type == StmtType::kProgram) {
    if (ast_->ChildrenCount() > 0) {
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
  if (query->stmt_type == StmtType::kQuery) {
    this->TranslateQuery(query);
  } else {
    LOGE << "first child is not a query";
    end(EXIT_FAILURE);
  }

  if (node->ChildrenCount() > 1) {
    auto other_queries = node->get_child(1);
    if (other_queries->stmt_type == StmtType::kSemicolonDelimiter) {
      if (other_queries->ChildrenCount() > 0) {
        this->TranslateProgram(other_queries);
      }
    } else {
      LOGE << "invalid delimiter between queries";
      end(EXIT_FAILURE);
    }
  }
}
void QueryAssembler::TranslateQuery(std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGD << "empty query";
    return;
  }

  auto data_language = node->get_child(0);
  switch (data_language->stmt_type) {
    case StmtType::kDdlStmt:
      this->TranslateDDLStatement(data_language);
      break;
    case StmtType::kDmlStmt:
      this->TranslateDMLStatement(data_language);
      break;
    default:
      LOGE << "unknown query data language";
      end(EXIT_FAILURE);
  }
}

} // scc::query_assembler
