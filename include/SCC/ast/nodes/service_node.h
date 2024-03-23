#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

class ServiceNode : public INode {
public:
  explicit ServiceNode();

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;
};
