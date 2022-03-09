#include "SCC/lexical_analyzer.h"

namespace Tree {
  void PrintTreeRecursive(Node *node) {
    switch (node->get_type()) {
      case DIGIT:
        std::cout << ((IntNode *) node)->get_data() << " "; //TODO: learn about casts
        break;
      case OPERATOR:
        std::cout << ((CharNode *) node)->get_data() << " ";
        break;
      case STRING:
        std::cout << ((StringNode *) node)->get_data() << " ";
        break;
      case ROOT:
        std::cout << "ROOT" << " ";
        break;
    }

    if (node->get_children_amount() == 0)
      return;
    else
      std::cout << std::endl;

    for (int i = 0; i < node->get_children_amount(); i++) {
      PrintTreeRecursive(node->get_children(i));
    }
  }
}

using SNode = Node*;

void test_tree_func() {
  auto root = new Node(N_type::ROOT);

  root->add_children((SNode)(new IntNode(125)));
  root->add_children((Node*)(new CharNode('+')));
  root->add_children((Node*)(new StringNode("le cringe")));
  root->add_children((Node*)(new IntNode(125)));

  Node* child_node = root->get_children(0);

  child_node->add_children((Node*)(new StringNode("lol")));

  Tree::PrintTreeRecursive(root);
}