#pragma once

#include <deque>
#include <sstream>

#include "SCC/ast.h"
#include "SCC/config.h"
#include "SCC/log.h"

class Tokenizer {
public:
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
  std::deque<std::shared_ptr<Node>> tokens_array_;

  void GetNumber();
  void GetWord();
  void GetOperator();
  void GetCharacter(DataType type);
};
