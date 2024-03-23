#include "SCC/lexical_analyzer.h"

//-------------------Tokenizer-------------------

Tokenizer::Tokenizer(const std::filesystem::path& input_path)
: input_(input_path) {}

void Tokenizer::Tokenize() {
  LOGI << "tokenizing...";

  line_number_ = 1;
  while (true) {
    char another_symbol = PeekSQLSymbol();
    if (std::isspace(another_symbol)) {
      if (another_symbol == '\n')
        line_number_++;
      GetSQLSymbol();
    } else if (std::isdigit(another_symbol)) {
      this->GetNumber();
    } else if (std::isalpha(another_symbol)) {
      this->GetWord();
    } else if (Tokenizer::IsOperator(another_symbol)) {
      this->GetOperator();
    } else if (Tokenizer::IsBracket(another_symbol)) {
      this->GetCharacter(DataType::kBracket);
    } else if (Tokenizer::IsPunctuation(another_symbol)) {
      this->GetCharacter(DataType::kPunctuation);
    } else if (another_symbol == EOF || another_symbol == '\0') {
      break;
    } else {
      LOGE << "Unknown symbol \'" << another_symbol
                                     << "\' in line " << line_number_;
      end(EXIT_FAILURE);
    }
  }

  LOGI << "lexical analysis is ended";
}

std::deque<std::shared_ptr<INode>> Tokenizer::get_tokens_array() const {
  return tokens_array_;
}

bool Tokenizer::IsOperator(char symbol) {
  return Tokenizer::IsCharacterFromArray(symbol, "+-*/^!=<>");
}
bool Tokenizer::IsBracket(char symbol) {
  return Tokenizer::IsCharacterFromArray(symbol, "(){}[]");
}
bool Tokenizer::IsPunctuation(char symbol) {
  return Tokenizer::IsCharacterFromArray(symbol, ":;.,\'\"");
}

bool Tokenizer::IsCharacterFromArray(char ch, const std::string& array) {
  return std::any_of(array.begin(), array.end(), [ch](char op) {
    return (ch == op);
  });
}

char Tokenizer::GetSQLSymbol() {
  return (char) input_.get();
}
char Tokenizer::PeekSQLSymbol() {
  return (char) input_.peek();
}
void Tokenizer::GetNumber() {
  LOGD << "getting a number...";

  double data = 0.0;
  double power = 1.0;

  while (isdigit(PeekSQLSymbol()))
    data = 10.0 * data + GetSQLSymbol() - '0';

  if (PeekSQLSymbol() == '.') {
    GetSQLSymbol();
  } else {
    tokens_array_.push_back(std::make_shared<IntNumNode>((int) data));
    LOGD << "got the integer number";
    return;
  }

  while (isdigit(PeekSQLSymbol())) {
    data = 10.0 * data + GetSQLSymbol() - '0';
    power *= 10.0;
  }
  data = data / power;

  auto num_node = std::make_shared<FloatNumNode>(data);
  num_node->line = line_number_;
  tokens_array_.push_back(num_node);
  LOGD << "got the float number";
}
void Tokenizer::GetWord() {
  LOGD << "getting a word...";
  std::ostringstream data;

  char another_symbol = PeekSQLSymbol();
  while (isalpha(another_symbol) || isdigit(another_symbol)
      || another_symbol == '_') {
    data << GetSQLSymbol();
    another_symbol = PeekSQLSymbol();
  }

  auto word_node = std::make_shared<StringNode>(data.str(), DataType::kWord);
  word_node->line = line_number_;
  tokens_array_.push_back(word_node);
  LOGD << "got the word";
}
void Tokenizer::GetOperator() {
  LOGD << "getting an operator...";
  std::ostringstream data;

  data << GetSQLSymbol();
  while (IsOperator(PeekSQLSymbol()))
    data << GetSQLSymbol();

  auto op_node = std::make_shared<StringNode>(data.str(), DataType::kOperator);
  op_node->line = line_number_;
  tokens_array_.push_back(op_node);
  LOGD << "got the operator";
}
void Tokenizer::GetCharacter(DataType type) {
  LOGD << "getting a character...";
  auto char_node = std::make_shared<CharNode>(GetSQLSymbol(), type);
  char_node->line = line_number_;
  tokens_array_.push_back(char_node);
  LOGD << "got the character";
}
