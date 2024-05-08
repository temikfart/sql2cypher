#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

StmtType Parser::DetermineConstraintType(const std::string& keyword) const {
  if (DetermineIsPrimaryKey(keyword)) {
    return StmtType::kPrimaryKey;
  } else if (DetermineIsForeignKey(keyword)) {
    return StmtType::kForeignKey;
  }
  return StmtType::kNone;
}
StmtType Parser::DetermineAlterTableActionType(const std::string& keyword) const {
  std::string keyword_lower = scc::common::LowerCase(keyword);
  if (kST_ADD_KW.find(keyword_lower) == 0) {
    return StmtType::kAddKW;
  } else if (kST_DROP_KW.find(keyword_lower) == 0) {
    return StmtType::kDropKW;
  }
  return StmtType::kNone;
}
StmtType Parser::DetermineDropElementType(const std::string& keyword) const {
  std::string keyword_lower = scc::common::LowerCase(keyword);
  if (kST_CONSTRAINT_KW.find(keyword_lower) == 0) {
    return StmtType::kDropConstraint;
  } else if (kST_COLUMN_KW.find(keyword_lower) == 0) {
    return StmtType::kDropColumn;
  }
  return StmtType::kNone;
}
StmtType Parser::DetermineLogicalOperatorType(const std::string& keyword) const {
  if (DetermineIsOROperator(keyword)) {
    return StmtType::kOROperator;
  } else if (DetermineIsANDOperator(keyword)) {
    return StmtType::kANDOperator;
  } else if (DetermineIsNOTOperator(keyword)) {
    return StmtType::kNOTOperator;
  }
  return StmtType::kNone;
}

bool Parser::DetermineIsFullConstraintDefinition(const std::string& keyword) const {
  return DetermineIsPrefix(kST_CONSTRAINT_KW, keyword);
}
bool Parser::DetermineIsPrimaryKey(const std::string& keyword) const {
  return DetermineIsPrefix(kST_PRIMARY_KEY, keyword);
}
bool Parser::DetermineIsForeignKey(const std::string& keyword) const {
  return DetermineIsPrefix(kST_FOREIGN_KEY, keyword);
}
bool Parser::DetermineIsReferences(const std::string& keyword) const {
  return DetermineIsPrefix(kST_REFERENCES_KW, keyword);
}
bool Parser::DetermineIsOROperator(const std::string& keyword) const {
  return DetermineIsPrefix(kST_OR_OPERATOR, keyword);
}
bool Parser::DetermineIsANDOperator(const std::string& keyword) const {
  return DetermineIsPrefix(kST_AND_OPERATOR, keyword);
}
bool Parser::DetermineIsNOTOperator(const std::string& keyword) const {
  return DetermineIsPrefix(kST_NOT_OPERATOR, keyword);
}
bool Parser::DetermineIsPrefix(const std::string_view& string, const std::string& prefix) const {
  return string.find(scc::common::LowerCase(prefix)) == 0;
}

