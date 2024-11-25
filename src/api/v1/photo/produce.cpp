#include "produce.hpp"

#include <iostream>
#include <ostream>
#include <userver/kafka/exceptions.hpp>

namespace kafka_produce {

SendStatus Produce(const kafka::Producer& producer, const RequestMessage& message) {
  try {
    producer.Send(message.topic, message.key, message.payload);
    return SendStatus::kSuccess;
  } catch (const kafka::SendException& ex) {
    std::cout << ex.what() << std::endl;
    return ex.IsRetryable() ? SendStatus::kErrorRetryable : SendStatus::kErrorNonRetryable;
  }
}

}  // namespace kafka_produce