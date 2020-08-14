#include "Vote.h"
#include <drogon/drogon.h>
#include <fmt/core.h>
#include <plugins/DmUser.h>
#include <plugins/Redis.h>
#include <random>
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

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, 10000);
  auto secret = distr(gen);

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
      fmt::format("UPDATE profile SET \"crates_{0}\"=\"crates_{0}\"+1 WHERE "
                  "\"user\"=$1;",
                  rarity),
      userId);

  // Get the Redis instance
  auto *redisClientPtr = drogon::app().getPlugin<Redis>();
  // Set the cooldown
  redisClientPtr->setUserTimer(userId);
  // Update cache
  redisClientPtr->updateUserCache(rarity, userId);

  redisClientPtr->commit();

  // DM the user
  auto *dmUserPtr = drogon::app().getPlugin<DmUser>();
  dmUserPtr->dmUser(rarity, userId);

  auto resp = HttpResponse::newHttpResponse();
  resp->setStatusCode(k200OK);
  resp->setContentTypeCode(CT_TEXT_PLAIN);
  resp->setBody("OK\r\n");
  callback(resp);
}
