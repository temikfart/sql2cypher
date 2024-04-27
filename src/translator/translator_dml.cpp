#include "SCC/translator/translator.h"

namespace scc::translator {

using namespace ast;

void Translator::TranslateDMLStatement(std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGD << "empty DML query";
  }

  auto statement = node->get_child(0);
  switch (statement->stmt_type) {
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

void Translator::TranslateInsert(std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "insert statement without body";
    end(EXIT_FAILURE);
  }
}
void Translator::TranslateDelete(std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "delete statement without body";
    end(EXIT_FAILURE);
  }
}
void Translator::TranslateUpdate(std::shared_ptr<INode> node) {
  if (node->ChildrenCount() == 0) {
    LOGE << "update statement without body";
    end(EXIT_FAILURE);
  }
}

} // scc::translator
