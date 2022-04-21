#include "SCC/tree_dump.h"

void TreeDump::Dump(const std::shared_ptr<Node> &AST) {
  std::string dot_file_path = "../resources/tree_dump/tree_dump.txt";
  std::string png_file_path = "../resources/tree_dump/tree_dump.png";
  std::ofstream dot_file;
  dot_file.open(dot_file_path, std::ios::out);
  //TODO: opening check

  dot_file << "digraph Dump\n"
           << "{\n";
  RecursiveTreeDump(AST, dot_file, -1);
  dot_file << "\n}";

  MakeDumpPng(dot_file_path, png_file_path);

  dot_file.close();
  //TODO: closing check
}
void TreeDump::RecursiveTreeDump(const std::shared_ptr<Node> &node,
                              std::ofstream &dot_file, int parent_node_num) {
  static int num_of_nodes = -1;
  if (parent_node_num == -1) {
    num_of_nodes = -1; //If this function will be called more than one time
  }
  num_of_nodes++;
  int current_node_num = num_of_nodes;

  size_t num_of_children = node->get_children_amount();

  dot_file << "\tNode" << current_node_num << " [shape = record, label = ";

  switch (node->get_type()) {
    case DataType::ROOT:
      dot_file << "\" {<data> ROOT | <type> ROOT} \"];\n";
      break;
    case DataType::SERVICE:
      dot_file << "\" {<data> "
               << GetServiceNodeData(node->get_st_type())
               <<" | <type> SERVICE} \"];\n";
      break;
    case DataType::INT_NUMBER:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<IntNumNode>(node)->get_data()
               << " | <type> INT_NUMBER} \"];\n";
      break;
    case DataType::FLOAT_NUMBER:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<FloatNumNode>(node)->get_data()
               << " | <type> FLOAT_NUMBER} \"];\n";
      break;
    case DataType::BRACKET:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<CharNode>(node)->get_data()
               << " | <type> BRACKET} \"];\n";
      break;
    case DataType::PUNCTUATION:
      dot_file << "\" {<data> \'"
               << std::dynamic_pointer_cast<CharNode>(node)->get_data()
               << "\' | <type> PUNCTUATION} \"];\n";
      break;
    case DataType::WORD:
      dot_file << "\" {<data> \\\""
               << std::dynamic_pointer_cast<StringNode>(node)->get_data()
               << "\\\" | <type> WORD} \"];\n";
      break;
    case DataType::OPERATOR:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<StringNode>(node)->get_data()
               << " | <type> OPERATOR} \"];\n";
      break;
    case DataType::STRING:
      dot_file << "\" {<data> \\\""
               << std::dynamic_pointer_cast<StringNode>(node)->get_data()
               << "\\\" | <type> STRING} \"];\n";
      break;
    default:
      LOG(ERROR, "Wrong node type");
      exit(EXIT_FAILURE);
  }

  if (num_of_nodes > 0) {
    dot_file << "\tNode" << parent_node_num << " -> Node"
             << current_node_num << "\n";
  }

  for (size_t i = 0; i < num_of_children; i++) {
    RecursiveTreeDump(node->get_child(i), dot_file, current_node_num);
  }
}
void TreeDump::MakeDumpPng(const std::string &dot_file_path,
                           const std::string &png_file_path) {
  std::ostringstream txt_to_png;
  txt_to_png << "dot " << dot_file_path << " -T png -o "
             << png_file_path << std::endl;

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
      return ".";
    case StatementType::delimiter_comma:
      return ",";
    case StatementType::delimiter_semicolon:
      return ";";

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