#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

class RootNode : public INode {
public:
  explicit RootNode();

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;
};
