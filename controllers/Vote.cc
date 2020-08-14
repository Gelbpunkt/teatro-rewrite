#include "Vote.h"
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

  std::cout << user << std::endl;

  auto resp = HttpResponse::newHttpResponse();
  resp->setStatusCode(k200OK);
  resp->setContentTypeCode(CT_TEXT_PLAIN);
  resp->setBody("2");
  callback(resp);
}
