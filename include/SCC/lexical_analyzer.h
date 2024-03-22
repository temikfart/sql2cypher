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
#include "SCC/log.h"

#include "logger/log.hpp"

class Tokenizer {
public:
  explicit Tokenizer(const std::filesystem::path& input_path);

  void Tokenize();

  std::deque<std::shared_ptr<INode>> get_tokens_array() const;

  static bool IsOperator(char symbol);
  static bool IsBracket(char symbol);
  static bool IsPunctuation(char symbol);

  static bool IsCharacterFromArray(char ch, const std::string& array);

private:
  std::ifstream input_;
  int line_number_ = 0;
  std::deque<std::shared_ptr<INode>> tokens_array_;

  char GetSQLSymbol();
  char PeekSQLSymbol();

  void GetNumber();
  void GetWord();
  void GetOperator();
  void GetCharacter(DataType type);
};
