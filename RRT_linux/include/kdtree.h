#ifndef RRT_PROJECT_KDTREE_H
#define RRT_PROJECT_KDTREE_H

#include <vector>
#include "field.h"

struct Point {
    int x;
    int y;

    Point(int xi, int yi) {
        x = xi;
        y = yi;
    }

    Point() {
        x = -1;
        y = -1;
    }

    int get(int index) {
        if (index == 0) {
            return x;
        }
        if (index == 1) {
            return y;
        }
    }
};

struct Node {
public:
    int _x;
    int _y;
    std::vector<Node *> _child;
    Node *parent = nullptr;
    Node *_right = nullptr;
    Node *_left = nullptr;
    std::vector<Node *> _same;

    Node() : _x(-1), _y(-1) {}

    Node(int x, int y) : _x(x), _y(y) {}

    void add_child(Node *node) {
        _child.push_back(node);
    }

    ~Node() {
        for (auto node: _child){
            delete node;
        }
    }

    int get(int index) const {
        if (index == 0) {
            return _x;
        }
        if (index == 1) {
            return _y;
        }
    }

    int distance(Point *point) const {
        return (point->x - _x) * (point->x - _x) + (point->y - _y) * (point->y - _y);
    }

};


class KDTree {
public:
    Node *_parent;
    Node *_best = nullptr;
    int _best_dist = 0;
    int _dim = 2;
    int _visited = 0;

    Node *insertRec(Node *point, Node *node, int sDim) {
        if (node == nullptr) {
            return point;
        }
        if (point->get(sDim) < node->get(sDim)) {
            node->_left = insertRec(point, node->_left, (sDim + 1) % _dim);
        } else if (point->get(sDim) > node->get(sDim)) {
            node->_right = insertRec(point, node->_right, (sDim + 1) % _dim);
        } else {
            node->_same.push_back(point);
        }
        return node;
    }


    Node *insert(Node *root, Node *point) {
        return insertRec(point, root, 0);
    }

    void nearestRec(Node *root, Point *point, int index) {
        if (root == nullptr)
            return;
        _visited++;
        int d = root->distance(point);
        if (_best == nullptr || d < _best_dist) {
            _best_dist = d;
            _best = root;
        }
        if (_best_dist == 0)
            return;
        double dx = root->get(index) - point->get(index);
        index = (index + 1) % _dim;
        nearestRec(dx > 0 ? root->_left : root->_right, point, index);
        if (dx * dx >= _best_dist)
            return;
        nearestRec(dx > 0 ? root->_right : root->_left, point, index);
    }

    Node *nearest(Point *pt) {
        if (_parent == nullptr)
            throw;
        _best = nullptr;
        _best_dist = 0;
        _visited = 0;
        nearestRec(_parent, pt, 0);
        return _best;
    }


};

#endif //RRT_PROJECT_KDTREE_H
