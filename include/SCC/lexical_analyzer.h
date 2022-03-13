#pragma once

#include "SCC/ast.h"
#include "SCC/config.h"
#include "SCC/log.h"
#include <sstream>

enum TokensConstants {

};

class Tokenizer {
  std::vector<Node*> tokens_array_;
public:
  ~Tokenizer();

  void PrintTokens();
  void Tokenize();

private:
  void GetNumber();
  void GetWord();
  void GetOperator();
  void GetCharacter(DataType type);

  static bool IsOperator(char symbol);
  static bool IsBracket(char symbol);
  static bool IsPunctuation(char symbol);

  static bool IsCharacterFromArray(char ch, const std::string& array);
};