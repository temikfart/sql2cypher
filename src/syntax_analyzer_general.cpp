#include "SCC/syntax_analyzer.h"

namespace scc::parser {

using namespace ast;

SyntaxAnalyzer::SyntaxAnalyzer(std::deque<std::shared_ptr<INode>>&& tokens)
    : tokens_(std::move(tokens)) {}

std::shared_ptr<INode> SyntaxAnalyzer::Analyze() {
  LOGI << "starting syntax analysis...";

  std::shared_ptr<INode> root;
  root = std::dynamic_pointer_cast<INode>(std::make_shared<RootNode>());
  root->stmt_type = StmtType::kProgram;

  if (tokens_.empty()) {
    LOGI << "syntax analysis is ended: empty tokens' array";
    return nullptr;
  }

  std::shared_ptr<INode> query, separator;

  query = this->GetDL();
  SyntaxAnalyzer::MakeKinship(root, query);

  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsSemicolon(this->peek_first_token())) {
      separator = this->General();
      SyntaxAnalyzer::MakeKinship(root, separator);
    }
  }

  if (!tokens_.empty()) {
    LOGE << "syntax analysis is ended with not empty tokens' array";
    end(EXIT_FAILURE);
  }

  LOGI << "syntax analysis is ended";
  return root;
}

// Start

