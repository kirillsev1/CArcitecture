#include "consume.hpp"

#include <userver/formats/json/value_builder.hpp>
#include <userver/ugrpc/client/rpc.hpp>

namespace kafka_consume {
formats::json::Value Consume(kafka::MessageBatchView messages) {
  formats::json::ValueBuilder builder;
  for (const auto& message : messages) {
    if (!message.GetTimestamp().has_value()) {
      continue;
    }

    service::example::api::PhotoMetaRequest request;
    request.ParseFromString(static_cast<std::string>(message.GetPayload()));

    builder["key"] = request.key();
    builder["filename"] = request.filename();
    builder["im_size"] = request.im_size();
  }
  return builder.ExtractValue();
}
}  // namespace kafka_consume