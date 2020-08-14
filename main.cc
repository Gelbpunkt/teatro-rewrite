#include <drogon/drogon.h>
#include <iostream>
int main() {
  // Set HTTP listener address and port
  drogon::app()
      .registerBeginningAdvice(
          []() { LOG_INFO << "Votehandler running on port 7666"; })
      .loadConfigFile("config.json")
      .run();
  return 0;
}
