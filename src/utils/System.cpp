/*
*
* 
*
*/

#include "System.h"

namespace System {
  // CLIPBOARD
  namespace {    
    char* PrintSelection(Display *display, Window window, const char *bufname, const char *fmtname) {
      char *clipboardContent {NULL};
      char *result;
      unsigned long ressize, restail;
      int resbits;
      Atom bufid = XInternAtom(display, bufname, False),
        fmtid = XInternAtom(display, fmtname, False),
        propid = XInternAtom(display, "XSEL_DATA", False),
        incrid = XInternAtom(display, "INCR", False);
      XEvent event;

      XConvertSelection(display, bufid, fmtid, propid, window, CurrentTime);
      do {
        XNextEvent(display, &event);
      } while (event.type != SelectionNotify || event.xselection.selection != bufid);

      if (event.xselection.property)
      {
        XGetWindowProperty(display, window, propid, 0, LONG_MAX/4, False, AnyPropertyType,
        &fmtid, &resbits, &ressize, &restail, (unsigned char**)&result);

        if (fmtid == incrid) {
          printf("Buffer is too large and INCR reading is not implemented yet.\n");
        }
        else {
          // Allocate memory for clipboard content (including space for null-terminator)
          clipboardContent = (char*)malloc(ressize + 1);
          if (clipboardContent == NULL) {
            printf("Memory allocation failed!\n");
            XFree(result);
            return NULL;
          }

          // Copy the content from result to clipboardContent
          std::memcpy(clipboardContent, result, ressize);

          // Null-terminate the string
          clipboardContent[ressize] = '\0';
        }

        // Free result after usage
        XFree(result);
      } else {
        printf("Failed to retrieve clipboard content.\n");
      }

      return clipboardContent;
    }
  }

  char* getClipboardContent() {

    Display *display = XOpenDisplay(NULL);
    unsigned long color = BlackPixel(display, DefaultScreen(display));
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0,0, 1,1, 0, color, color);

    char *clipboardContent = PrintSelection(display, window, "CLIPBOARD", "UTF8_STRING");
    if (clipboardContent == "") {
      clipboardContent = PrintSelection(display, window, "CLIPBOARD", "STRING");
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return clipboardContent;
  }

  std::string getOsName() {
    std::string osName {""};

    #ifdef _WIN32
    osName = "Windows 32-bit or Windows 64-bit";
    #elif _WIN64
    osName = "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    osName = "Mac OSX";
    #elif __linux__
    osName = "Linux";
    #elif __FreeBSD__
    osName "FreeBSD";
    #elif __OpenBSD__
    osName "OpenBSD";
    #elif __ANDROID__
    osName = "Android";
    #elif __unix || __unix__
    osName = "Unix";
    #else
    osName = "UndefinedOS";
    #endif

    return osName;
  }

  std::string getOsArchitecture() {
    std::string osArch {""};

    // 64-bits architectures
    #ifdef __aarch64__
    osArch = "ARM64";
    #elif __amd64__ || __amd64 || __x86_64__ || __x86_64
    osArch = "AMD64";
    // 32-bits architectures
    #elif i386 || __i386 || __i386__ || _M_I86
    osArch = "Intel x86";
    #else
    osArch = "UndefinedArch";
    #endif

    return osArch;
  }

  std::string getKernelVersionByFile() {
    std::string osKernelV {""};

    std::ifstream inf1 {"/proc/version"};
    std::ifstream inf2 {"/etc/os-release"};

    if (inf1) {
      std::stringstream version_buffer {""};
      version_buffer << inf1.rdbuf();
      osKernelV += version_buffer.str() + '\n';
    }

    if (inf2) {
      std::stringstream release_buffer {""};
      release_buffer << inf2.rdbuf();
      osKernelV += release_buffer.str();
    }

    inf1.close();
    inf2.close();

    return osKernelV;
  }

  std::string getOsHostname() {
    std::string osHostname {""};

    // get with /etc/hostname
    std::ifstream inf {"/etc/hostname"};

    if (inf) {
      std::stringstream buffer {""};
      buffer << inf.rdbuf();
      osHostname = buffer.str();
    }
    
    // get with en env
    if (osHostname == "") {
      osHostname = std::getenv("HOSTNAME");
    }

    return osHostname;
  }

  std::string getHostsFile() {
    std::string osHosts {""};

    std::ifstream inf {"/etc/hosts"};

    if (inf) {
      std::stringstream buffer {""};
      buffer << inf.rdbuf();
      osHosts = buffer.str();
    }

    return osHosts;
  }

  std::string getOsInfos() {
    std::string osInfos {getKernelVersionByFile()};

    // use the function if /proc/version is not accessible
    if (osInfos == "") {
      std::string osName {getOsName()};
      std::string osArch {getOsArchitecture()};
      osInfos += osName + ' ' + osArch;
      // format the output prettier
    } else {
      // format the output prettier
    }

    return osInfos;
  }

  std::string getCurrentUserInfos() {
    std::string currentUserInfos {""};

    /*
    *
    * TO DO
    *
    */

    return currentUserInfos;
  }

  std::string getAllUsers() {
    std::string allUsers {""};

    std::ifstream inf {"/etc/passwd"};

    if (inf) {
      std::stringstream buffer {""};
      buffer << inf.rdbuf();
      allUsers = buffer.str();
    }

    return allUsers;
  }

  std::unordered_map<std::string, std::vector<std::string>> getBasicDirectoryContent() {
    std::unordered_map<std::string, std::vector<std::string>> directoriesContent {};

    std::vector<std::string> rootDirectories {"root", "home", "mnt", "srv", "tmp", "var", "media", "opt", "usr"};
    std::vector<std::string> content {};

    for (std::string d : rootDirectories) {
      directoriesContent.insert(
        std::pair< std::string, std::vector<std::string> >(d, content)
      );
    }

    std::string path {""};
    struct stat status;
    for (std::string directory : rootDirectories) {
      path = '/' + directory;
      try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
          std::filesystem::path outfilename = entry.path();
          std::string outfilename_str = outfilename.string();
          const char* path = outfilename_str.c_str();
            
          if (stat(path, &status) == 0) { // directory exists
            directoriesContent[directory].push_back(outfilename_str);
          }
        }
      } catch (std::filesystem::filesystem_error const& err) {
        directoriesContent[directory].push_back(err.what());
      }
    }
    
    return directoriesContent;
  }

  void showBasicDirectoryContent(std::unordered_map<std::string, std::vector<std::string>> directoriesContent) {
    std::string k {""};
    std::vector<std::string> v {};

    for (auto item = directoriesContent.begin(); item != directoriesContent.end(); item++) {
      k = (*item).first;
      v = directoriesContent[k];

      std::cout << '/' << (*item).first << '\n';

      for (int j {0}; j < v.size(); j++) {
        std::cout << "  -> " << v[j] << '\n';
      }
    }
  }
}

// Regarder pour quand le clipboard est vide il y a une std::logic_error
