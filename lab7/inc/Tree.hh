#include "../inc/ITree.hh"
//#include "../inc/TreeNode.hh"
#include <iostream>

class Tree: public ITree {
private:
  int s; //size
public:
  TreeNode* root;
  int size();
  bool isEmpty();
  void insert(TreeNode* tree, int value);
  int remove() {return 1;};
  int find() {return 1;};
  void print_inorder(std::ostream& out, TreeNode* tree);

  Tree();
  ~Tree();
};
