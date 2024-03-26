#include "SCC/main.h"

int main(int argc, char* argv[]) {
  try {
    scc::SCCArgs scc_args;
    scc_args.ParseArgs(argc, argv);

    scc::InitConfig(scc_args);
    scc::SCCConfig* config = scc::SCCConfig::Get();

    logger::init(config->log_severity, config->log_directory);

    scc::lexer::Lexer lexer(config->get_sql_file());
    std::deque<std::shared_ptr<scc::ast::INode>> tokens = lexer.Analyze();

    scc::parser::SyntaxAnalyzer syntax_analyzer(std::move(tokens));
    std::shared_ptr<scc::ast::INode> AST = syntax_analyzer.Analyze();

    if (scc_args.IsUsed("--dump")) {
      scc::dump::TreeDump dump(config->get_ast_dump_file());
      dump.DumpTree(AST);
    }

    scc::query_assembler::QueryAssembler query_assembler(config->get_cypher_file());
    query_assembler.Translate(AST);

    end(EXIT_SUCCESS);
  } catch (const std::exception& e) {
    std::cerr << "Translation failed with error." << std::endl;
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
