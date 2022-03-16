
#include "client.h"
#include "server.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char** argv)
{
    if (false) // make false to run unit-tests
    {
        // std::string trx = "ali-hamed-1.5";
        // // int count { std::count(trx.begin(), trx.end(), '-') };
        // size_t first = trx.find('-');
        // size_t second = trx.find('-', first + 1);
        // std::cout << trx.substr(0, first) << std::endl;
        // std::cout << trx.substr(first + 1, second - first - 1) << std::endl;
        // std::cout << trx.substr(second + 1) << std::endl;

    } else {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret { RUN_ALL_TESTS() };
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}