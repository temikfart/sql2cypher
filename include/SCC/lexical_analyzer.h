#pragma once

#include "SCC/ast.h"
#include "SCC/config.h"

enum TokensConstants {

};

class Tokenizer {
private:
  std::vector<Node*> tokens_array_;
public:
  void PrintTokens();

  void Tokenize();

  void GetNumber();
};