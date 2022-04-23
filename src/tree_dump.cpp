#include "SCC/tree_dump.h"

void TreeDump::Dump(const std::shared_ptr<Node> &AST) {
  std::string dot_file_path = "../resources/tree_dump/tree_dump.txt";
  std::string png_file_path = "../resources/tree_dump/tree_dump.png";
  std::ofstream dot_file;
  dot_file.open(dot_file_path, std::ios::out);
  //TODO: opening check

  dot_file << "digraph Dump\n"
           << "{\n";
  PrintDumpInfo(dot_file);
  RecursiveTreeDump(AST, dot_file, -1);
  dot_file << "\n}";

//  MakeDumpPng(dot_file_path, png_file_path);
  dot_file.close();
  //TODO: closing check
}
void TreeDump::PrintDumpInfo(std::ofstream &dot_file) {
  dot_file << "\tsubgraph info {\n"
           << "\tedge [color = white];\n"
           << "\tnode [color = black, shape = record];\n"
           << "\tnode [style = filled, fillcolor = gray, label = ROOT];\n"
           << "\troot;\n"
           << "\tnode [style = filled, fillcolor = purple, fontcolor = white, label = SERVICE];\n"
           << "\tservice;\n"
           << "\tnode [style = filled, fillcolor = green, fontcolor = black, label = \"INT/FLOAT_NUMBER\"];\n"
           << "\tint_float_number;\n"
           << "\tnode [style = filled, fillcolor = lightblue, fontcolor = black,  label = BRACKET];\n"
           << "\tbracket;\n"
           << "\tnode [style = filled, fillcolor = blue, fontcolor = white, label = PUNCTUATION];\n"
           << "\tpunctuation;\n"
           << "\tnode [style = filled, fillcolor = orange, label = OPERATOR];\n"
           << "\toperator;\n"
           << "\tnode [style = filled, fillcolor = red, label = WORD];\n"
           << "\tword;\n"
           << "\tnode [style = filled, fillcolor = yellow, fontcolor = black, label = STRING];\n"
           << "\tstring;\n"
           << "\tstyle = filled;\n"
           << "\tcolor = lightgray;\n"
           << "\troot->service->int_float_number->bracket->punctuation->operator->word->string;\n"
           << "\tlabel = \"graph info\";\n"
           << "\t}\n";
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
  DataType type = node->get_type();
  StatementType st_type = node->get_st_type();

  dot_file << "\tNode" << current_node_num
           << " [shape = record, color = black, label = ";

  switch (type) {
    case DataType::ROOT:
      dot_file << "\" {<data> ROOT";
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = gray];\n";
      break;
    case DataType::SERVICE:
      dot_file << "\" <st_type> "
               << GetServiceNodeData(st_type)
               <<" } \", style = filled, fillcolor = purple, "
                 "fontcolor = white];\n";
      break;
    case DataType::INT_NUMBER:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<IntNumNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = green, "
                  "fontcolor = black];\n";
      break;
    case DataType::FLOAT_NUMBER:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<FloatNumNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = green, "
                  "fontcolor = black];\n";
      break;
    case DataType::BRACKET:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<CharNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = lightblue, "
                  "fontcolor = black];\n";
      break;
    case DataType::PUNCTUATION:
      dot_file << "\" {<data> \'"
               << std::dynamic_pointer_cast<CharNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = blue, "
                  "fontcolor = white];\n";
      break;
    case DataType::WORD:
      dot_file << "\" {<data> \\\""
               << std::dynamic_pointer_cast<StringNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = red, "
                  "fontcolor = white];\n";
      break;
    case DataType::OPERATOR:
      dot_file << "\" {<data> "
               << std::dynamic_pointer_cast<StringNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = orange, "
                  "fontcolor = white];\n";
      break;
    case DataType::STRING:
      dot_file << "\" {<data> \\\""
               << std::dynamic_pointer_cast<StringNode>(node)->get_data();
      if (st_type != EMPTY_TYPE)
        dot_file << " | <st_type>" << GetServiceNodeData(st_type);
      dot_file << "} \", style = filled, fillcolor = yellow, "
                  "fontcolor = black];\n";
      break;
    default:
      LOG(ERROR, "Wrong node type");
      exit(EXIT_FAILURE);
  }

  if (num_of_nodes > 0) {
    dot_file << "\tNode" << parent_node_num << " -> Node"
             << current_node_num << ";\n";
  }

  for (size_t i = 0; i < num_of_children; i++) {
    RecursiveTreeDump(node->get_child(i), dot_file, current_node_num);
  }
}
//void TreeDump::MakeDumpPng(const std::string &dot_file_path,
//                           const std::string &png_file_path) {
//  std::ostringstream txt_to_png;
//  txt_to_png << "dot " << dot_file_path << " -T png -o "
//             << png_file_path << std::endl;
//
//  printf("%s", txt_to_png.str().c_str());
//
//  std::system(txt_to_png.str().c_str()); //TODO: fix problem with executing this command
//}
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