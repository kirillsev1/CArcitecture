#pragma once

#include <string>

#include <service_example.pb.h>

#include <userver/kafka/producer.hpp>
#include <userver/formats/json/value.hpp>

#include <userver/utest/using_namespace_userver.hpp>

namespace kafka_produce {

enum class SendStatus {
    kSuccess,
    kErrorRetryable,
    kErrorNonRetryable,
};

struct RequestMessage {
    std::string topic;
    std::string key;
    formats::json::Value payload;
};

SendStatus Produce(const kafka::Producer& producer, const service::example::api::PhotoMetaRequest& message, const std::string& topic);

}  // namespace kafka_produce