#include "consume.hpp"

#include <userver/formats/json/value_builder.hpp>

namespace kafka_consume {
formats::json::Value Consume(kafka::MessageBatchView messages) {
  formats::json::ValueBuilder builder;
  for (const auto& message : messages) {
    if (!message.GetTimestamp().has_value()) {
      continue;
    }
    builder["key"] = message.GetKey();
    builder["payload"] = message.GetPayload();
  }
  return builder.ExtractValue();
}
}  // namespace kafka_consume