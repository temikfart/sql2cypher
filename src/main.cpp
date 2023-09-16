#include "SCC/main.h"

int main(int argc, char* argv[]) {
  logger::init(logger::debug, logger::cout);

  LOG_OLD(INFO, "Starting system...");
  config.Start(argc, argv);
  LOG_OLD(INFO, "System started");

  Tokenizer tokenizer;
  tokenizer.Tokenize();

  SyntaxAnalyzer syntax_analyzer;
  std::shared_ptr<Node> AST =
      syntax_analyzer.Analyze(tokenizer.get_tokens_array());

  if (config.get_is_need_dump()) {
    TreeDump dump;
    dump.DumpTree(AST);
  }

  QueryAssembler query_assembler;
  query_assembler.Translate(AST);

  end(EXIT_SUCCESS);

  return 0;
}
