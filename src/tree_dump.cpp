#include "SCC/tree_dump.h"

TreeDump::TreeDump(const std::filesystem::path& out_path)
: dot_file_(out_path), dot_file_path_(out_path.string()) {}

void TreeDump::DumpTree(const std::shared_ptr<INode>& AST) {
  LOGI << "creating tree dump of the AST";
  if (AST == nullptr) {
    LOGI << "creation is ended: AST is empty";
    return;
  }

  dot_file_ << "digraph Dump\n"
            << "{\n";
  this->PrintDumpInfo();
  LOGI << "recursive AST pass and dump creation...";
  this->RecursiveTreeDump(AST, -1);
  dot_file_ << "\n}";
  LOGI << "dump is created";

  CloseTreeDumpFile();
  this->MakeDumpPng();
  LOGI << "graphviz dot file is written and image is created";
}
void TreeDump::PrintDumpInfo() {
  LOGI << "printing dump info to the dot file";
  dot_file_ << "\tsubgraph info {\n"
            << "\t\tedge [color = white];\n"
            << "\t\tnode [color = black, shape = record];\n"
            << "\t\tnode [style = filled, fillcolor = gray, label = ROOT];\n"
            << "\t\troot;\n"
            << "\t\tnode [style = filled, fillcolor = purple,"
               " fontcolor = white, label = SERVICE];\n"
            << "\t\tservice;\n"
            << "\t\tnode [style = filled, fillcolor = green,"
               " fontcolor = black, label = \"INT/FLOAT_NUMBER\"];\n"
            << "\t\tint_float_number;\n"
            << "\t\tnode [style = filled, fillcolor = lightblue,"
               " fontcolor = black,  label = BRACKET];\n"
            << "\t\tbracket;\n"
            << "\t\tnode [style = filled, fillcolor = blue,"
               " fontcolor = white, label = PUNCTUATION];\n"
            << "\t\tpunctuation;\n"
            << "\t\tnode [style = filled, fillcolor = orange,"
               " label = OPERATOR];\n"
            << "\t\toperator;\n"
            << "\t\tnode [style = filled, fillcolor = red, label = WORD];\n"
            << "\t\tword;\n"
            << "\t\tnode [style = filled, fillcolor = yellow,"
               " fontcolor = black, label = STRING];\n"
            << "\t\tstring;\n"
            << "\t\troot->service->int_float_number->bracket->"
               "punctuation->operator->word->string;\n"
            << "\t\tlabel = \"graph info\";\n"
            << "\t\tcolor = black;\n"
            << "\t}\n";
  LOGI << "dump info is printed";
}
void TreeDump::RecursiveTreeDump(const std::shared_ptr<INode>& node,
                                 int parent_node_num) {
  static int num_of_nodes = -1;
  if (parent_node_num == -1) {
    num_of_nodes = -1; // If this function will be called more than one time
  }
  num_of_nodes++;
  int current_node_num = num_of_nodes;

  size_t num_of_children = node->get_children_amount();
  DataType type = node->data_type;
  StmtType st_type = node->stmt_type;

  dot_file_ << "\tNode" << current_node_num
            << " [shape = record, color = black, label = ";

  switch (type) {
    case DataType::kRoot:
      dot_file_ << "\" {<data> ROOT";
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = gray];\n";
      break;
    case DataType::kService:
      dot_file_ << "\" {<st_type> "
                << GetServiceNodeData(st_type)
                << " } \", style = filled, fillcolor = purple, "
                   "fontcolor = white];\n";
      break;
    case DataType::kInt:
      dot_file_ << "\" {<data> "
                << std::dynamic_pointer_cast<IntNumNode>(node)->get_data();
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = green, "
                   "fontcolor = black];\n";
      break;
    case DataType::kFloat:
      dot_file_ << "\" {<data> "
                << std::dynamic_pointer_cast<FloatNumNode>(node)->get_data();
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = green, "
                   "fontcolor = black];\n";
      break;
    case DataType::kBracket:
      dot_file_ << "\" {<data> "
                << std::dynamic_pointer_cast<CharNode>(node)->get_data();
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = lightblue, "
                   "fontcolor = black];\n";
      break;
    case DataType::kPunctuation:
      dot_file_ << "\" {<data> \'"
                << std::dynamic_pointer_cast<CharNode>(node)->get_data();
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = blue, "
                   "fontcolor = white];\n";
      break;
    case DataType::kWord:
      dot_file_ << R"(" {<data> \")"
                << std::dynamic_pointer_cast<StringNode>(node)->get_data()
                << R"(\")";
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = red, "
                   "fontcolor = white];\n";
      break;
    case DataType::kOperator:
      dot_file_ << "\" {<data> "
                << std::dynamic_pointer_cast<StringNode>(node)->get_data();
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = orange, "
                   "fontcolor = white];\n";
      break;
    case DataType::kString:
      dot_file_ << R"(" {<data> \")"
                << std::dynamic_pointer_cast<StringNode>(node)->get_data()
                << R"(\")";
      if (st_type != StmtType::kNone)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = yellow, "
                   "fontcolor = black];\n";
      break;
    default:
      LOGE << "wrong node type";
      end(EXIT_FAILURE);
  }

  if (num_of_nodes > 0) {
    dot_file_ << "\tNode" << parent_node_num << " -> Node"
              << current_node_num << ";\n";
  }

  for (size_t i = 0; i < num_of_children; i++)
    this->RecursiveTreeDump(node->get_child(i), current_node_num);
}

void TreeDump::MakeDumpPng() {
  LOGI << "invoking dot command for creation the image...";
  std::string png_file_path =
      dot_file_path_.substr(0, dot_file_path_.find_last_of('.')) + ".png";
  std::ostringstream txt_to_png;
  txt_to_png << "dot \"" << dot_file_path_ << "\" -T png -o \""
             << png_file_path << "\"";

  LOGD << "executing: " << txt_to_png.str();

  int wait_status = std::system(txt_to_png.str().c_str());
  if (wait_status) {
    LOGE << "execution ended with error, wait status is " << wait_status;
  } else {
    LOGI << "the image with tree dump "
              "was created at \'" << png_file_path << "\'";
  }
}

std::string TreeDump::GetServiceNodeData(StmtType statement_type) {
  switch (statement_type) {
    case StmtType::kNone:
      return "EMPTY_TYPE";
    case StmtType::kProgram:
      return "Program";
    case StmtType::kQuery:
      return "query";
    case StmtType::kDdlStmt:
      return "ddlStatement";
    case StmtType::kDmlStmt:
      return "dmlStatement";

      // DDL Statements
    case StmtType::kAlterTableStmt:
      return "alterTableStatement";
    case StmtType::kCreateDatabaseStmt:
      return "createDatabaseStatement";
    case StmtType::kCreateTableStmt:
      return "createTableStatement";
    case StmtType::kDropDatabaseStmt:
      return "dropDatabaseStatement";
    case StmtType::kDropTableStmt:
      return "dropTableStatement";

      // DDL Basic Statements
    case StmtType::kTableDef:
      return "tableDefinition";
    case StmtType::kColumnDef:
      return "columnDefinition";
    case StmtType::kTableConstraint:
      return "tableConstraint";
    case StmtType::kAlterActionAdd:
      return "alterActionADD";
    case StmtType::kAlterActionDrop:
      return "alterActionDROP";
    case StmtType::kDropList:
      return "dropList";
    case StmtType::kDropConstraint:
      return "dropConstraint";
    case StmtType::kDropColumn:
      return "dropColumn";

      // DML Statements
    case StmtType::kDeleteStmt:
      return "deleteStatement";
    case StmtType::kInsertStmt:
      return "insertStatement";
    case StmtType::kUpdateStmt:
      return "updateStatement";

      // DML Basic Statements
    case StmtType::kCondition:
      return "condition";
    case StmtType::kORCondition:
      return "ORcondition";
    case StmtType::kANDCondition:
      return "ANDcondition";
    case StmtType::kNOTCondition:
      return "NOTcondition";
    case StmtType::kPredicate:
      return "predicate";
    case StmtType::kExpression:
      return "expression";

      // Basic Statements
    case StmtType::kPrimaryKey:
      return "primaryKey";
    case StmtType::kForeignKey:
      return "foreignKey";
    case StmtType::kReference:
      return "reference";
    case StmtType::kName:
      return "name";
    case StmtType::kIdentifier:
      return "identifier";
    case StmtType::kDotDelimiter:
      return "delimiter_dot";
    case StmtType::kCommaDelimiter:
      return "delimiter_comma";
    case StmtType::kSemicolonDelimiter:
      return "delimiter_semicolon";

      // Basic Statements
    case StmtType::kConstraintKW:
      return "kw_constraint";

      // SQL datatypes
    case StmtType::kIntType:
      return "SQL_int";
    case StmtType::kFloatType:
      return "SQL_float";
    case StmtType::kCharType:
      return "SQL_char";
    case StmtType::kVarcharType:
      return "SQL_varchar";

    default:
      LOGE << "unknown statement type";
      end(EXIT_FAILURE);
      return "";
  }
}

bool TreeDump::CloseTreeDumpFile() {
  if (dot_file_.is_open()) {
    dot_file_.close();
    if (dot_file_.good()) {
      ;
    } else {
      std::cerr << "dot dump file close error" << std::endl;
      return false;
    }
  }
  return true;
}
