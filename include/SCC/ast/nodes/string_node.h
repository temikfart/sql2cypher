#pragma once

#include <iostream>

#include "inode.h"
#include "SCC/ast/data_types.h"

class StringNode : public INode {
public:
  explicit StringNode(std::string string, DataType type);

  void set_data(const std::string& data, DataType type);
  void set_data(const std::string& data);
  std::string get_data() const;

  void PrintData(std::ostream& stream) override;
  void PrintType(std::ostream& stream) override;

private:
  std::string data_;

  void ValidateType(DataType type) const override;
};