NodePtr<INode> Parser::ParseListOf(StmtType element_type) {
  auto next_token = NextToken();
  NodePtr<INode> separator = ASTUtils::CreateServiceNode(StmtType::kCommaDelimiter, next_token);

  ValidateHasTokens("Invalid statement at line " + std::to_string(next_token->line)
                        + ": expected something after the comma");
  NodePtr<INode> next_element;
  switch (element_type) {
    case StmtType::kIdentifier:
      next_element = ParseIdentifier();
      break;
    case StmtType::kTableDef:
      next_element = ParseTableDefinitionElement();
      break;
    case StmtType::kColumnDef:
      next_element = ParseColumnDefinition();
      break;
    case StmtType::kTableConstraint:
      next_element = ParseTableConstraint();
      break;
    case StmtType::kName:
      next_element = ParseName();
      break;
    default:
      throw parsing_error("Invalid statatement at line " + std::to_string(PeekToken()->line)
                              + ": unsupported elements in the list \'" + element_type.ToString()
                              + "\'");
  }
  ASTUtils::Link(separator, next_element);

  if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> next_elements = ParseListOf(element_type);
    ASTUtils::Link(separator, next_elements);
  }

  return separator;
}
NodePtr<INode> Parser::ParsePrimaryKey() {
  NodePtr<INode> primary_key = ASTUtils::CreateServiceNode(StmtType::kPrimaryKey);

  std::string incorrect_msg_prefix = "Incorrect \'PRIMARY KEY\' definition at line ";

  auto next_token = NextToken();
  ValidateIsWord(next_token);
  std::string primary_keyword = ASTUtils::CastToNodeType<StringNode>(next_token)->data;
  if (!DetermineIsPrimaryKey(primary_keyword)) {
    throw parsing_error(incorrect_msg_prefix + std::to_string(next_token->line)
                            + ": expected \'PRIMARY\' keyword");
  }

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                        + ": missing \'KEY\' keyword");
  next_token = NextToken();
  ValidateIsWord(next_token);
  std::string key_keyword = ASTUtils::CastToNodeType<StringNode>(next_token)->data;
  if (scc::common::LowerCase(key_keyword) != "key") {
    throw parsing_error(incorrect_msg_prefix + std::to_string(next_token->line)
                            + ": expected \'KEY\' keyword");
  }

  ValidateHasTokens("Missing \'PRIMARY KEY\' definition at line "
                        + std::to_string(next_token->line));
  int line = PeekToken()->line;
  ValidateIsOpeningRoundBracket(NextToken());

  ValidateHasTokens("Missed column name at line " + std::to_string(line));
  while (!tokens_.empty()) {
    NodePtr<INode> column_name = ParseColumnName();
    ASTUtils::Link(primary_key, column_name);

    if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
      next_token = NextToken();
      ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                            + ": missing column name after comma");
      ValidateIsWord(PeekToken());
    } else {
      break;
    }
  }

  ValidateHasTokens("Missed closing round bracket at line " + std::to_string(line));
  ValidateIsClosingRoundBracket(NextToken());

  return primary_key;
}
NodePtr<INode> Parser::ParseForeignKey() {
  NodePtr<INode> foreign_key = ASTUtils::CreateServiceNode(StmtType::kForeignKey);

  std::string incorrect_msg_prefix = "Incorrect \'FOREIGN KEY\' definition at line ";

  auto next_token = NextToken();
  ValidateIsWord(next_token);
  std::string foreign_keyword = ASTUtils::CastToNodeType<StringNode>(next_token)->data;
  if (!DetermineIsForeignKey(foreign_keyword)) {
    throw parsing_error(incorrect_msg_prefix + std::to_string(next_token->line)
                            + ": expected \'FOREIGN\' keyword");
  }

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                        + ": missing \'KEY\' keyword");
  next_token = NextToken();
  ValidateIsWord(next_token);
  std::string key_keyword = ASTUtils::CastToNodeType<StringNode>(next_token)->data;
  if (scc::common::LowerCase(key_keyword) != "key") {
    throw parsing_error(incorrect_msg_prefix + std::to_string(next_token->line)
                            + ": expected \'KEY\' keyword");
  }

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                        + ": missing \'FOREIGN KEY\' definition");
  next_token = NextToken();
  ValidateIsOpeningRoundBracket(next_token);

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                        + ": missing referencing column name");
  while (!tokens_.empty()) {
    NodePtr<INode> referencing_column_name = ParseColumnName();
    ASTUtils::Link(foreign_key, referencing_column_name);

    if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
      next_token = NextToken();
      ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                            + ": missing referencing column name after comma");
      ValidateIsWord(PeekToken());
    } else {
      break;
    }
  }

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                        + ": missing closing parenthesis");
  next_token = NextToken();
  ValidateIsClosingRoundBracket(next_token);

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                        + ": missing \'REFERENCES\' keyword");
  NodePtr<INode> reference = ParseReference();
  ASTUtils::Link(foreign_key, reference);

  return foreign_key;
}
NodePtr<INode> Parser::ParseReference() {
  std::string incorrect_msg_prefix = "Incorrect \'REFERENCES\' definition at line ";

  auto next_token = NextToken();
  ValidateIsWord(next_token);
  std::string references_keyword = ASTUtils::CastToNodeType<StringNode>(next_token)->data;
  if (!DetermineIsReferences(references_keyword)) {
    throw parsing_error(incorrect_msg_prefix + std::to_string(next_token->line));
  }
  NodePtr<INode> reference = ASTUtils::CreateServiceNode(StmtType::kReferencesKW, next_token);

  ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                        + ": missing references table name");
  NodePtr<INode> referenced_table_name = ParseTableName();
  ASTUtils::Link(reference, referenced_table_name);

  if (!tokens_.empty() && NodeDataClassifier::IsOpeningRoundBracket(PeekToken())) {
    next_token = NextToken();

    ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                          + ": missing referenced column name");
    while (!tokens_.empty()) {
      NodePtr<INode> referenced_column_name = ParseColumnName();
      ASTUtils::Link(reference, referenced_column_name);

      if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
        next_token = NextToken();
        ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                              + ": missing referenced column name after comma");
        ValidateIsWord(PeekToken());
      } else {
        break;
      }
    }

    ValidateHasTokens(incorrect_msg_prefix + std::to_string(next_token->line)
                          + ": missing closing parenthesis");
    ValidateIsClosingRoundBracket(NextToken());
  }

  return reference;
}

