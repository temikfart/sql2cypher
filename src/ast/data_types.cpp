#include "SCC/ast/data_types.h"

DataType::DataType(Value value) {
  if (value > STRING)
    throw std::invalid_argument("Incorrect value for Data Type");
  this->value_ = value;
}