std::shared_ptr<INode> SyntaxAnalyzer::General() {
  std::shared_ptr<INode> separator, query, next_queries;
  this->pop_first_token();
  separator = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  separator->stmt_type = StmtType::kSemicolonDelimiter;

  if (!tokens_.empty()) {
    query = this->GetDL();
    SyntaxAnalyzer::MakeKinship(separator, query);
  }

  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsSemicolon(this->peek_first_token())) {
      next_queries = this->General();
      SyntaxAnalyzer::MakeKinship(separator, next_queries);
    }
  }

  return separator;
}
StmtType SyntaxAnalyzer::GetDLStType() {
  StmtType DLStType = StmtType::kNone;   // invalid value

  std::string key_word =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->data;

  std::vector<std::string> ddlSt_kws = {
      "CREATE", "ALTER", "DROP"
  };
  std::vector<std::string> dmlSt_kws = {
      "UPDATE", "DELETE", "INSERT"
  };

  bool is_ddlSt = std::any_of(ddlSt_kws.begin(),
                              ddlSt_kws.end(),
                              [key_word](std::string& st) {
                                return (key_word == st);
                              });
  bool is_dmlSt = std::any_of(dmlSt_kws.begin(),
                              dmlSt_kws.end(),
                              [key_word](std::string& st) {
                                return (key_word == st);
                              });

  if (is_ddlSt) {
    DLStType = StmtType::kDdlStmt;
  } else if (is_dmlSt) {
    DLStType = StmtType::kDmlStmt;
  }

  return DLStType;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetDL() {
  std::shared_ptr<INode> query, statement;
  query = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  query->stmt_type = StmtType::kQuery;

  this->ValidateIsWord(this->peek_first_token());

  switch (this->GetDLStType()) {
    case StmtType::kDdlStmt:
      statement = this->GetDDLSt();
      break;
    case StmtType::kDmlStmt:
      statement = this->GetDMLSt();
      break;
    default:
      LOGE << "unknown DL on line "
          << this->peek_first_token()->line;
      end(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(query, statement);

  return query;
}

// Basic statements

std::shared_ptr<INode> SyntaxAnalyzer::GetDataType() {
  std::shared_ptr<INode> node = this->get_first_token();

  std::string datatype =
      std::dynamic_pointer_cast<StringNode>(node)->data;
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
std::shared_ptr<INode> SyntaxAnalyzer::GetPrimaryKey() {
  std::shared_ptr<INode> primary_key, column_name, separator;
  primary_key =
      std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  primary_key->stmt_type = StmtType::kPrimaryKey;

  int line = this->peek_first_token()->line;
  this->ValidateIsOpeningRoundBracket(this->peek_first_token());
  this->pop_first_token();

  // Get PRIMARY KEY definition
  if (tokens_.empty()) {
    LOGE << "column name is missed in line " << line;
    end(EXIT_FAILURE);
  }
  column_name = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(primary_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      separator = this->GetListOf(StmtType::kIdentifier);
      SyntaxAnalyzer::MakeKinship(primary_key, separator);
    }
  }

  if (tokens_.empty()) {
    LOGE << "closing round bracket is missed in line " << line;
    end(EXIT_FAILURE);
  }
  this->ValidateIsClosingRoundBracket(this->peek_first_token());
  this->pop_first_token();

  return primary_key;
}

std::shared_ptr<INode> SyntaxAnalyzer::GetForeignKey() {
  std::shared_ptr<INode> foreign_key, column_name, separator, reference;
  foreign_key =
      std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  foreign_key->stmt_type = StmtType::kForeignKey;

  int line = this->peek_first_token()->line;
  this->ValidateIsOpeningRoundBracket(this->peek_first_token());
  this->pop_first_token();

  // Get FOREIGN KEY definition
  if (tokens_.empty()) {
    LOGE << "columnName is missed in line " << line;
    end(EXIT_FAILURE);
  }
  column_name = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(foreign_key, column_name);

  // Get listOf(column_names)
  if (!tokens_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    separator = this->GetListOf(StmtType::kIdentifier);
    SyntaxAnalyzer::MakeKinship(foreign_key, separator);
  }

  if (tokens_.empty()) {
    LOGE << "closing round bracket is missed in line " << line;
    end(EXIT_FAILURE);
  }
  this->ValidateIsClosingRoundBracket(this->peek_first_token());
  this->pop_first_token();

  // Get Reference
  if (tokens_.empty()) {
    LOGE << "reference is missed in line " << line;
    end(EXIT_FAILURE);
  }
  this->ValidateIsWord(this->peek_first_token());
  reference = this->GetReference();
  SyntaxAnalyzer::MakeKinship(foreign_key, reference);

  return foreign_key;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetReference() {
  std::shared_ptr<INode> reference, ref_table_name;

  // Get REFERENCES
  int line = this->peek_first_token()->line;
  std::string ref_kw =
      std::dynamic_pointer_cast<StringNode>(
          this->peek_first_token())->data;
  if (ref_kw != "REFERENCES") {
    LOGE << "incorrect reference key word in line "
        << line << ": " << ref_kw;
    end(EXIT_FAILURE);
  }
  this->pop_first_token();
  reference = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  reference->stmt_type = StmtType::kReference;

  // Get referenced table or columns
  if (tokens_.empty()) {
    LOGE << "table name is missed in line " << line;
    end(EXIT_FAILURE);
  }
  ref_table_name = this->GetName();
  SyntaxAnalyzer::MakeKinship(reference, ref_table_name);

  // Get columns if present
  if (!tokens_.empty()
      && SyntaxAnalyzer::IsOpeningRoundBracket(this->peek_first_token())) {
    std::shared_ptr<INode> ref_column_name, next_ref_column_names;

    this->ValidateIsOpeningRoundBracket(this->peek_first_token());
    this->pop_first_token();

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
          << line << ": incorrect list of column names";
      end(EXIT_FAILURE);
    }
    ref_column_name = this->GetIdentifier();
    SyntaxAnalyzer::MakeKinship(reference, ref_column_name);

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
          << line << ": closing round bracket is missed";
      end(EXIT_FAILURE);
    }
    if (SyntaxAnalyzer::IsComma(this->peek_first_token())) {
      next_ref_column_names =
          this->GetListOf(StmtType::kIdentifier);
      SyntaxAnalyzer::MakeKinship(reference, next_ref_column_names);
    }

    if (tokens_.empty()) {
      LOGE << "invalid reference in line "
          << line << ": closing round bracket is missed";
      end(EXIT_FAILURE);
    }
    this->ValidateIsClosingRoundBracket(this->peek_first_token());
    this->pop_first_token();
  }

  return reference;
}

std::shared_ptr<INode> SyntaxAnalyzer::GetString() {
  std::shared_ptr<INode> node;

  int line = this->peek_first_token()->line;
  bool is_single_quote =
      SyntaxAnalyzer::IsSingleQuote(this->peek_first_token());
  this->pop_first_token();

  if (tokens_.empty()) {
    LOGE << "invalid expression in line "
        << line << ": invalid string";
    end(EXIT_FAILURE);
  }
  std::shared_ptr<StringNode> str =
      std::make_shared<StringNode>("", DataType::kString);
  while (!SyntaxAnalyzer::IsSingleQuote(this->peek_first_token())
      || !SyntaxAnalyzer::IsDoubleQuote(this->peek_first_token())) {
    std::shared_ptr<INode> tmp = this->get_first_token();
    line = tmp->line;
    DataType tmp_type = tmp->data_type;
    std::string new_data;
    if (!str->data.empty()) {
      new_data = str->data + " ";
    }
    switch (tmp_type) {
      case DataType::kInt:
        new_data += std::to_string(
            std::dynamic_pointer_cast<IntNumNode>(tmp)->data);
        break;
      case DataType::kFloat:
        new_data += std::to_string(
            std::dynamic_pointer_cast<FloatNumNode>(tmp)->data);
        break;
      case DataType::kWord:
      case DataType::kOperator:
        new_data += std::dynamic_pointer_cast<StringNode>(tmp)->data;
        break;
      case DataType::kBracket:
      case DataType::kPunctuation:
        new_data += std::to_string(
            std::dynamic_pointer_cast<CharNode>(tmp)->data);
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
    this->ValidateIsSingleQuote(this->peek_first_token());
  } else {
    this->ValidateIsDoubleQuote(this->peek_first_token());
  }
  this->pop_first_token();

  node = std::dynamic_pointer_cast<INode>(str);

  return node;
}

std::shared_ptr<INode> SyntaxAnalyzer::GetName() {
  std::shared_ptr<INode> name, identifier, next_identifiers;
  name = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  name->stmt_type = StmtType::kName;

  identifier = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(name, identifier);

  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
      next_identifiers = this->GetIdentifiers();
      SyntaxAnalyzer::MakeKinship(name, next_identifiers);
    }
  }

  return name;
}
std::shared_ptr<INode> SyntaxAnalyzer::GetIdentifiers() {
  std::shared_ptr<INode> dot, identifier, next_identifiers;
  int line = this->peek_first_token()->line;
  this->pop_first_token();
  dot = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  dot->stmt_type = StmtType::kDotDelimiter;

  if (tokens_.empty()) {
    LOGE << "bad name, which ends in a dot, in line " << line;
    end(EXIT_FAILURE);
  }
  identifier = this->GetIdentifier();
  SyntaxAnalyzer::MakeKinship(dot, identifier);

  if (!tokens_.empty()) {
    if (SyntaxAnalyzer::IsDot(this->peek_first_token())) {
      next_identifiers = this->GetIdentifiers();
      SyntaxAnalyzer::MakeKinship(dot, next_identifiers);
    }
  }

  return dot;
}

std::shared_ptr<INode> SyntaxAnalyzer::GetIdentifier() {
  this->ValidateIsWord(this->peek_first_token());
  std::shared_ptr<INode> identifier;
  identifier =
      std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  identifier->stmt_type = StmtType::kIdentifier;

  std::shared_ptr<INode> argument = this->get_first_token();
  SyntaxAnalyzer::MakeKinship(identifier, argument);

  return identifier;
}

std::shared_ptr<INode> SyntaxAnalyzer::GetListOf(
    StmtType get_function_type) {
  std::shared_ptr<INode> argument, separator, next_separator;

  // Get separator (comma)
  int line = this->peek_first_token()->line;
  this->pop_first_token();
  separator = std::dynamic_pointer_cast<INode>(std::make_shared<ServiceNode>());
  separator->stmt_type = StmtType::kCommaDelimiter;

  if (tokens_.empty()) {
    LOGE << "invalid listOf in line "
        << line << ": argument is missed";
    end(EXIT_FAILURE);
  }
  switch (get_function_type) {
    case StmtType::kIdentifier:
      argument = this->GetIdentifier();
      break;
    case StmtType::kTableDef:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetTableDefinitionObject();
      break;
    case StmtType::kColumnDef:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetColumnDefinition();
      break;
    case StmtType::kTableConstraint:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetTableConstraint();
      break;
    case StmtType::kName:
      this->ValidateIsWord(this->peek_first_token());
      argument = this->GetName();
      break;
    default:
      LOGE << "unknown statement type for the listOf in line "
          << this->peek_first_token()->line;
      end(EXIT_FAILURE);
  }
  SyntaxAnalyzer::MakeKinship(separator, argument);

  if (!tokens_.empty()
      && SyntaxAnalyzer::IsComma(this->peek_first_token())) {
    next_separator = this->GetListOf(get_function_type);
    SyntaxAnalyzer::MakeKinship(separator, next_separator);
  }

  return separator;
}

} // scc::parser
