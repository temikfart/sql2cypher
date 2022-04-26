#include "SCC/tree_dump.h"

#include <utility>

TreeDump::TreeDump(std::string dot_path, std::string png_path) :
  dot_path_(std::move(dot_path)), png_path_(std::move(png_path)) {}

void TreeDump::Dump(const std::shared_ptr<Node> &AST) {
  dot_file_.open(dot_path_, std::ios::out);
  this->ValidateDotFile();

  dot_file_ << "digraph Dump\n"
           << "{\n";
  this->PrintDumpInfo();
  this->RecursiveTreeDump(AST, -1);
  dot_file_ << "\n}";

  this->MakeDumpPng();
  this->CloseDotFile();
}
void TreeDump::PrintDumpInfo() {
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
}
void TreeDump::RecursiveTreeDump(const std::shared_ptr<Node> &node,
                                 int parent_node_num) {
  static int num_of_nodes = -1;
  if (parent_node_num == -1) {
    num_of_nodes = -1; //If this function will be called more than one time
  }
  num_of_nodes++;
  int current_node_num = num_of_nodes;

  size_t num_of_children = node->get_children_amount();
  DataType type = node->get_type();
  StatementType st_type = node->get_st_type();

  dot_file_ << "\tNode" << current_node_num
           << " [shape = record, color = black, label = ";

  switch (type) {
    case DataType::ROOT:
      dot_file_ << "\" {<data> ROOT";
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = gray];\n";
      break;
    case DataType::SERVICE:
      dot_file_ << "\" {<st_type> "
               << GetServiceNodeData(st_type)
               <<" } \", style = filled, fillcolor = purple, "
                 "fontcolor = white];\n";
      break;
    case DataType::INT_NUMBER:
      dot_file_ << "\" {<data> "
               << std::dynamic_pointer_cast<IntNumNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = green, "
                  "fontcolor = black];\n";
      break;
    case DataType::FLOAT_NUMBER:
      dot_file_ << "\" {<data> "
               << std::dynamic_pointer_cast<FloatNumNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = green, "
                  "fontcolor = black];\n";
      break;
    case DataType::BRACKET:
      dot_file_ << "\" {<data> "
               << std::dynamic_pointer_cast<CharNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = lightblue, "
                  "fontcolor = black];\n";
      break;
    case DataType::PUNCTUATION:
      dot_file_ << "\" {<data> \'"
               << std::dynamic_pointer_cast<CharNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = blue, "
                  "fontcolor = white];\n";
      break;
    case DataType::WORD:
      dot_file_ << "\" {<data> \\\""
               << std::dynamic_pointer_cast<StringNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = red, "
                  "fontcolor = white];\n";
      break;
    case DataType::OPERATOR:
      dot_file_ << "\" {<data> "
               << std::dynamic_pointer_cast<StringNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = orange, "
                  "fontcolor = white];\n";
      break;
    case DataType::STRING:
      dot_file_ << "\" {<data> \\\""
               << std::dynamic_pointer_cast<StringNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file_ << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file_ << "} \", style = filled, fillcolor = yellow, "
                  "fontcolor = black];\n";
      break;
    default:
      LOG(ERROR, "Wrong node type");
      exit(EXIT_FAILURE);
  }

  if (num_of_nodes > 0) {
    dot_file_ << "\tNode" << parent_node_num << " -> Node"
             << current_node_num << ";\n";
  }

  for (size_t i = 0; i < num_of_children; i++)
    this->RecursiveTreeDump(node->get_child(i), current_node_num);
}

void TreeDump::MakeDumpPng() {
  std::ostringstream txt_to_png;
  txt_to_png << "dot " << dot_path_ << " -T png -o "
             << png_path_ << std::endl;

  printf("%s", txt_to_png.str().c_str());

  std::system(txt_to_png.str().c_str()); //TODO: fix problem with executing this command
}

std::string TreeDump::GetServiceNodeData(StatementType statement_type) {
  switch (statement_type) {
    case StatementType::EMPTY_TYPE:
      return "EMPTY_TYPE";
    case StatementType::Program:
      return "Program";
    case StatementType::query:
      return "query";
    case StatementType::ddlStatement:
      return "ddlStatement";
    case StatementType::dmlStatement:
      return "dmlStatement";

    // DDL Statements
    case StatementType::alterTableStatement:
      return "alterTableStatement";
    case StatementType::createDatabaseStatement:
      return "createDatabaseStatement";
    case StatementType::createTableStatement:
      return "createTableStatement";
    case StatementType::dropDatabaseStatement:
      return "dropDatabaseStatement";
    case StatementType::dropTableStatement:
      return "dropTableStatement";

    // DDL Basic Statements
    case StatementType::tableDefinition:
      return "tableDefinition";
    case StatementType::columnDefinition:
      return "columnDefinition";
    case StatementType::tableConstraint:
      return "tableConstraint";
    case StatementType::alterActionADD:
      return "alterActionADD";
    case StatementType::alterActionDROP:
      return "alterActionDROP";
    case StatementType::dropList:
      return "dropList";
    case StatementType::dropConstraint:
      return "dropConstraint";
    case StatementType::dropColumn:
      return "dropColumn";

    // DML Statements
    case StatementType::deleteStatement:
      return "deleteStatement";
    case StatementType::insertStatement:
      return "insertStatement";
    case StatementType::updateStatement:
      return "updateStatement";

    // DML Basic Statements
    case StatementType::condition:
      return "condition";
    case StatementType::ORcondition:
      return "ORcondition";
    case StatementType::ANDcondition:
      return "ANDcondition";
    case StatementType::NOTcondition:
      return "NOTcondition";
    case StatementType::predicate:
      return "predicate";
    case StatementType::expression:
      return "expression";

    // Basic Statements
    case StatementType::primaryKey:
      return "primaryKey";
    case StatementType::foreignKey:
      return "foreignKey";
    case StatementType::reference:
      return "reference";
    case StatementType::name:
      return "name";
    case StatementType::identifier:
      return "identifier";
    case StatementType::delimiter_dot:
      return "delimiter_dot";
    case StatementType::delimiter_comma:
      return "delimiter_comma";
    case StatementType::delimiter_semicolon:
      return "delimiter_semicolon";

    // Basic Statements
    case StatementType::kw_constraint:
      return "kw_constraint";

    // SQL datatypes
    case StatementType::SQL_int:
      return "SQL_int";
    case StatementType::SQL_float:
      return "SQL_float";
    case StatementType::SQL_char:
      return "SQL_char";
    case StatementType::SQL_varchar:
      return "SQL_varchar";

    // Basic Statements
    case StatementType::StTypeCount:
      return "StTypeCount";

    default:
      LOG(ERROR, "Unknown statement type");
      exit(EXIT_FAILURE);
  }
}

void TreeDump::ValidateDotFile() {
  if (!dot_file_.is_open()) {
    LOG(ERROR, "dot dump file stream is invalid");
    exit(EXIT_FAILURE);
  }
  LOG(DEBUG, "dot dump file stream is valid");
}
void TreeDump::CloseDotFile() {
  LOG(DEBUG, "closing dot dump file...");
  if (dot_file_.is_open()) {
    dot_file_.close();
    if (dot_file_.good()) {
      LOG(DEBUG, "dot dump file closed successfully");
    } else {
      LOG(ERROR, "dot dump file close error");
      exit(EXIT_FAILURE);
    }
  } else {
    LOG(INFO, "dot dump file is already closed");
  }
}

