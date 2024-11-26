#include "load.hpp"
 
#include <userver/kafka/producer_component.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
 
#include <userver/formats/json/inline.hpp>
#include <userver/formats/json/value.hpp>
 
#include "produce.hpp"
 
namespace kafka_produce {
 
service::example::api::PhotoMetaRequest parse(const formats::json::Value& doc) {
  service::example::api::PhotoMetaRequest request_message;
  request_message.set_key(doc["key"].As<std::string>());
  request_message.set_filename(doc["payload"]["filename"].As<std::string>());
  request_message.set_im_size(doc["payload"]["im_size"].As<std::int32_t>());
  return request_message;
}
 
namespace {
 
constexpr std::string_view kReqTopicFieldName = "topic";
constexpr std::string_view kReqKeyFieldName = "key";
constexpr std::string_view kReqPayloadFieldName = "payload";
 
constexpr std::string_view kErrorMembersNotSet = R"(
  {
    "error": "Expected body has `topic`, `key` and `payload` fields"
  }
)";
 
bool IsCorrectRequest(const formats::json::Value& request_json) {
    return request_json.HasMember(kReqPayloadFieldName) && request_json.HasMember(kReqTopicFieldName) &&
           request_json.HasMember(kReqKeyFieldName);
}
 
}  // namespace
 
ProducerHandler::ProducerHandler(const components::ComponentConfig& config, const components::ComponentContext& context)
    : server::handlers::HttpHandlerJsonBase{config, context},
      producer_{context.FindComponent<kafka::ProducerComponent>().GetProducer()} {}
 
formats::json::Value ProducerHandler::HandleRequestJsonThrow(
    const server::http::HttpRequest& request,
    const formats::json::Value& request_json,
    server::request::RequestContext& /*context*/
) const {
    if (!IsCorrectRequest(request_json)) {
        request.SetResponseStatus(server::http::HttpStatus::kBadRequest);
 
        return formats::json::FromString(kErrorMembersNotSet);
    }
 
    const auto message = parse(request_json);
    switch (Produce(producer_, message, request_json["topic"].As<std::string>())) {
        case SendStatus::kSuccess:
            return formats::json::MakeObject("message", "Message send successfully");
        case SendStatus::kErrorRetryable:
            request.SetResponseStatus(server::http::HttpStatus::TooManyRequests);
            return formats::json::MakeObject("error", "Retry later");
        case SendStatus::kErrorNonRetryable:
            request.SetResponseStatus(server::http::HttpStatus::kBadRequest);
            return formats::json::MakeObject("error", "Bad request");
    }
    UINVARIANT(false, "Unknown produce status");
}
 
}  // namespace kafka_produce