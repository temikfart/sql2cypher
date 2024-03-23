#include "SCC/ast/stmt_types.h"

StmtType::StmtType(Value value) {
  if (value > SQL_varchar)
    throw std::invalid_argument("Incorrect value for Statement Type");
  this->value_ = value;
}
