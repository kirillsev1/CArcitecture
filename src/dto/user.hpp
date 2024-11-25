#pragma once

#include <string>
#include <tuple>
#include <userver/formats/json.hpp>
#include <userver/formats/parse/common_containers.hpp>

namespace dto {

struct UserRegistrationDTO {
  std::optional<std::string> name;
  std::optional<std::string> password;
};

struct UserLoginDTO {
  std::optional<std::string> name;
  std::optional<std::string> password;
};

struct UserUpdateDTO {
  std::optional<std::string> name;
  std::optional<std::string> password;
};

UserRegistrationDTO Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UserRegistrationDTO>);

UserLoginDTO Parse(const userver::formats::json::Value& json,
                   userver::formats::parse::To<UserLoginDTO>);

UserUpdateDTO Parse(const userver::formats::json::Value& json,
                    userver::formats::parse::To<UserUpdateDTO>);
}  // namespace dto