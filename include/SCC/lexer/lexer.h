#pragma once

#include <deque>
#include <filesystem>
#include <sstream>

#include "SCC/ast/nodes/inode.h"
#include "SCC/ast/nodes/char_node.h"
#include "SCC/ast/nodes/float_node.h"
#include "SCC/ast/nodes/int_node.h"
#include "SCC/ast/nodes/root_node.h"
#include "SCC/ast/nodes/service_node.h"
#include "SCC/ast/nodes/string_node.h"
#include "SCC/config/scc_config.h"
#include "SCC/lexer/symbol_classifier.h"
#include "SCC/lexer/symbol_types.h"
#include "SCC/log.h"

#include "logger/log.hpp"

namespace scc::lexer {

class Lexer {
public:
  explicit Lexer(const std::filesystem::path& input_path);

  std::deque<std::shared_ptr<ast::INode>> Analyze();

private:
  std::ifstream input_;
  int line_ = 1;
  std::deque<std::shared_ptr<ast::INode>> tokens_;

  std::shared_ptr<ast::INode> GetToken(char symbol, SymbolType sym_type);

  char GetSymbol();
  char PeekSymbol();

  int GetDigit();
  std::shared_ptr<ast::INode> GetNumber();
  std::shared_ptr<ast::INode> GetWord();
  std::shared_ptr<ast::INode> GetOperator();
  std::shared_ptr<ast::INode> GetCharacter(ast::DataType data_type);
};

} // std::lexer
