#include <iostream>

#include "ini_parser.h"


int main() {
    {
        ini::document ini_file("../input/example.ini");
        auto value = ini_file.get_value<int>("Section1.var1");
        std::cout << "Section1.var1 = " << value << std::endl;

        std::cout << std::endl;
        ini_file.print_document(std::cout);
    }
    {
        ini::document ini_file("../input/3dsMax.ini");
        std::cout << std::endl;
        ini_file.print_document(std::cout);
    }
    return 0;
}
