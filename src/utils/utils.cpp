/*
 *
 * This file handles all the misc content like printing a banner or other shit
 *
 */

#include "utils.h"


namespace Utils {
    void printBanner() {
        
    };

    std::string concatenateVariables(std::vector<std::string> dataList) {
        std::string concatDatas {""};

        for (int i {0}; i < dataList.size(); i++) {
            concatDatas += base64::to_base64(dataList[i] + "\n===================================================\n");
        }

        return concatDatas;
    }

}