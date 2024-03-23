#include "SCC/lexer/lexical_analyzer.h"

Lexer::Lexer(const std::filesystem::path& input_path)
    : input_(input_path) {}

std::deque<std::shared_ptr<INode>> Lexer::Analyze() {
  LOGI << "tokenizing...";

  while (true) {
    char symbol = PeekSymbol();
    if (TokenClassifier::IsSpace(symbol)) {
      if (symbol == '\n')
        line_number_++;
      GetSymbol();
    } else if (TokenClassifier::IsDigit(symbol)) {
      GetNumber();
    } else if (TokenClassifier::IsAlpha(symbol)) {
      GetWord();
    } else if (TokenClassifier::IsOperator(symbol)) {
      GetOperator();
    } else if (TokenClassifier::IsBracket(symbol)) {
      GetCharacter(DataType::kBracket);
    } else if (TokenClassifier::IsPunctuation(symbol)) {
      GetCharacter(DataType::kPunctuation);
    } else if (TokenClassifier::IsEOF(symbol) || TokenClassifier::IsNullTerminator(symbol)) {
      break;
    } else {
      LOGE << "Unknown symbol \'" << symbol
           << "\' in line " << line_number_;
      end(EXIT_FAILURE);
    }
  }

  LOGI << "lexical analysis is ended";

  return tokens_;
}

char Lexer::GetSymbol() {
  return (char) input_.get();
}
char Lexer::PeekSymbol() {
  return (char) input_.peek();
}

void Lexer::GetNumber() {
  LOGD << "getting a number...";

  double data = 0.0;
  double power = 1.0;

  while (TokenClassifier::IsDigit(PeekSymbol()))
    data = 10.0 * data + GetSymbol() - '0';

  if (PeekSymbol() == '.') {
    GetSymbol();
  } else {
    tokens_.push_back(std::make_shared<IntNumNode>((int) data));
    LOGD << "got the integer number";
    return;
  }

  while (TokenClassifier::IsDigit(PeekSymbol())) {
    data = 10.0 * data + GetSymbol() - '0';
    power *= 10.0;
  }
  data = data / power;

  auto num_node = std::make_shared<FloatNumNode>(data);
  num_node->line = line_number_;
  tokens_.push_back(num_node);
  LOGD << "got the float number";
}
void Lexer::GetWord() {
  LOGD << "getting a word...";
  std::ostringstream data;

  char another_symbol = PeekSymbol();
  while (TokenClassifier::IsAlpha(another_symbol) || TokenClassifier::IsDigit(another_symbol)
      || another_symbol == '_') {
    data << GetSymbol();
    another_symbol = PeekSymbol();
  }

  auto word_node = std::make_shared<StringNode>(data.str(), DataType::kWord);
  word_node->line = line_number_;
  tokens_.push_back(word_node);
  LOGD << "got the word";
}
void Lexer::GetOperator() {
  LOGD << "getting an operator...";
  std::ostringstream data;

  data << GetSymbol();
  while (TokenClassifier::IsOperator(PeekSymbol()))
    data << GetSymbol();

  auto op_node = std::make_shared<StringNode>(data.str(), DataType::kOperator);
  op_node->line = line_number_;
  tokens_.push_back(op_node);
  LOGD << "got the operator";
}
void Lexer::GetCharacter(DataType type) {
  LOGD << "getting a character...";
  auto char_node = std::make_shared<CharNode>(GetSymbol(), type);
  char_node->line = line_number_;
  tokens_.push_back(char_node);
  LOGD << "got the character";
}
