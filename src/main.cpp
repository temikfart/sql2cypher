#include "SCC/main.h"

int main(int argc, char* argv[]) {
  try {
    scc::config::SCCArgs scc_args;
    scc::config::SCCSubcommandSchema scc_schema_subcommand;
    scc_args.add_subparser(scc_schema_subcommand);
    scc::config::SCCSubcommandData scc_data_subcommand;
    scc_args.add_subparser(scc_data_subcommand);

    scc_args.ParseArgs(argc, argv);

    scc::config::InitConfig(scc_args);
    scc::config::SCCConfig* config = scc::config::SCCConfig::Get();

    logger::init(config->log_severity, config->log_directory);

    if (config->translate_schema) {
      scc::lexer::Lexer lexer(config->get_sql_schema_file());
      std::deque<std::shared_ptr<scc::ast::INode>> tokens = lexer.Analyze();

      scc::parser::Parser parser(std::move(tokens));
      std::shared_ptr<scc::ast::INode> AST = parser.Parse();

      if (scc_args.IsUsed("--dump")) {
        scc::dump::TreeDump dump(config->get_ast_dump_file());
        dump.DumpTree(AST);
      }

      scc::translator::Translator schema_translator(AST, config->get_graph_schema_file());
      schema_translator.Translate();
    }

    if (config->translate_data) {
      scc::lexer::Lexer lexer(config->get_sql_file());
      std::deque<std::shared_ptr<scc::ast::INode>> tokens = lexer.Analyze();

      scc::parser::Parser parser(std::move(tokens));
      std::shared_ptr<scc::ast::INode> AST = parser.Parse();

      scc::translator::Translator data_translator(AST, config->get_graph_schema_file(),
                                                  config->get_cypher_file());
      data_translator.Translate();
    }

    end(EXIT_SUCCESS);
  } catch (const std::exception& e) {
    std::cerr << "Translation failed with error" << std::endl;
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Translation failed with unknown error" << std::endl;
  }

  return 0;
}
