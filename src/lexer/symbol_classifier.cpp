#include "SCC/lexer/symbol_classifier.h"

SymbolType SymbolClassifier::DefineSymbolType(char c) {
  if (IsSpace(c))
    return SymbolType::kSpace;
  if (IsDigit(c))
    return SymbolType::kDigit;
  if (IsAlpha(c))
    return SymbolType::kAlpha;
  if (IsOperator(c))
    return SymbolType::kOperator;
  if (IsBracket(c))
    return SymbolType::kBracket;
  if (IsPunctuation(c))
    return SymbolType::kPunctuation;
  if (IsEOF(c))
    return SymbolType::kEOF;
  if (IsNullTerminator(c))
    return SymbolType::kNullTerminator;

  return SymbolType::kUnknown;
}
bool SymbolClassifier::IsSpace(char c) {
  return std::isspace(c) != 0;
}
bool SymbolClassifier::IsDigit(char c) {
  return std::isdigit(c) != 0;
}
bool SymbolClassifier::IsAlpha(char c) {
  return std::isalpha(c) != 0;
}
bool SymbolClassifier::IsOperator(char c) {
  return IsCharFromString(c, "+-*/^!=<>");
}
bool SymbolClassifier::IsBracket(char c) {
  return IsCharFromString(c, "(){}[]");
}
bool SymbolClassifier::IsPunctuation(char c) {
  return IsCharFromString(c, ":;.,\'\"");
}
bool SymbolClassifier::IsNullTerminator(char c) {
  return c == '\0';
}
bool SymbolClassifier::IsEOF(char c) {
  return c == EOF;
}

bool SymbolClassifier::IsCharFromString(char c, const std::string& str) {
  return std::any_of(str.begin(), str.end(), [c](char op) {
    return (c == op);
  });
}
