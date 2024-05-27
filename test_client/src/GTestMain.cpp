#include <stdio.h>
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <iostream>

// namespace fs = std::filesystem;

// Main function for GTest
GTEST_API_ int main(int argc, char **argv)
{

    // std::string path = "/path/to/directory";
    // for (const auto & entry : fs::directory_iterator(path))
    //     std::cout << entry.path() << std::endl;

    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

