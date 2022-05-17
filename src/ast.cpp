#include "SCC/ast.h"

/* ------------------ Node ------------------ */

Node::Node(DataType type) : type_(type) {}
Node::~Node() = default;

void Node::set_line(int line) {
  line_ = line;
}
int Node::get_line() {
  return line_;
}
void Node::set_st_type(StatementType type) {
  this->ValidateStType(type);
  st_type_ = type;
}
StatementType Node::get_st_type() {
  return st_type_;
}
void Node::set_parent(std::shared_ptr<Node>& node) {
  parent_ = node;
}
DataType Node::get_type() const {
  return type_;
}
std::shared_ptr<Node>& Node::get_child(size_t node_num) {
  this->ValidateChildNumber(node_num);
  return children_[node_num];
}
size_t Node::get_children_amount() const {
  return children_.size();
}
std::shared_ptr<Node>& Node::get_parent() {
  return parent_;
}

void Node::AddChild(std::shared_ptr<Node> const& node) {
  this->ValidateAddChild(node);
  children_.push_back(node);
}
bool Node::IsNodesEqual(const std::shared_ptr<Node>& node1,
                        const std::shared_ptr<Node>& node2) {
  if (node1->get_type() != node2->get_type() ||
      node1->get_children_amount()
          != node2->get_children_amount()) { //TODO: add parent ptr comparison
    LOG(DEBUG, "not equal: comparing nodes with different "
               "numbers of children or have different types");
    return false;
  }

  switch (node1->get_type()) {
    case DataType::ROOT:
      break;
    case DataType::INT_NUMBER:
      if (std::dynamic_pointer_cast<IntNumNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<IntNumNode>(node2)->get_data()) {
        LOG(DEBUG, "not equal: different integer number data");
        return false;
      }
      break;
    case DataType::FLOAT_NUMBER:
      if (std::dynamic_pointer_cast<FloatNumNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<FloatNumNode>(node2)->get_data()) {
        LOG(DEBUG, "not equal: different float number data");
        return false;
      }
      break;
    case DataType::PUNCTUATION:
    case DataType::BRACKET:
      if (std::dynamic_pointer_cast<CharNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<CharNode>(node2)->get_data()) {
        return false;
      }
      break;
    case DataType::WORD:
    case DataType::OPERATOR:
      if (std::dynamic_pointer_cast<StringNode>(node1)->get_data() !=
          std::dynamic_pointer_cast<StringNode>(node2)->get_data()) {
        LOG(DEBUG, "not equal: different string data");
        return false;
      }
      break;
    default:
      LOG(ERROR, "invalid Datatype: " << node1->get_type());
      end(EXIT_FAILURE);
  }

  size_t children_amount = node1->get_children_amount();
  for (size_t i = 0; i < children_amount; i++) {
    if (!IsNodesEqual(node1->get_child(i),
                      node2->get_child(i))) {
      LOG(DEBUG, "not equal: different subtrees");
      return false;
    }
  }

  LOG(DEBUG, "nodes are equal");
  return true;
}

void Node::ValidateChildNumber(size_t node_num) const {
  if (node_num >= children_.size()) {
    LOG(ERROR, "node number too big");
    end(EXIT_FAILURE);
  }
}
void Node::ValidateAddChild(std::shared_ptr<Node> const& node) const {
  if (node == nullptr) {
    LOG(ERROR, "nullptr can not be added to the tree");
    end(EXIT_FAILURE);
  }
}
void Node::ValidateStType(StatementType type) {
  if (StatementType::StTypeCount <= type) {
    LOG(ERROR, "invalid statement type: " << type);
    end(EXIT_FAILURE);
  }
}

/* --------------- IntNumNode --------------- */
IntNumNode::IntNumNode(int value, DataType type)
    : Node(type), data_(value) {
  this->ValidateType(type);
}

void IntNumNode::set_data(const int& data, DataType type) {
  data_ = data;
  this->ValidateType(type);
  type_ = type;
}
void IntNumNode::set_data(const int& data) {
  data_ = data;
}
int IntNumNode::get_data() const {
  return data_;
}

void IntNumNode::PrintData(std::ostream& stream) {
  stream << data_;
}
void IntNumNode::PrintType(std::ostream& stream) {
  stream << "INT_NUMBER";
}

void IntNumNode::ValidateType(DataType type) const {
  if (type != DataType::INT_NUMBER) {
    LOG(ERROR, "invalid type for IntNumNode: " << type);
    end(EXIT_FAILURE);
  }
}

/* -------------- FloatNumNode -------------- */
FloatNumNode::FloatNumNode(double value, DataType type)
    : Node(type), data_(value) {
  this->ValidateType(type);
}

