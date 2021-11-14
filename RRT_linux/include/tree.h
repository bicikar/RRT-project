#ifndef RRT_PROJECT_TREE_H
#define RRT_PROJECT_TREE_H

#include <vector>
#include "kdtree.h"

class Tree {
public:
    void addNode(Node *node) {
        _nodes.push_back(node);
    }


    Node *_root;
    std::vector<Node *> _nodes;
};

#endif
