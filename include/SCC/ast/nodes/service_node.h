#pragma once

#include <iostream>

#include "inode.h"
#include "SCC/ast/data_types.h"

class ServiceNode : public INode {
public:
  explicit ServiceNode();

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  void ValidateType(DataType type) const override;
};