NodePtr<INode> Parser::ParseDatabaseName() {
  return ParseName();
}
NodePtr<INode> Parser::ParseTableName() {
  return ParseName();
}
NodePtr<INode> Parser::ParseColumnName() {
  return ParseName();
}

NodePtr<INode> Parser::ParseDataType() {
  auto next_token = NextToken();
  ValidateIsWord(next_token);
  std::string datatype = ASTUtils::CastToNodeType<StringNode>(next_token)->data;
  try {
    StmtType sql_datatype(datatype);
    next_token->stmt_type = sql_datatype;
  } catch (const std::invalid_argument& ia) {
    throw parsing_error("Unknown Data Type \'" + datatype
                            + "\' at line " + std::to_string(next_token->line));
  }
  return next_token;
}
NodePtr<INode> Parser::ParseString() {
  auto next_token = NextToken();
  ValidateIsSingleQuote(next_token);

  ValidateHasTokens("Missing string literal content or closing single quote at line "
                        + std::to_string(next_token->line));
  NodePtr<StringNode> string = ASTUtils::CastToNodeType<StringNode>(
      ASTUtils::CreateStringNode("", DataType::kString)
  );
  auto peeked_token = PeekToken();
  while (!NodeDataClassifier::IsSingleQuote(peeked_token)) {
    NodePtr<INode> next_word = NextToken();
    if (!string->data.empty()) {
      string->data + " "; // TODO: Lexer should save whitespace characters.
    }
    switch (next_word->data_type) {
      case DataType::kInt:
        string->data += std::to_string(ASTUtils::CastToNodeType<IntNumNode>(next_word)->data);
        break;
      case DataType::kFloat:
        string->data += std::to_string(ASTUtils::CastToNodeType<FloatNumNode>(next_word)->data);
        break;
      case DataType::kBracket:
      case DataType::kPunctuation:
        string->data += std::to_string(ASTUtils::CastToNodeType<CharNode>(next_word)->data);
        break;
      case DataType::kOperator:
      case DataType::kWord:
        string->data += ASTUtils::CastToNodeType<StringNode>(next_word)->data;
        break;
      default:
        throw parsing_error("Invalid string at line " + std::to_string(next_word->line)
                                + ": unsupported data type for string literal part");
    }
    ValidateHasTokens("Missing closing single quote or string literal part at line "
                          + std::to_string(next_word->line));
  }
  NextToken(); // Pop closing single quote

  return string;
}
NodePtr<INode> Parser::ParseName() {
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kName, PeekToken());

  while (!tokens_.empty()) {
    NodePtr<INode> identifier = ParseIdentifier();
    ASTUtils::Link(service_node, identifier);

    if (!tokens_.empty() && NodeDataClassifier::IsDot(PeekToken())) {
      auto next_token = NextToken();
      ValidateHasTokens("Missing identifier after the dot delimiter at line "
                            + std::to_string(next_token->line));
      ValidateIsWord(PeekToken());
    } else {
      break;
    }
  }

  return service_node;
}
NodePtr<INode> Parser::ParseIdentifier() {
  auto next_token = NextToken();
  ValidateIsWord(next_token);
  NodePtr<INode> service_node = ASTUtils::CreateServiceNode(StmtType::kIdentifier, next_token);
  NodePtr<INode> identifier = next_token;
  ASTUtils::Link(service_node, identifier);

  return service_node;
}

} // scc::parser
