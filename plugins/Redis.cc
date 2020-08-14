/**
 *
 *  Redis.cc
 *
 */

#include "Redis.h"
#include <cpp_redis/cpp_redis>

using namespace drogon;

void Redis::initAndStart(const Json::Value &config) {
  /// Initialize and start the plugin
  client.connect(config.get("host", "127.0.0.1").asString(),
                 config.get("port", 6379).asUInt());
}

void Redis::shutdown() {
  /// Shutdown the plugin
  client.disconnect();
}
