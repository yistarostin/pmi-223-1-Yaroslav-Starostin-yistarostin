#include <iostream>

#include "controller.h"

int main(int argc, char** argv) {
    try {
        Controller().Run(argc, argv);
    } catch (const std::invalid_argument& e) {
        std::cerr << "ERROR:" << '\t' << e.what() << std::endl;
    }

    return 0;
}
