#include "SCC/lexical_analyzer.h"

//-------------------Tokenizer-------------------

std::shared_ptr<Node>& Tokenizer::peek_last_token() {
  return tokens_array_.back();
}
std::shared_ptr<Node>& Tokenizer::peek_first_token() {
  return tokens_array_.front();
}
std::shared_ptr<Node> Tokenizer::get_first_token() {
  std::shared_ptr<Node> node = tokens_array_.front();
  tokens_array_.pop_front();
  return node;
}

void Tokenizer::PrintTokens() {
  for(const auto& token: tokens_array_) {
    std::cout << "data: ", token->PrintData(std::cout);
    std::cout << " | ";
    std::cout << "type: ", token->PrintType(std::cout);
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
void Tokenizer::Tokenize() {
  int string_number = 1;
  while(true) {
    char another_symbol = config.PeekSQLSymbol();
    if (std::isspace(another_symbol)) {
      if (another_symbol == '\n')
        string_number++;
      config.GetSQLSymbol();
    } else if (std::isdigit(another_symbol)) {
      this->GetNumber();
    } else if (std::isalpha(another_symbol)) {
      this->GetWord();
    } else if (Tokenizer::IsOperator(another_symbol)) {
      this->GetOperator();
    } else if (Tokenizer::IsBracket(another_symbol)) {
      this->GetCharacter(DataType::BRACKET);
    } else if (Tokenizer::IsPunctuation(another_symbol)) {
      this->GetCharacter(DataType::PUNCTUATION);
    } else if (another_symbol == EOF || another_symbol == '\0') {
      break;
    } else {
      std::ostringstream message;
      message << "Unknown symbol \'" << another_symbol
        << "\' at string " << string_number;
      LOG(ERROR, message.str());
      exit(EXIT_FAILURE);
    }
  }
}
std::deque<std::shared_ptr<Node>> Tokenizer::get_tokens_array() const {
  return tokens_array_;
}

void Tokenizer::GetNumber() {
  double data = 0.0;
  double power = 1.0;

  while (isdigit(config.PeekSQLSymbol()))
    data = 10.0 * data + config.GetSQLSymbol() - '0';

  if (config.PeekSQLSymbol() == '.') {
    config.GetSQLSymbol();
  } else {
    tokens_array_.push_back(std::make_shared<IntNumNode>((int)data));
    return;
  }

  while (isdigit(config.PeekSQLSymbol())) {
    data = 10.0 * data + config.GetSQLSymbol() - '0';
    power *= 10.0;
  }
  data = data / power;

  tokens_array_.push_back(std::make_shared<FloatNumNode>(data));
}
void Tokenizer::GetWord() {
  std::ostringstream data;

  char another_symbol = config.PeekSQLSymbol();
  while (isalpha(another_symbol) || isdigit(another_symbol)
         || another_symbol == '_') {
    data << config.GetSQLSymbol();
    another_symbol = config.PeekSQLSymbol();
  }

  tokens_array_.push_back(
    std::make_shared<StringNode>(data.str(),DataType::WORD));
}
void Tokenizer::GetOperator() {
  std::ostringstream data;

  data << config.GetSQLSymbol();
  while (IsOperator(config.PeekSQLSymbol()))
    data << config.GetSQLSymbol();

  tokens_array_.push_back(
    std::make_shared<StringNode>(data.str(), DataType::OPERATOR));
}
void Tokenizer::GetCharacter(DataType type) {
  tokens_array_.push_back(
    std::make_shared<CharNode>(config.GetSQLSymbol(), type));
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
