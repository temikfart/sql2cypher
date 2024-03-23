#pragma once

#include <iostream>

#include "inode.h"
#include "SCC/ast/data_types.h"

class FloatNumNode : public INode {
public:
  explicit FloatNumNode(double value, DataType type = DataType::kFloat);

  double get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  double data_;

  void ValidateType(DataType type) const override;
};
