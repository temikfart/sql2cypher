#include "SCC/main.h"

int main(int argc, char* argv[]) {
  LOG(INFO, "Starting system...");
  config.Start(argc, argv);
  LOG(INFO, "System started");

  Tokenizer tokenizer;
  tokenizer.Tokenize();
//  tokenizer.PrintTokens();

  SyntaxAnalyzer syntax_analyzer;

  std::shared_ptr<Node> AST =
      syntax_analyzer.Analyze(tokenizer.get_tokens_array());

  TreeDump dump;
  dump.DumpTree(AST);

  QueryAssembler query_assembler;
  query_assembler.Translate(AST);

  config.CloseOutputFile();
  config.CloseInputFile();

  return 0;
}
