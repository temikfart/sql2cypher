#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

class ServiceNode : public INode {
public:
  explicit ServiceNode();

  std::ostream& operator<<(std::ostream& os) override;
};
