#include "SCC/parser/parser.h"

namespace scc::parser {

using namespace ast;

template<typename NodeType,
    typename std::enable_if<std::is_base_of<INode, NodeType>::value>::type* = nullptr>
using NodePtr = std::shared_ptr<NodeType>;

NodePtr<INode> Parser::GetDataType() {
  NodePtr<INode> node = get_first_token();

  std::string datatype = CastToNodeType<StringNode>(node)->data;
  StmtType SQL_datatype = StmtType::kNone;  // invalid value
  if (datatype == "int" || datatype == "integer") {
    SQL_datatype = StmtType::kIntType;
  } else if (datatype == "float") {
    SQL_datatype = StmtType::kFloatType;
  } else if (datatype == "char") {
    SQL_datatype = StmtType::kCharType;
  } else if (datatype == "varchar") {
    SQL_datatype = StmtType::kVarcharType;
  } else {
    LOGE << "invalid column datatype in line " << node->line;
    end(EXIT_FAILURE);
  }
  node->stmt_type = SQL_datatype;

  return node;
}
NodePtr<INode> Parser::GetPrimaryKey() {
  NodePtr<INode> primary_key = CreateServiceNode(StmtType::kPrimaryKey);

  int line = peek_first_token()->line;
  ValidateIsOpeningRoundBracket(peek_first_token());
  pop_first_token();

  // Get PRIMARY KEY definition
  if (tokens_.empty()) {
    LOGE << "column name is missed in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> column_name = GetIdentifier();
  INode::Link(primary_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty()) {
    if (Parser::IsComma(peek_first_token())) {
      NodePtr<INode> separator = GetListOf(StmtType::kIdentifier);
      INode::Link(primary_key, separator);
    }
  }

  if (tokens_.empty()) {
    LOGE << "closing round bracket is missed in line " << line;
    end(EXIT_FAILURE);
  }
  ValidateIsClosingRoundBracket(peek_first_token());
  pop_first_token();

  return primary_key;
}

NodePtr<INode> Parser::GetForeignKey() {
  NodePtr<INode> foreign_key = CreateServiceNode(StmtType::kForeignKey);

  int line = peek_first_token()->line;
  ValidateIsOpeningRoundBracket(peek_first_token());
  pop_first_token();

  // Get FOREIGN KEY definition
  if (tokens_.empty()) {
    LOGE << "columnName is missed in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> column_name = GetIdentifier();
  INode::Link(foreign_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty()
      && Parser::IsComma(peek_first_token())) {
    NodePtr<INode> separator = GetListOf(StmtType::kIdentifier);
    INode::Link(foreign_key, separator);
  }

  if (tokens_.empty()) {
    LOGE << "closing round bracket is missed in line " << line;
    end(EXIT_FAILURE);
  }
  ValidateIsClosingRoundBracket(peek_first_token());
  pop_first_token();

  // Get Reference
  if (tokens_.empty()) {
    LOGE << "reference is missed in line " << line;
    end(EXIT_FAILURE);
  }
  ValidateIsWord(peek_first_token());
  NodePtr<INode> reference = GetReference();
  INode::Link(foreign_key, reference);

  return foreign_key;
}
NodePtr<INode> Parser::GetReference() {
  // Get REFERENCES
  int line = peek_first_token()->line;
  std::string ref_kw = CastToNodeType<StringNode>(peek_first_token())->data;
  if (ref_kw != "REFERENCES") {
    LOGE << "incorrect reference key word in line "
         << line << ": " << ref_kw;
    end(EXIT_FAILURE);
  }
  pop_first_token();
  NodePtr<INode> reference = CreateServiceNode(StmtType::kReference);

  // Get referenced table or columns
  if (tokens_.empty()) {
    LOGE << "table name is missed in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> ref_table_name = GetName();
  INode::Link(reference, ref_table_name);

  // Get columns if present
  if (!tokens_.empty()
      && Parser::IsOpeningRoundBracket(peek_first_token())) {
    NodePtr<INode> ref_column_name, next_ref_column_names;

    ValidateIsOpeningRoundBracket(peek_first_token());
    pop_first_token();

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
           << line << ": incorrect list of column names";
      end(EXIT_FAILURE);
    }
    ref_column_name = GetIdentifier();
    INode::Link(reference, ref_column_name);

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
           << line << ": closing round bracket is missed";
      end(EXIT_FAILURE);
    }
    if (Parser::IsComma(peek_first_token())) {
      next_ref_column_names =
          GetListOf(StmtType::kIdentifier);
      INode::Link(reference, next_ref_column_names);
    }

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
           << line << ": closing round bracket is missed";
      end(EXIT_FAILURE);
    }
    ValidateIsClosingRoundBracket(peek_first_token());
    pop_first_token();
  }

  return reference;
}

NodePtr<INode> Parser::GetString() {
  NodePtr<INode> node;

  int line = peek_first_token()->line;
  bool is_single_quote =
      Parser::IsSingleQuote(peek_first_token());
  pop_first_token();

  if (tokens_.empty()) {
    LOGE << "invalid expression in line "
         << line << ": invalid string";
    end(EXIT_FAILURE);
  }
  NodePtr<StringNode> str =
      std::make_shared<StringNode>("", DataType::kString);
  while (!Parser::IsSingleQuote(peek_first_token())
      || !Parser::IsDoubleQuote(peek_first_token())) {
    NodePtr<INode> tmp = get_first_token();
    line = tmp->line;
    DataType tmp_type = tmp->data_type;
    std::string new_data;
    if (!str->data.empty()) {
      new_data = str->data + " ";
    }
    switch (tmp_type) {
      case DataType::kInt:
        new_data += std::to_string(CastToNodeType<IntNumNode>(tmp)->data);
        break;
      case DataType::kFloat:
        new_data += std::to_string(CastToNodeType<FloatNumNode>(tmp)->data);
        break;
      case DataType::kWord:
      case DataType::kOperator:
        new_data += CastToNodeType<StringNode>(tmp)->data;
        break;
      case DataType::kBracket:
      case DataType::kPunctuation:
        new_data += std::to_string(CastToNodeType<CharNode>(tmp)->data);
        break;
      default:
        LOGE << "invalid string in line "
             << line << ": incorrect type of token inside the string";
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
    ValidateIsSingleQuote(peek_first_token());
  } else {
    ValidateIsDoubleQuote(peek_first_token());
  }
  pop_first_token();

  node = CastToNodeType<INode>(str);

  return node;
}

NodePtr<INode> Parser::GetName() {
  NodePtr<INode> name = CreateServiceNode(StmtType::kName);

  NodePtr<INode> identifier = GetIdentifier();
  INode::Link(name, identifier);

  if (!tokens_.empty()) {
    if (Parser::IsDot(peek_first_token())) {
      NodePtr<INode> next_identifiers = GetIdentifiers();
      INode::Link(name, next_identifiers);
    }
  }

  return name;
}
NodePtr<INode> Parser::GetIdentifiers() {
  int line = peek_first_token()->line;
  pop_first_token();
  NodePtr<INode> dot = CreateServiceNode(StmtType::kDotDelimiter);

  if (tokens_.empty()) {
    LOGE << "bad name, which ends in a dot, in line " << line;
    end(EXIT_FAILURE);
  }
  NodePtr<INode> identifier = GetIdentifier();
  INode::Link(dot, identifier);

  if (!tokens_.empty()) {
    if (Parser::IsDot(peek_first_token())) {
      NodePtr<INode> next_identifiers = GetIdentifiers();
      INode::Link(dot, next_identifiers);
    }
  }

  return dot;
}

NodePtr<INode> Parser::GetIdentifier() {
  ValidateIsWord(peek_first_token());
  NodePtr<INode> identifier = CreateServiceNode(StmtType::kIdentifier);

  NodePtr<INode> argument = get_first_token();
  INode::Link(identifier, argument);

  return identifier;
}

NodePtr<INode> Parser::GetListOf(StmtType get_function_type) {
  // Get separator (comma)
  int line = peek_first_token()->line;
  pop_first_token();
  NodePtr<INode> separator = CreateServiceNode(StmtType::kCommaDelimiter);

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
      ValidateIsWord(peek_first_token());
      argument = GetTableDefinitionObject();
      break;
    case StmtType::kColumnDef:
      ValidateIsWord(peek_first_token());
      argument = GetColumnDefinition();
      break;
    case StmtType::kTableConstraint:
      ValidateIsWord(peek_first_token());
      argument = GetTableConstraint();
      break;
    case StmtType::kName:
      ValidateIsWord(peek_first_token());
      argument = GetName();
      break;
    default:
      LOGE << "unknown statement type for the listOf in line "
           << peek_first_token()->line;
      end(EXIT_FAILURE);
  }
  INode::Link(separator, argument);

  if (!tokens_.empty()
      && Parser::IsComma(peek_first_token())) {
    NodePtr<INode> next_separator = GetListOf(get_function_type);
    INode::Link(separator, next_separator);
  }

  return separator;
}

} // scc::parser
