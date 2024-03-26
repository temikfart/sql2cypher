#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

namespace scc::ast {

class FloatNumNode : public INode {
public:
  double data;

  explicit FloatNumNode(double value);

  std::ostream& operator<<(std::ostream& os) override;
};

} // scc::ast
