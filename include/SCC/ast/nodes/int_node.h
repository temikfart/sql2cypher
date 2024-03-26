#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

namespace scc::ast {

class IntNumNode : public INode {
public:
  int data;

  explicit IntNumNode(int value);

  std::ostream& operator<<(std::ostream& os) override;
};

} // scc::ast
