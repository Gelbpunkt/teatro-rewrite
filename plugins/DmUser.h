/**
 *
 *  DmUser.h
 *
 */

#pragma once

#include <drogon/drogon.h>
#include <drogon/plugins/Plugin.h>

class DmUser : public drogon::Plugin<DmUser> {
public:
  drogon::HttpClientPtr client;
  std::string token;
  DmUser() {}
  /// This method must be called by drogon to initialize and start the plugin.
  /// It must be implemented by the user.
  virtual void initAndStart(const Json::Value &config) override;

  virtual void dmUser(std::string rarity, unsigned long userId);

  /// This method must be called by drogon to shutdown the plugin.
  /// It must be implemented by the user.
  virtual void shutdown() override;
};
