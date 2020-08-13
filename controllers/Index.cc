#include "Index.h"
void Index::asyncHandleHttpRequest(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  // logic goes here
  auto resp = HttpResponse::newHttpResponse();
  resp->setStatusCode(k200OK);
  resp->setContentTypeCode(CT_TEXT_PLAIN);
  resp->setBody("1");
  callback(resp);
}
