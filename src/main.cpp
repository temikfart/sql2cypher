#include "SCC/main.h"

int main(int argc, char* argv[]) {
  SCCArgs scc_args = SCCArgs();
  scc_args.ParseArgs(argc, argv);

  scc::InitConfig(scc_args);
  scc::SCCConfig* config = scc::SCCConfig::Get();

  logger::init(config.log_severity, config.log_directory);

  Tokenizer tokenizer(config->get_sql_file());
  tokenizer.Tokenize();

  SyntaxAnalyzer syntax_analyzer;
  std::shared_ptr<Node> AST =
      syntax_analyzer.Analyze(tokenizer.get_tokens_array());

  if (scc_args.IsUsed("--dump")) {
    TreeDump dump(config->get_ast_dump_file());
    dump.DumpTree(AST);
  }

  QueryAssembler query_assembler(config->get_cypher_file());
  query_assembler.Translate(AST);

  end(EXIT_SUCCESS);

  return 0;
}
