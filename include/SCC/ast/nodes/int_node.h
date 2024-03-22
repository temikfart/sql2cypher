#pragma once

#include <iostream>

#include "inode.h"
#include "SCC/ast/data_types.h"

class IntNumNode : public INode {
public:
  explicit IntNumNode(int value, DataType type = DataType::INT_NUMBER);

  int get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  int data_;

  void ValidateType(DataType type) const override;
};
