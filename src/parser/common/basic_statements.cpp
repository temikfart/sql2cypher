#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;
using namespace ast::common;
using namespace parser::common;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> Parser::GetDataType() {
  NodePtr<INode> node = NextToken();

  try {
    std::string datatype = ASTUtils::CastToNodeType<StringNode>(node)->data;
    StmtType SQL_datatype(datatype);
    node->stmt_type = SQL_datatype;
  } catch (const std::invalid_argument& ia) {
    LOGE << ia.what();
    LOGE << "invalid column datatype in line " << node->line;
    end(EXIT_FAILURE);
  }

  return node;
}
NodePtr<INode> Parser::GetPrimaryKey() {
  NodePtr<INode> primary_key = ASTUtils::CreateServiceNode(StmtType::kPrimaryKey);

  int line = PeekToken()->line;
  ValidateIsOpeningRoundBracket(PeekToken());
  NextToken();

  // Get PRIMARY KEY definition
  if (tokens_.empty()) {
    LOGE << "column name is missed in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> column_name = GetIdentifier();
  ASTUtils::Link(primary_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty()) {
    if (NodeDataClassifier::IsComma(PeekToken())) {
      NodePtr<INode> separator = GetListOf(StmtType::kIdentifier);
      ASTUtils::Link(primary_key, separator);
    }
  }

  if (tokens_.empty()) {
    LOGE << "closing round bracket is missed in line " << line;
    end(EXIT_FAILURE);
  }
  ValidateIsClosingRoundBracket(PeekToken());
  NextToken();

  return primary_key;
}

NodePtr<INode> Parser::GetForeignKey() {
  NodePtr<INode> foreign_key = ASTUtils::CreateServiceNode(StmtType::kForeignKey);

  int line = PeekToken()->line;
  ValidateIsOpeningRoundBracket(PeekToken());
  NextToken();

  // Get FOREIGN KEY definition
  if (tokens_.empty()) {
    LOGE << "columnName is missed in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> column_name = GetIdentifier();
  ASTUtils::Link(foreign_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty() && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> separator = GetListOf(StmtType::kIdentifier);
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
  NodePtr<INode> ref_table_name = GetName();
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
    ref_column_name = GetIdentifier();
    ASTUtils::Link(reference, ref_column_name);

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
           << line << ": closing round bracket is missed";
      end(EXIT_FAILURE);
    }
    if (NodeDataClassifier::IsComma(PeekToken())) {
      next_ref_column_names =
          GetListOf(StmtType::kIdentifier);
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

NodePtr<INode> Parser::GetName() {
  NodePtr<INode> name = ASTUtils::CreateServiceNode(StmtType::kName);

  NodePtr<INode> identifier = GetIdentifier();
  ASTUtils::Link(name, identifier);

  if (!tokens_.empty()) {
    if (NodeDataClassifier::IsDot(PeekToken())) {
      NodePtr<INode> next_identifiers = GetIdentifiers();
      ASTUtils::Link(name, next_identifiers);
    }
  }

  return name;
}
NodePtr<INode> Parser::GetIdentifiers() {
  int line = PeekToken()->line;
  NextToken();
  NodePtr<INode> dot = ASTUtils::CreateServiceNode(StmtType::kDotDelimiter);

  if (tokens_.empty()) {
    LOGE << "bad name, which ends in a dot, in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> identifier = GetIdentifier();
  ASTUtils::Link(dot, identifier);

  if (!tokens_.empty()) {
    if (NodeDataClassifier::IsDot(PeekToken())) {
      NodePtr<INode> next_identifiers = GetIdentifiers();
      ASTUtils::Link(dot, next_identifiers);
    }
  }

  return dot;
}

NodePtr<INode> Parser::GetIdentifier() {
  ValidateIsWord(PeekToken());
  NodePtr<INode> identifier = ASTUtils::CreateServiceNode(StmtType::kIdentifier);

  NodePtr<INode> argument = NextToken();
  ASTUtils::Link(identifier, argument);

  return identifier;
}

NodePtr<INode> Parser::GetListOf(StmtType get_function_type) {
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
      argument = GetIdentifier();
      break;
    case StmtType::kTableDef:
      ValidateIsWord(PeekToken());
      argument = GetTableDefinitionObject();
      break;
    case StmtType::kColumnDef:
      ValidateIsWord(PeekToken());
      argument = GetColumnDefinition();
      break;
    case StmtType::kTableConstraint:
      ValidateIsWord(PeekToken());
      argument = GetTableConstraint();
      break;
    case StmtType::kName:
      ValidateIsWord(PeekToken());
      argument = GetName();
      break;
    default:
      LOGE << "unknown statement type for the listOf in line "
           << PeekToken()->line;
      end(EXIT_FAILURE);
  }
  ASTUtils::Link(separator, argument);

  if (!tokens_.empty()
      && NodeDataClassifier::IsComma(PeekToken())) {
    NodePtr<INode> next_separator = GetListOf(get_function_type);
    ASTUtils::Link(separator, next_separator);
  }

  return separator;
}

} // scc::parser
