#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

class CharNode : public INode {
public:
  char data;

  CharNode(char ch, DataType data_type);

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;
};
