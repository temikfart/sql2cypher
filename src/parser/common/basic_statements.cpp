#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

StmtType Parser::DetermineDropElementType(const std::string& keyword) {
  if (kST_CONSTRAINT_KW.find(keyword) == 0) {
    return StmtType::kConstraintKW;
  } else if (kST_COLUMN_KW.find(keyword) == 0) {
    return StmtType::kColumnKW;
  }
  return StmtType::kNone;
}
StmtType Parser::DetermineAlterTableActionType(const std::string& keyword) {
  if (kST_ADD_KW.find(scc::common::LowerCase(keyword)) == 0) {
    return StmtType::kAddKW;
  } else if (kST_DROP_KW.find(scc::common::LowerCase(keyword)) == 0) {
    return StmtType::kDropKW;
  }
  return StmtType::kNone;
}

StmtType Parser::DetermineConstraintType(const std::string& keyword) {
  if (DetermineIsPrimaryKey(keyword)) {
    return StmtType::kPrimaryKey;
  } else if (DetermineIsForeignKey(keyword)) {
    return StmtType::kForeignKey;
  }
  return StmtType::kNone;
}
bool Parser::DetermineIsFullConstraintDefinition(const std::string& keyword) {
  return kST_CONSTRAINT_KW.find(scc::common::LowerCase(keyword)) == 0;
}
NodePtr<INode> Parser::ParseDataType() {
  NodePtr<INode> node = NextToken();
  std::string datatype = ASTUtils::CastToNodeType<StringNode>(node)->data;
  try {
    StmtType sql_datatype(datatype);
    node->stmt_type = sql_datatype;
  } catch (const std::invalid_argument& ia) {
    throw parsing_error("Unknown Data Type \'" + datatype
                            + "\' at line " + std::to_string(node->line));
  }
  return node;
}
bool Parser::DetermineIsPrimaryKey(const std::string& keyword) {
  return kST_PRIMARY_KEY.find(scc::common::LowerCase(keyword)) == 0;
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
  ValidateIsOpeningRoundBracket(PeekToken());
  NextToken();

  ValidateHasTokens("Missed column name at line " + std::to_string(line));
  NodePtr<INode> column_name = ParseIdentifier();
  ASTUtils::Link(primary_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> separator = ParseListOf(StmtType::kIdentifier);
    ASTUtils::Link(primary_key, separator);
  }

  ValidateHasTokens("Missed closing round bracket at line " + std::to_string(line));
  ValidateIsClosingRoundBracket(PeekToken());
  NextToken();

  return primary_key;
}

bool Parser::DetermineIsForeignKey(const std::string& keyword) {
  return kST_FOREIGN_KEY.find(scc::common::LowerCase(keyword)) == 0;
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

  ValidateHasTokens("Missing \'FOREIGN KEY\' definition at line "
                        + std::to_string(next_token->line));
  int line = PeekToken()->line;
  ValidateIsOpeningRoundBracket(PeekToken());
  NextToken();

  // Get FOREIGN KEY definition
  if (tokens_.empty()) {
    LOGE << "columnName is missed in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> column_name = ParseIdentifier();
  ASTUtils::Link(foreign_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> separator = ParseListOf(StmtType::kIdentifier);
    ASTUtils::Link(foreign_key, separator);
  }

  if (tokens_.empty()) {
    LOGE << "closing round bracket is missed in line " << line;
    end(EXIT_FAILURE);
  }
  ValidateIsClosingRoundBracket(PeekToken());
  NextToken();

  // Get Reference
  if (tokens_.empty()) {
    LOGE << "reference is missed in line " << line;
    end(EXIT_FAILURE);
  }
  ValidateIsWord(PeekToken());
  NodePtr<INode> reference = GetReference();
  ASTUtils::Link(foreign_key, reference);

  return foreign_key;
}
NodePtr<INode> Parser::GetReference() {
  // Get REFERENCES
  int line = PeekToken()->line;
  std::string ref_kw = ASTUtils::CastToNodeType<StringNode>(PeekToken())->data;
  if (ref_kw != "REFERENCES") {
    LOGE << "incorrect reference key word in line "
         << line << ": " << ref_kw;
    end(EXIT_FAILURE);
  }
  NextToken();
  NodePtr<INode> reference = ASTUtils::CreateServiceNode(StmtType::kReference);

  // Get referenced table or columns
  if (tokens_.empty()) {
    LOGE << "table name is missed in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> ref_table_name = ParseName();
  ASTUtils::Link(reference, ref_table_name);

  // Get columns if present
  if (!tokens_.empty() && NodeDataClassifier::IsOpeningRoundBracket(PeekToken())) {
    NodePtr<INode> ref_column_name, next_ref_column_names;

    ValidateIsOpeningRoundBracket(PeekToken());
    NextToken();

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
           << line << ": incorrect list of column names";
      end(EXIT_FAILURE);
    }
    ref_column_name = ParseIdentifier();
    ASTUtils::Link(reference, ref_column_name);

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
           << line << ": closing round bracket is missed";
      end(EXIT_FAILURE);
    }
    if (NodeDataClassifier::IsComma(PeekToken())) {
      next_ref_column_names =
          ParseListOf(StmtType::kIdentifier);
      ASTUtils::Link(reference, next_ref_column_names);
    }

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
           << line << ": closing round bracket is missed";
      end(EXIT_FAILURE);
    }
    ValidateIsClosingRoundBracket(PeekToken());
    NextToken();
  }

  return reference;
}

