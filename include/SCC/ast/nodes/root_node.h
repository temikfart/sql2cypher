#pragma once

#include <iostream>

#include "inode.h"
#include "SCC/ast/data_types.h"

class RootNode : public INode {
public:
  explicit RootNode();

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  void ValidateType(DataType type) const override;
};
