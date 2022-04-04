#include "SCC/main.h"

int main(int argc, char* argv[]) {
  LOG(INFO, "Starting system...");
  Configure(argc, argv);
  LOG(INFO, "System started");

  Tokenizer tokenizer;
  tokenizer.Tokenize();
  tokenizer.PrintTokens();

  config.CloseOutputFile();
  config.CloseInputFile();
  
  return 0;
}