NodePtr<INode> Parser::GetString() {
  NodePtr<INode> node;

  int line = PeekToken()->line;
  bool is_single_quote = NodeDataClassifier::IsSingleQuote(PeekToken());
  NextToken();

  if (tokens_.empty()) {
    LOGE << "invalid expression in line "
         << line << ": invalid string";
    end(EXIT_FAILURE);
  }
  NodePtr<StringNode> str =
      std::make_shared<StringNode>("", DataType::kString);
  while (!NodeDataClassifier::IsQuote(PeekToken())) {
    NodePtr<INode> tmp = NextToken();
    std::string new_data;
    if (!str->data.empty()) {
      new_data = str->data + " ";
    }
    switch (tmp->data_type) {
      case DataType::kInt:
        new_data += std::to_string(ASTUtils::CastToNodeType<IntNumNode>(tmp)->data);
        break;
      case DataType::kFloat:
        new_data += std::to_string(ASTUtils::CastToNodeType<FloatNumNode>(tmp)->data);
        break;
      case DataType::kWord:
      case DataType::kOperator:
        new_data += ASTUtils::CastToNodeType<StringNode>(tmp)->data;
        break;
      case DataType::kBracket:
      case DataType::kPunctuation:
        new_data += std::to_string(ASTUtils::CastToNodeType<CharNode>(tmp)->data);
        break;
      default:
        LOGE << "invalid string in line "
             << tmp->line << ": incorrect type of token inside the string";
        end(EXIT_FAILURE);
    }
    str->data = new_data;
  }

  if (tokens_.empty()) {
    LOGE << "invalid string in line "
         << line << ": closing quote is missed";
    end(EXIT_FAILURE);
  }
  if (is_single_quote) {
    ValidateIsSingleQuote(PeekToken());
  } else {
    ValidateIsDoubleQuote(PeekToken());
  }
  NextToken();

  node = ASTUtils::CastToNodeType<INode>(str);

  return node;
}

NodePtr<INode> Parser::ParseName() {
  NodePtr<INode> name = ASTUtils::CreateServiceNode(StmtType::kName);

  NodePtr<INode> identifier = ParseIdentifier();
  ASTUtils::Link(name, identifier);

  if (!tokens_.empty()) {
    if (NodeDataClassifier::IsDot(PeekToken())) {
      NodePtr<INode> next_identifiers = ParseIdentifiers();
      ASTUtils::Link(name, next_identifiers);
    }
  }

  return name;
}
NodePtr<INode> Parser::ParseIdentifiers() {
  int line = PeekToken()->line;
  NextToken();
  NodePtr<INode> dot = ASTUtils::CreateServiceNode(StmtType::kDotDelimiter);

  if (tokens_.empty()) {
    LOGE << "bad name, which ends in a dot, in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> identifier = ParseIdentifier();
  ASTUtils::Link(dot, identifier);

  if (!tokens_.empty()) {
    if (NodeDataClassifier::IsDot(PeekToken())) {
      NodePtr<INode> next_identifiers = ParseIdentifiers();
      ASTUtils::Link(dot, next_identifiers);
    }
  }

  return dot;
}

NodePtr<INode> Parser::ParseIdentifier() {
  ValidateIsWord(PeekToken());
  NodePtr<INode> identifier = ASTUtils::CreateServiceNode(StmtType::kIdentifier);

  NodePtr<INode> argument = NextToken();
  ASTUtils::Link(identifier, argument);

  return identifier;
}

NodePtr<INode> Parser::ParseListOf(StmtType get_function_type) {
  // Get separator (comma)
  int line = PeekToken()->line;
  NextToken();
  NodePtr<INode> separator = ASTUtils::CreateServiceNode(StmtType::kCommaDelimiter);

  if (tokens_.empty()) {
    LOGE << "invalid listOf in line "
         << line << ": argument is missed";
    end(EXIT_FAILURE);
  }

  NodePtr<INode> argument;
  switch (get_function_type) {
    case StmtType::kIdentifier:
      argument = ParseIdentifier();
      break;
    case StmtType::kTableDef:
      ValidateIsWord(PeekToken());
      argument = ParseTableDefinitionElement();
      break;
    case StmtType::kColumnDef:
      ValidateIsWord(PeekToken());
      argument = ParseColumnDefinition();
      break;
    case StmtType::kTableConstraint:
      ValidateIsWord(PeekToken());
      argument = ParseTableConstraint();
      break;
    case StmtType::kName:
      ValidateIsWord(PeekToken());
      argument = ParseName();
      break;
    default:
      LOGE << "unknown statement type for the listOf in line "
           << PeekToken()->line;
      end(EXIT_FAILURE);
  }
  ASTUtils::Link(separator, argument);

  if (!tokens_.empty()
      && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> next_separator = ParseListOf(get_function_type);
    ASTUtils::Link(separator, next_separator);
  }

  return separator;
}

} // scc::parser
