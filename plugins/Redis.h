/**
 *
 *  Redis.h
 *
 */

#pragma once

#include <cpp_redis/cpp_redis>
#include <drogon/plugins/Plugin.h>

class Redis : public drogon::Plugin<Redis> {
public:
  cpp_redis::client client;
  Redis() {}
  /// This method must be called by drogon to initialize and start the plugin.
  /// It must be implemented by the user.
  virtual void initAndStart(const Json::Value &config) override;

  /// This method must be called by drogon to shutdown the plugin.
  /// It must be implemented by the user.
  virtual void shutdown() override;
};
