#pragma once

#include <deque>
#include <filesystem>
#include <sstream>

#include "SCC/ast/ast.h"
#include "SCC/config/scc_config.h"
#include "SCC/log.h"

#include "logger/log.hpp"

class Tokenizer {
public:
  explicit Tokenizer(const std::filesystem::path& input_path);

  void Tokenize();

  std::shared_ptr<Node>& peek_first_token();
  std::shared_ptr<Node>& peek_last_token();
  std::shared_ptr<Node> get_first_token();
  std::deque<std::shared_ptr<Node>> get_tokens_array() const;

  static bool IsOperator(char symbol);
  static bool IsBracket(char symbol);
  static bool IsPunctuation(char symbol);

  static bool IsCharacterFromArray(char ch, const std::string& array);

  void PrintTokens();

private:
  std::ifstream input_;
  int line_number_ = 0;
  std::deque<std::shared_ptr<Node>> tokens_array_;

  char GetSQLSymbol();
  char PeekSQLSymbol();
  bool CloseInputFile();

  void GetNumber();
  void GetWord();
  void GetOperator();
  void GetCharacter(DataType type);
};
