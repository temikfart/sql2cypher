#include "SCC/main.h"

int main(int argc, char* argv[]) {
  LOG(INFO, "Starting system...");
  Configure(argc, argv);
  LOG(INFO, "System started");

  Tokenizer tokenizer;
  tokenizer.Tokenize();
//  tokenizer.PrintTokens();

  std::shared_ptr<IntNumNode> root = std::make_shared<IntNumNode>(5, DataType::WORD);

  return 0;
}
