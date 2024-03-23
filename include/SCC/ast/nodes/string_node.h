#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

class StringNode : public INode {
public:
  std::string data;

  explicit StringNode(std::string string, DataType type);

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;
};
