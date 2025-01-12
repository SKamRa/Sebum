#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char* argv[]) {
  std::vector<std::string> myVector {};

  myVector.push_back("test1");
  myVector.push_back("test2");
  myVector.push_back("test3");

  std::unordered_map< std::string, std::vector<std::string> > myHashMap {};

  for (int i {0}; i < myVector.size(); i++) {
    std::cout << myVector[i] << std::endl;
    myHashMap.insert(
      std::pair< std::string, std::vector<std::string> >(myVector[i], std::vector<std::string>())
    );
    
    for (int j {0}; j < myVector.size(); j++) {
      if (j != i) {
        myHashMap[myVector[i]].push_back(myVector[j]);
      }
    }
  }

  std::string k {};
  std::vector<std::string> v {};
  for (auto b {myHashMap.begin()}; b != myHashMap.end(); b++) {
    k = (*b).first;
    v = myHashMap[(*b).first];
    std::cout << k << std::endl;

    for (int y {0}; y < v.size(); y++) {
      std::cout << "  " << v[y] << std::endl;
    }
  }
  
  return 0;
}