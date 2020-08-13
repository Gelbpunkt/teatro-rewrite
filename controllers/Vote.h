#pragma once
#include <drogon/HttpSimpleController.h>
using namespace drogon;
class Vote : public drogon::HttpSimpleController<Vote> {
public:
  virtual void asyncHandleHttpRequest(
      const HttpRequestPtr &req,
      std::function<void(const HttpResponsePtr &)> &&callback) override;
  PATH_LIST_BEGIN
  // list path definitions here;
  // PATH_ADD("/path","filter1","filter2",HttpMethod1,HttpMethod2...);
  PATH_ADD("/", Post);
  PATH_LIST_END
};
