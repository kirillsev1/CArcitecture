#pragma once

#include <optional>
#include <string>
#include <tuple>

#include <userver/formats/json/value_builder.hpp>
#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include "db/types.hpp"

namespace models {

using UserId = std::string;
struct User final {
  UserId id;
  std::string name;
  std::string password;
  bool is_admin;

  auto Introspect() {
    return std::tie(id, name, password, is_admin);
  }
};

userver::formats::json::Value Serialize(
    const User& user,
    userver::formats::serialize::To<userver::formats::json::Value>);

}  // namespace models

namespace userver::storages::postgres::io {

template <>
struct CppToUserPg<models::User> {
  static constexpr v2_5_rc::storages::postgres::DBTypeName postgres_name{sql::types::kUser.data()};
};

}  // namespace userver::storages::postgres::io