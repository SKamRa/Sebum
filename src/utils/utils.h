/*
 *
 * 
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include "./base64.hpp"

#include <iostream>
#include <vector>
#include <string>

namespace Utils {
    void printBanner();
    std::string concatenateVariables(std::vector<std::string> dataList);
}

#endif