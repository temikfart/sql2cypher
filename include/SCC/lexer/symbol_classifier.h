#pragma once

#include <cctype>

#include <algorithm>
#include <string>

#include "SCC/lexer/symbol_types.h"

class SymbolClassifier {
public:
  static SymbolType DefineSymbolType(char c);
  static bool IsSpace(char c);
  static bool IsDigit(char c);
  static bool IsAlpha(char c);
  static bool IsOperator(char c);
  static bool IsBracket(char c);
  static bool IsPunctuation(char c);
  static bool IsNullTerminator(char c);
  static bool IsEOF(char c);

private:
  static bool IsCharFromString(char c, const std::string& str);
};
