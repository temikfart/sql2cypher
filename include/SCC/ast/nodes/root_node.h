#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

namespace scc::ast {

class RootNode : public INode {
public:
  explicit RootNode();
  explicit RootNode(StmtType stmt_type);

  std::ostream& operator<<(std::ostream& os) override;
};

} // scc::ast
