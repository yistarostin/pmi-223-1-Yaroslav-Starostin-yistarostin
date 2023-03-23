#include <iostream>

#include "controller.h"

int main(int argc, char** argv) {
    //std::cout << argc << std::endl;
    Controller().Run(argc, argv);
    return 0;
}
