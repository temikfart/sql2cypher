#include "SCC/lexical_analyzer.h"

//-------------------Tokenizer-------------------

void Tokenizer::PrintTokens() {
  for(auto token: tokens_array_)
    token->PrintData();
  std::cout << std::endl;
}

void Tokenizer::Tokenize() {
  char another_symbol;
  while(true) {
    config.ReadSQL() >> another_symbol;
    if (std::isdigit(another_symbol))
      this->GetNumber();
//    else if (std::isalpha(another_symbol))
//      this->GetWord();
//    else if (IsOperator(another_symbol))
//      this->GetOperator();
    else if (another_symbol == EOF)
      break;
  }
}

void Tokenizer::GetNumber() {
  double data = 0.0;
  double power = 1.0;

  while (isdigit(config.CheckLastSymbolSQL()))
    data = 10.0 * data + config.GetLastSymbolSQL() - '0';
  if (config.CheckLastSymbolSQL() == '.')
    config.GetLastSymbolSQL();
  while (isdigit(config.CheckLastSymbolSQL())) {
    data = 10.0 * data + config.GetLastSymbolSQL() - '0';
    power *= 10.0;
  }
  data = data / power;

  tokens_array_.push_back(new NumNode<double>(data)); //TODO: fix bug
}
