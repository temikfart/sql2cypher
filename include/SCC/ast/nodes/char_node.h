#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

namespace scc::ast {

class CharNode : public INode {
public:
  char data;

  CharNode(char ch, DataType data_type);
};

} // scc::ast
