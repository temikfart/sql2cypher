#include "SCC/lexer/lexer.h"

Lexer::Lexer(const std::filesystem::path& input_path)
    : input_(input_path) {}

std::deque<std::shared_ptr<INode>> Lexer::Analyze() {
  LOGI << "tokenizing...";

  while (true) {
    char symbol = PeekSymbol();
    SymbolType sym_type = SymbolClassifier::DefineSymbolType(symbol);

    if (sym_type == SymbolType::kSpace) {
      if (symbol == '\n')
        line_++;
      GetSymbol();
      continue;
    }

    std::shared_ptr<INode> token = GetToken(symbol, sym_type);
    if (token == nullptr)
      break;

    token->line = line_;
    tokens_.push_back(token);
  }

  LOGI << "lexical analysis is ended";
  return tokens_;
}
std::shared_ptr<INode> Lexer::GetToken(char symbol, SymbolType sym_type) {
  switch (sym_type) {
    case SymbolType::kDigit:
      return GetNumber();
    case SymbolType::kAlpha:
      return GetWord();
    case SymbolType::kOperator:
      return GetOperator();
    case SymbolType::kBracket:
      return GetCharacter(DataType::kBracket);
    case SymbolType::kPunctuation:
      return GetCharacter(DataType::kPunctuation);
    case SymbolType::kEOF:
    case SymbolType::kNullTerminator:
      return nullptr;
    default:
      std::string msg = "Unknown symbol \'"
          + std::to_string(symbol)
          + "\' in line "
          + std::to_string(line_);
      LOGE << msg;
      throw std::invalid_argument(msg);
  }
}

char Lexer::GetSymbol() {
  return (char) input_.get();
}
char Lexer::PeekSymbol() {
  return (char) input_.peek();
}

int Lexer::GetDigit() {
  return static_cast<int>(GetSymbol() - '0');
}
std::shared_ptr<INode> Lexer::GetNumber() {
  LOGD << "getting a number...";

  double data = 0.0;
  double power = 1.0;

  while (SymbolClassifier::IsDigit(PeekSymbol()))
    data = 10.0 * data + GetDigit();

  if (PeekSymbol() == '.') {
    GetSymbol();
  } else {
    LOGD << "got the integer number";
    return std::make_shared<IntNumNode>(static_cast<int>(data));
  }

  while (SymbolClassifier::IsDigit(PeekSymbol())) {
    data = 10.0 * data + GetSymbol() - '0';
    power *= 10.0;
  }
  data = data / power;

  LOGD << "got the float number";
  return std::make_shared<FloatNumNode>(data);
}
std::shared_ptr<INode> Lexer::GetWord() {
  LOGD << "getting a word...";
  std::ostringstream data;

  char another_symbol = PeekSymbol();
  while (SymbolClassifier::IsAlpha(another_symbol) || SymbolClassifier::IsDigit(another_symbol)
      || another_symbol == '_') {
    data << GetSymbol();
    another_symbol = PeekSymbol();
  }

  LOGD << "got the word";
  return std::make_shared<StringNode>(data.str(), DataType::kWord);
}
std::shared_ptr<INode> Lexer::GetOperator() {
  LOGD << "getting an operator...";
  std::ostringstream data;

  data << GetSymbol();
  while (SymbolClassifier::IsOperator(PeekSymbol()))
    data << GetSymbol();

  LOGD << "got the operator";
  return std::make_shared<StringNode>(data.str(), DataType::kOperator);
}
std::shared_ptr<INode> Lexer::GetCharacter(DataType type) {
  LOGD << "getting a character...";
  LOGD << "got the character";
  return std::make_shared<CharNode>(GetSymbol(), type);
}
