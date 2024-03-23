#pragma once

#include <deque>
#include <filesystem>
#include <sstream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"
#include "SCC/config/scc_config.h"
#include "SCC/lexer/token_classifier.h"
#include "SCC/log.h"

#include "logger/log.hpp"

class Lexer {
public:
  explicit Lexer(const std::filesystem::path& input_path);

  std::deque<std::shared_ptr<INode>> Analyze();

private:
  std::ifstream input_;
  int line_number_ = 1;
  std::deque<std::shared_ptr<INode>> tokens_;

  char GetSymbol();
  char PeekSymbol();

  void GetNumber();
  void GetWord();
  void GetOperator();
  void GetCharacter(DataType type);
};
