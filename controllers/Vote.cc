#include "Vote.h"
void Vote::asyncHandleHttpRequest(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  // logic goes here
  auto resp = HttpResponse::newHttpResponse();
  resp->setStatusCode(k200OK);
  resp->setContentTypeCode(CT_TEXT_PLAIN);
  resp->setBody("2");
  callback(resp);
}
