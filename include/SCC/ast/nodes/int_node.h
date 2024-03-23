#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

class IntNumNode : public INode {
public:
  int data;

  explicit IntNumNode(int value);

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;
};
