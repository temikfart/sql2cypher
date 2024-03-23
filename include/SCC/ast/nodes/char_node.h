#pragma once

#include <iostream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/data_types.h"

class CharNode : public INode {
public:
  char data;

  CharNode(char ch, DataType data_type);

  std::ostream& operator<<(std::ostream& os) override;
};
