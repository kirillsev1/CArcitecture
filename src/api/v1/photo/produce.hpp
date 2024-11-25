#pragma once

#include <string>

#include <userver/kafka/producer.hpp>

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
    std::string payload;
};

SendStatus Produce(const kafka::Producer& producer, const RequestMessage& message);

}  // namespace kafka_produce