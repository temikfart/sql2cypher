#pragma once

#include <iostream>

#include "inode.h"
#include "SCC/ast/data_types.h"

class CharNode : public INode {
public:
  explicit CharNode(char ch, DataType type);

  char get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  char data_;

  void ValidateType(DataType type) const override;
};
