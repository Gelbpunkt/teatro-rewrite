/**
 *
 *  DmUser.cc
 *
 */

#include "DmUser.h"
#include <fmt/core.h>

using namespace drogon;

void DmUser::initAndStart(const Json::Value &config) {
  /// Initialize and start the plugin
  this->token = fmt::format("Bot {}", config["token"].asString());
  this->client = HttpClient::newHttpClient("https://discord.com");
}

void DmUser::dmUser(std::string rarity, unsigned long userId) {
  auto httpReq = HttpRequest::newHttpRequest();
  httpReq->setMethod(Post);
  httpReq->setPath("/api/v7/users/@me/channels");
  httpReq->setContentTypeCode(CT_APPLICATION_JSON);
  httpReq->setBody(fmt::format("{{\"recipient_id\": \"{}\"}}", userId));
  httpReq->addHeader("Authorization", this->token);
  httpReq->addHeader("User-Agent", "DiscordVoteHandlerC++ (0.1.0) IdleRPG");
  this->client->sendRequest(httpReq, [this, rarity,
                                      userId](ReqResult result,
                                              const HttpResponsePtr &response) {
    auto body = *response->getJsonObject();
    std::string channelId = body["id"].asString();

    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Post);
    req->setPath(fmt::format("/api/v7/channels/{}/messages", channelId));
    req->setContentTypeCode(CT_APPLICATION_JSON);
    req->setBody(fmt::format("{{\"content\":\"Thank you for the upvote! "
                             "You received a {} crate!\"}}",
                             rarity));
    req->addHeader("Authorization", this->token);
    req->addHeader("User-Agent", "DiscordVoteHandlerC++ (0.1.0) IdleRPG");
    this->client->sendRequest(
        req, [userId](ReqResult result, const HttpResponsePtr &response) {
          LOG_INFO << fmt::format("Done processing a vote for user {}", userId);
        });
  });
}

void DmUser::shutdown() {
  /// Shutdown the plugin
}
