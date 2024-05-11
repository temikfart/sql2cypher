#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

namespace scc::ast {

class ServiceNode : public INode {
public:
  explicit ServiceNode();
  explicit ServiceNode(StmtType stmt_type);
  explicit ServiceNode(StmtType stmt_type, int line);
};

} // scc::ast
