/*
*
* 
*
*/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <sys/stat.h>
#include <chrono>

// clipboard management
#include <limits.h>
#include <X11/Xlib.h>

namespace System {
  namespace {
    char *PrintSelection(Display *display, Window window, const char *bufname, const char *fmtname);
  }
  char *getClipboardContent();

  std::string getOsName();
  std::string getOsArchitecture();
  std::string getKernelVersionByFile();
  std::string getOsHostname();
  std::string getHostsFile();
  std::string getOsInfos();
  std::string getCurrentUserInfos();
  std::string getAllUsers();

  std::unordered_map<std::string, std::vector<std::string>> getBasicDirectoryContent();
  void showBasicDirectoryContent(std::unordered_map<std::string, std::vector<std::string>>);
};

#endif