#include "Vote.h"
#include <drogon/drogon.h>
#include <fmt/core.h>
#include <json/json.h>
#include <plugins/Redis.h>
#include <stdlib.h>

void Vote::asyncHandleHttpRequest(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  // logic goes here
  auto jsonPtr = req.get()->getJsonObject().get();
  if (!jsonPtr) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k400BadRequest);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("no JSON body was provided");
    callback(resp);
    return;
  }
  auto json = *jsonPtr;
  auto user = json["user"];
  if (!user) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k400BadRequest);
    resp->setContentTypeCode(CT_TEXT_PLAIN);
    resp->setBody("no user value was provided");
    callback(resp);
    return;
  }

  auto secret = rand() % 10000;

  std::string rarity;
  if (secret <= 10) {
    rarity = "legendary";
  } else if (secret <= 100) {
    rarity = "magic";
  } else if (secret <= 500) {
    rarity = "rare";
  } else if (secret <= 1000) {
    rarity = "uncommon";
  } else {
    rarity = "common";
  }

  auto userId = std::stoul(user.asString());

  // Get a DB connection and run the query, not waiting for the result
  auto clientPtr = drogon::app().getFastDbClient();
  clientPtr->execSqlAsyncFuture(
      fmt::format(
          "UPDATE profile SET \"crates_{}\"=\"crates_{}\"+1 WHERE \"user\"=$1;",
          rarity, rarity),
      userId);

  // Get the Redis instance
  auto *redisClientPtr = drogon::app().getPlugin<Redis>();
  // Set the cooldown
  redisClientPtr->client.setex(fmt::format("cd:{}:vote", userId), 43200,
                               "vote");
  auto redisKey = fmt::format("profilecache:{}", userId);
  // Get the cache value
  redisClientPtr->client.get(redisKey, [redisClientPtr, redisKey,
                                        rarity](cpp_redis::reply &reply) {
    auto value = reply.as_string();
    if (value == "")
      return;
    Json::Value root;
    JSONCPP_STRING err;
    // Parse it as JSON
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    reader->parse(value.c_str(), value.c_str() + value.length(), &root, &err);
    auto key = fmt::format("crates_{}", rarity);
    // It is parsed into "root"
    root[key] = root[key].asInt64() + 1;
    // Write it back to a string
    Json::StreamWriterBuilder writeBuilder;
    writeBuilder.settings_["indentation"] = "";
    std::string jsonFile = Json::writeString(writeBuilder, root);
    redisClientPtr->client.set(redisKey, jsonFile);
    redisClientPtr->client.commit();
  });

  redisClientPtr->client.commit();

  auto resp = HttpResponse::newHttpResponse();
  resp->setStatusCode(k200OK);
  resp->setContentTypeCode(CT_TEXT_PLAIN);
  resp->setBody("OK\r\n");
  callback(resp);
}
