#include "../include/field.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>
#include <cmath>

Field::Field(const std::string &file_name, int step) {
    std::ifstream datafile;
    datafile.open("maps/" + file_name);
    if (!datafile.is_open()) {
        throw std::invalid_argument("Incorrect file input");
    }
    datafile >> start_point.x;
    datafile >> start_point.y;
    datafile >> end_point.x;
    datafile >> end_point.y;
    std::string word;
    datafile >> word;
    datafile >> _rows;
    datafile >> word;
    datafile >> _cols;
    _data = new char *[_rows];
    _data[0] = new char[_rows * _cols];
    for (size_t i = 1; i < _rows; i++) {
        _data[i] = _data[i - 1] + _cols;
    }
    for (size_t i = 0; i < _rows; i++) {
        for (size_t j = 0; j < _cols; j++) {
            datafile >> _data[i][j];
        }
    }
    if (_data[start_point.y][start_point.x] == '@') {
        throw std::invalid_argument("Invalid start point");
    }
    if (_data[end_point.y][end_point.x] == '@') {
        throw std::invalid_argument("Invalid end point");
    }
    _data[start_point.y][start_point.x] = 'S';
    _data[end_point.y][end_point.x] = 'E';
    if (step <= 0) {
        sample_step = std::min(_rows, _cols) / 20;
    } else {
        sample_step = step;
    }

    _tree._root = new Node(start_point.x, start_point.y);
    _tree.addNode(_tree._root);
    datafile.close();
    srand(time(NULL));

    _kdtree._parent = _tree._root;

}

Field::~Field() {
    delete _tree._root;
    delete[] _data[0];
    delete[] _data;
}

void Field::print(const std::string &output_name) {
    std::ofstream out;
    out.open(output_name);
    out << "Rows: " << _rows << std::endl;
    out << "Cols: " << _cols << std::endl;
    out << "Start point: " << start_point.x << " " << start_point.y << std::endl;
    out << "Start point: " << end_point.x << " " << end_point.y << std::endl;

    if (_result_node == nullptr) {
        out << "Path was not find" << std::endl;
    } else {
        out << "Path was find. path nodes: " << std::endl;
        make_path_vector(_result_node);
        for (int i = _path_vector.size() - 1; i >= 0; i--) {
            out << "(" << _path_vector[i]->_x << ", " << _path_vector[i]->_y << ") ";
        }
        out << std::endl << "Number of vertices: " << _distance << std::endl;
    }

    for (size_t i = 0; i < _rows; i++) {
        for (size_t j = 0; j < _cols; j++) {
            out << _data[i][j];
        }
        out << std::endl;
    }
}

NodeAndBool Field::draw_path(int x0, int y0, int x1, int y1, bool drawing, char symbol) {
    int A, B, sign;
    A = y1 - y0;
    B = x0 - x1;
    if (abs(A) > abs(B)) sign = 1;
    else sign = -1;
    int signa, signb;
    if (A < 0) {
        signa = -1;
    } else {
        signa = 1;
    }
    if (B < 0) {
        signb = -1;
    } else {
        signb = 1;
    }
    int f = 0;
    if (_data[y0][x0] == '.') {
        _data[y0][x0] = 'P';
    }
    size_t steps = sample_step;
    int x = x0, y = y0;
    if (sign == -1) {
        while ((x != x1 || y != y1) && steps > 0) {
            f += A * signa;
            if (f > 0) {
                f -= B * signb;
                y += signa;
            }
            x -= signb;
            if (_data[y][x] == '@') {
                Node *temp = nullptr;
                return {temp, false};
            }
            if (drawing) {
                _data[y][x] = symbol;
            }
            steps--;
        }
    } else {
        while ((x != x1 || y != y1) && steps > 0) {
            f += B * signb;
            if (f > 0) {
                f -= A * signa;
                x -= signb;
            }
            y += signa;
            if (_data[y][x] == '@') {
                Node *temp = nullptr;
                return {temp, false};
            }
            if (drawing) {
                _data[y][x] = symbol;
            }
            steps--;
        }
    }
    if (_data[y][x] != 'S') {
        _data[y][x] = 'P';
    }
    Node *temp = new Node(x, y);
    return {temp, true};
}

Point *Field::take_random_point() const {
    int x = rand() % _cols;
    int y = rand() % _rows;

    if (_data[y][x] != '@') {
        return new Point(x, y);
    }
    take_random_point();
}

int Field::distance(Point *point, Node *node) {
    return (point->x - node->_x) * (point->x - node->_x) +
           (point->y - node->_y) * (point->y - node->_y);
}

int Field::distance(Node *node1, Node *node2) {
    return (node1->_x - node2->_x) * (node1->_x - node2->_x) +
           (node1->_y - node2->_y) * (node1->_y - node2->_y);
}

Node *Field::find_nearest(Point *point) {
    Node *min_node = _kdtree.nearest(point);
    _diff.push_back((float) _tree._nodes.size() / _kdtree._visited);
    return min_node;

}

Node *Field::steer(Point *point_rand, Node *node_nearest) {
    NodeAndBool steer_result = draw_path(node_nearest->_x, node_nearest->_y,
                                         point_rand->x, point_rand->y, false, '*');
    if (steer_result.ans) {
        Node *tmp = steer_result.node;
        node_nearest->add_child(tmp);
        _tree.addNode(tmp);
        _kdtree.insert(_kdtree._parent, tmp);
        tmp->parent = node_nearest;
        if (sqrt(distance(&end_point, tmp)) <= sample_step) {
            NodeAndBool steer_to_res = draw_path(tmp->_x, tmp->_y, end_point.x, end_point.y, false, '*');
            if(steer_to_res.ans) {
                _result_node = tmp;
                draw_path(tmp->_x, tmp->_y, end_point.x, end_point.y, true, '*');
            }
        }
        return tmp;

    }
    return nullptr;
}

void Field::rrt_algo(int iterations) {
    for (int k = 1; k < iterations; k++) {
        Point *random_point = take_random_point();
        Node *nearest_node = find_nearest(random_point);
        steer(random_point, nearest_node);
        if (_result_node != nullptr) {
            break;
        }
        delete random_point;
    }
}

void Field::draw_tree(Node *node) {
    if (node->_child.empty()) {
        return;
    }
    for (Node *child_node: node->_child) {
        draw_path(node->_x, node->_y, child_node->_x, child_node->_y, true, '*');
        draw_tree(child_node);
    }
}

void Field::read_parent(Node *node) {
    _distance += 1;
    _path_vector.push_back(node);
    if (node->parent) {
        draw_path(node->parent->_x, node->parent->_y, node->_x, node->_y, true, 'A');
        _data[node->_y][node->_x] = 'M';
        read_parent(node->parent);
    }
}

void Field::make_path_vector(Node *node) {
    _distance = 0;
    read_parent(node);
}