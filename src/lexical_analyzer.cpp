#include "SCC/lexical_analyzer.h"

void test_tree_func() {
  Node* root = new Node;

  root->add_children(1);
  root->add_children(2);
  root->add_children(3);

  root->PrintTree();
}