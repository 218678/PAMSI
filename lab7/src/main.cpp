#include "../inc/Tree.hh"
#include <iostream>


int main() {

  Tree* myTree;
  myTree = new Tree();

  myTree->insert(myTree->root, 7);
  myTree->print_inorder(std::cout, myTree->root);

  std::cout << myTree->isEmpty();

  return 0;
}
