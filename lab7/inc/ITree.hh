#include "../inc/TreeNode.hh"

class ITree {
public:
  virtual int size() = 0;
  virtual bool isEmpty() = 0;
  virtual void insert(TreeNode*, int) = 0;
  virtual int remove() = 0;
  virtual int find() = 0;
  virtual ~ITree() {};
};
