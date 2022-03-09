#include "SCC/lexical_analyzer.h"

//-------------------Node------------------------

Node::Node(const N_type type): type_(type) {}

Node::~Node() {
  for(auto & node_ptr : children_) {
    delete node_ptr;
  }
}

N_type Node::get_type() {
  return type_;
}

Node* Node::get_children (int node_num) {
  if (node_num > children_.size()) {
    LOG(ERROR, "Node number too big")
    return nullptr;
  }
  return children_[node_num];
}

void Node::add_children(Node* node) {
  children_.push_back(node);
}

size_t Node::get_children_amount () {
  return children_.size();
}

//-----------------------------------------------

namespace Tree {
  void PrintTreeRecursive(Node *node) {
    node->print_data();

    if (node->get_children_amount() == 0)
      return;

    for (int i = 0; i < node->get_children_amount(); i++) {
      PrintTreeRecursive(node->get_children(i));
    }
  }
}

using SNode = Node*;

void test_tree_func() {
  Node* root = new RootNode();

  root->add_children(new IntNode(125));
  root->add_children(new CharNode('+'));
  root->add_children(new StringNode("le cringe"));
  root->add_children(new IntNode(125));

  Node* child_node = root->get_children(0);

  child_node->add_children(new StringNode("lol"));

  Tree::PrintTreeRecursive(root);
}