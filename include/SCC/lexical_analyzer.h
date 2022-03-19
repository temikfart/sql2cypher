#pragma once

#include <sstream>
#include "SCC/ast.h"
#include "SCC/config.h"
#include "SCC/log.h"

class Tokenizer {
private:
  std::vector<std::shared_ptr<Node>> tokens_array_;
  
public:
  void PrintTokens();
  void Tokenize();

private:
  void GetNumber();
  void GetWord();
  void GetOperator();
  void GetCharacter(DataType type);

public: //TODO:
  static bool IsOperator(char symbol);
  static bool IsBracket(char symbol);
  static bool IsPunctuation(char symbol);

  static bool IsCharacterFromArray(char ch, const std::string& array);
};
