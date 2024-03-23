#include "SCC/lexer/token_classifier.h"

bool TokenClassifier::IsSpace(char c) {
  return std::isspace(c) != 0;
}
bool TokenClassifier::IsDigit(char c) {
  return std::isdigit(c) != 0;
}
bool TokenClassifier::IsAlpha(char c) {
  return std::isalpha(c) != 0;
}
bool TokenClassifier::IsOperator(char c) {
  return IsCharFromString(c, "+-*/^!=<>");
}
bool TokenClassifier::IsBracket(char c) {
  return IsCharFromString(c, "(){}[]");
}
bool TokenClassifier::IsPunctuation(char c) {
  return IsCharFromString(c, ":;.,\'\"");
}
bool TokenClassifier::IsNullTerminator(char c) {
  return c == '\0';
}
bool TokenClassifier::IsEOF(char c) {
  return c == EOF;
}

bool TokenClassifier::IsCharFromString(char c, const std::string& str) {
  return std::any_of(str.begin(), str.end(), [c](char op) {
    return (c == op);
  });
}
