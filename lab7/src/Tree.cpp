#include "../inc/Tree.hh"

Tree::Tree() {
  root = nullptr;
  s = 0;
}

Tree::~Tree() {

}

int Tree::size() {
  return s;
}

bool Tree::isEmpty() {
  return root != nullptr;
}

void Tree::insert(TreeNode* tree, int value) {
  if (tree != nullptr) {
    if (value < tree->key)
      insert(tree->left, value);
    if (value > tree->key)
      insert(tree->right, value);
  } else {
    tree = new TreeNode();
    tree->key = value;
    tree->left = nullptr;
    tree->right = nullptr;
    s++;
  }
}

void Tree::print_inorder(std::ostream& out, TreeNode* tree) {
  if (tree != nullptr) {
    print_inorder(out, tree->left);
    out << "v: " << tree->key << "\n";
    print_inorder(out, tree->right);
  }
}
