#pragma once

#include <userver/formats/json/value_builder.hpp>
#include <userver/kafka/message.hpp>

#include <service_example.pb.h>
#include <userver/utest/using_namespace_userver.hpp>
 
namespace kafka_consume {
    formats::json::Value Consume(kafka::MessageBatchView messages);
 
}  // namespace kafka_sample