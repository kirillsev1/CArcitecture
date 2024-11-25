#include "user.hpp"
#include <utils/jwt.hpp>

#include <userver/formats/serialize/common_containers.hpp>

namespace models {

userver::formats::json::Value Serialize(
    const User& user,
    userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder item;
  item["name"] = user.name;
  item["token"] = utils::jwt::GenerateJWT(user.id);
  return item.ExtractValue();
}

}  // namespace models