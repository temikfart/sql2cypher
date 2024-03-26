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
  double data = 0.0;
  double power = 1.0;
  const double kBase = 10.0;

  while (SymbolClassifier::IsDigit(PeekSymbol()))
    data = kBase * data + GetDigit();

  if (PeekSymbol() == '.') {
    GetSymbol();
  } else {
    return std::make_shared<IntNumNode>(static_cast<int>(data));
  }

  while (SymbolClassifier::IsDigit(PeekSymbol())) {
    data = kBase * data + GetDigit();
    power *= kBase;
  }
  data = data / power;

  return std::make_shared<FloatNumNode>(data);
}
std::shared_ptr<INode> Lexer::GetWord() {
  std::string data;

  char symbol = PeekSymbol();
  while (SymbolClassifier::IsAlpha(symbol)
      || SymbolClassifier::IsDigit(symbol)
      || symbol == '_') {
    data += GetSymbol();
    symbol = PeekSymbol();
  }

  return std::make_shared<StringNode>(data, DataType::kWord);
}
std::shared_ptr<INode> Lexer::GetOperator() {
  std::string data;

  data += GetSymbol();
  while (SymbolClassifier::IsOperator(PeekSymbol()))
    data += GetSymbol();

  return std::make_shared<StringNode>(data, DataType::kOperator);
}
std::shared_ptr<INode> Lexer::GetCharacter(DataType data_type) {
  return std::make_shared<CharNode>(GetSymbol(), data_type);
}
