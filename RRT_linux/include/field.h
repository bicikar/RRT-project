#ifndef RRT_PROJECT_FIELD_H
#define RRT_PROJECT_FIELD_H

#include <cstddef>
#include <string>
#include "tree.h"
#include "kdtree.h"

struct NodeAndBool {
    Node *node;
    bool ans;

    NodeAndBool(Node *nodei, bool ansi) {
        node = nodei;
        ans = ansi;
    }
};


class Field {
public:
    Field(const std::string &file_name, int step = 0);

    ~Field();

    void print(const std::string &output_name);

    NodeAndBool draw_path(int x1, int y1, int x2, int y2, bool del, char symbol);

    Point *take_random_point() const;

    static int distance(Point *point, Node *node);

    static int distance(Node *node1, Node *node2);

    Node *steer(Point *point_rand, Node *node_nearest);

    Node *find_nearest(Point *point);

    void rrt_algo(int iterations);

    void draw_tree(Node *node);

    Node *get_root() const {
        return _tree._root;
    }

    void read_parent(Node *node);

    void make_path_vector(Node *node);

public:
    size_t sample_step;
    size_t _rows;
    size_t _cols;
    char **_data;
    Point start_point;
    Point end_point;
    Tree _tree;
    KDTree _kdtree;
    Node *_result_node = nullptr;
    size_t _distance = 0;
    std::vector<Node *> _path_vector;
    std::vector<float> _diff;
};

#endif