void FloatNumNode::set_data(const double& data, DataType type) {
  data_ = data;
  this->ValidateType(type);
  type_ = type;
}
void FloatNumNode::set_data(const double& data) {
  data_ = data;
}
double FloatNumNode::get_data() const {
  return data_;
}

void FloatNumNode::PrintData(std::ostream& stream) {
  stream << data_;
}
void FloatNumNode::PrintType(std::ostream& stream) {
  stream << "FLOAT_NUMBER";
}

void FloatNumNode::ValidateType(DataType type) const {
  if (type != DataType::FLOAT_NUMBER) {
    LOG(ERROR, "invalid type for FloatNumNode: " << type);
    end(EXIT_FAILURE);
  }
}

/* ---------------- CharNode ---------------- */

CharNode::CharNode(char ch, DataType type)
    : Node(type), data_(ch) {
  this->ValidateType(type);
}

void CharNode::set_data(const char& data, DataType type) {
  data_ = data;
  this->ValidateType(type);
  type_ = type;
}
void CharNode::set_data(const char& data) {
  data_ = data;
}
char CharNode::get_data() const {
  return data_;
}

void CharNode::PrintData(std::ostream& stream) {
  stream << data_;
}
void CharNode::PrintType(std::ostream& stream) {
  switch (type_) {
    case DataType::BRACKET:
      stream << "BRACKET";
      break;
    case DataType::PUNCTUATION:
      stream << "PUNCTUATION";
      break;
    default:
      LOG(ERROR, "wrong type for CharNode, "
                 "something wrong with validation of type");
      end(EXIT_FAILURE);
  }
}

void CharNode::ValidateType(DataType type) const {
  bool is_BRACKET = type == DataType::BRACKET;
  bool is_PUNCTUATION = type == DataType::PUNCTUATION;
  if (!(is_BRACKET || is_PUNCTUATION)) {
    LOG(ERROR, "invalid type for CharNode: " << type);
    end(EXIT_FAILURE);
  }
}

/* --------------- StringNode --------------- */

StringNode::StringNode(std::string string, DataType type)
    : Node(type), data_(std::move(string)) {
  this->ValidateType(type);
}

void StringNode::set_data(const std::string& data, DataType type) {
  data_ = data;
  this->ValidateType(type);
  type_ = type;
}
void StringNode::set_data(const std::string& data) {
  data_ = data;
}
std::string StringNode::get_data() const {
  return data_;
}

void StringNode::PrintData(std::ostream& stream) {
  stream << data_;
}
void StringNode::PrintType(std::ostream& stream) {
  switch (type_) {
    case DataType::WORD:
      stream << "WORD";
      break;
    case DataType::OPERATOR:
      stream << "OPERATOR";
      break;
    default:
      LOG(ERROR, "wrong type for StringNode, "
                 "something wrong with validation of type");
      end(EXIT_FAILURE);
  }
}

void StringNode::ValidateType(DataType type) const {
  bool is_WORD = type == DataType::WORD;
  bool is_OPERATOR = type == DataType::OPERATOR;
  bool is_STRING = type == DataType::STRING;
  if (!(is_WORD || is_OPERATOR || is_STRING)) {
    LOG(ERROR, "invalid type for StringNode: " << type);
    end(EXIT_FAILURE);
  }
}

/* ---------------- RootNode ---------------- */

RootNode::RootNode() : Node(DataType::ROOT) {
  this->ValidateType(DataType::ROOT);
}

void RootNode::PrintData(std::ostream& stream) {
  stream << "ROOT";
}
void RootNode::PrintType(std::ostream& stream) {
  stream << "ROOT";
}

void RootNode::ValidateType(DataType type) const {
  if (type != DataType::ROOT) {
    LOG(ERROR, "invalid type for RootNode: " << type);
    end(EXIT_FAILURE);
  }
}

/* --------------- ServiceNode -------------- */

ServiceNode::ServiceNode() : Node(DataType::SERVICE) {
  this->ValidateType(DataType::SERVICE);
}

void ServiceNode::PrintData(std::ostream& stream) {
  stream << "SERVICE";
}
void ServiceNode::PrintType(std::ostream& stream) {
  stream << "SERVICE";
}

void ServiceNode::ValidateType(DataType type) const {
  if (type != DataType::SERVICE) {
    LOG(ERROR, "invalid type for ServiceNode: " << type);
    end(EXIT_FAILURE);
  }
}

/* ------------------ Tree ------------------ */

void Tree::PrintTreeRecursive(std::shared_ptr<Node> const& node,
                              std::ostream& stream) {
  node->PrintData(stream);
  stream << std::endl;

  if (node->get_children_amount() == 0)
    return;

  for (size_t i = 0; i < node->get_children_amount(); i++) {
    PrintTreeRecursive(node->get_child(i), std::cout);
  }
}
