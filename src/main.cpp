#include "SCC/main.h"

int main(int argc, char* argv[]) {
  LOG(INFO, "Starting system...");
  Configure(argc, argv);
  LOG(INFO, "System started");

//  Tree::TestTree();
  Tokenizer tokenizer;
  tokenizer.Tokenize();
  tokenizer.PrintTokens();

  return 0;
}
