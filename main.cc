#include <drogon/drogon.h>
#include <iostream>
int main() {
  // Set HTTP listener address and port
  drogon::app().addListener("0.0.0.0", 7666);
  // Load config file
  // drogon::app().loadConfigFile("../config.json");
  // Run HTTP framework,the method will block in the internal event loop
  std::cout << "Votehandler running on port 7666" << std::endl;
  drogon::app().run();
  return 0;
}
