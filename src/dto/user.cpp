#include "user.hpp"

namespace dto {

UserRegistrationDTO Parse(const userver::formats::json::Value& json,
                          userver::formats::parse::To<UserRegistrationDTO>) {
  return UserRegistrationDTO{
      json["name"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
  };
}

UserLoginDTO Parse(const userver::formats::json::Value& json,
                   userver::formats::parse::To<UserLoginDTO>) {
  return UserLoginDTO{
      json["name"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
  };
}

UserUpdateDTO Parse(const userver::formats::json::Value& json,
                    userver::formats::parse::To<UserUpdateDTO>) {
  return UserUpdateDTO{
      json["name"].As<std::optional<std::string>>(),
      json["password"].As<std::optional<std::string>>(),
  };
}

}  // namespace dto