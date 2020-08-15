/**
 *
 *  Redis.cc
 *
 */

#include "Redis.h"
#include <cpp_redis/cpp_redis>
#include <fmt/core.h>
#include <json/json.h>

using namespace drogon;

void Redis::initAndStart(const Json::Value &config) {
  /// Initialize and start the plugin
  this->client.connect(config.get("host", "127.0.0.1").asString(),
                       config.get("port", 6379).asUInt());
}

void Redis::setUserTimer(unsigned long userId) {
  this->client.setex(fmt::format("cd:{}:vote", userId), 43200, "vote");
}

void Redis::updateUserCache(std::string rarity, unsigned long userId) {
  auto redisKey = fmt::format("profilecache:{}", userId);
  LOG_INFO << fmt::format("checking {}", redisKey);
  // Get the cache value
  this->client.get(redisKey, [this, redisKey, rarity](cpp_redis::reply &reply) {
    auto value = reply.as_string();
    LOG_INFO << fmt::format("got redis result: {}", value);
    if (value == "")
      return;
    Json::Value root;
    JSONCPP_STRING err;
    // Parse it as JSON
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    reader->parse(value.c_str(), value.c_str() + value.length(), &root, &err);
    auto key = fmt::format("crates_{}", rarity);
    std::cout << root << std::endl;
    // It is parsed into "root"
    root[key] = root[key].asInt64() + 1;
    // Write it back to a string
    Json::StreamWriterBuilder writeBuilder;
    writeBuilder.settings_["indentation"] = "";
    std::string jsonFile = Json::writeString(writeBuilder, root);
    LOG_INFO << "setting value";
    std::cout << jsonFile << std::endl;
    this->client.set(redisKey, jsonFile);
    LOG_INFO << "committing";
    this->commit();
  });
}

void Redis::commit() { this->client.commit(); }

void Redis::shutdown() {
  /// Shutdown the plugin
  client.disconnect();
}
