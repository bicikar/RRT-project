#include <iostream>
#include "../include/field.h"

int main(int argc, char *argv[]) {
    if (argc < 3){
        throw std::invalid_argument("You should input at least 2 arguments: iterations and file name");
    }
    int iterations = std::atoi(argv[1]);
    std::string map_name = argv[2];
    int step = 0;
    if (argc > 3) {
        step = std::atoi(argv[3]);
    }
    Field field(map_name, step);
    field.rrt_algo(iterations);
    field.draw_tree(field.get_root());
    field.print("OUTPUT.txt");
}
