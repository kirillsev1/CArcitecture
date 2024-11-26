#include <userver/kafka/message.hpp>

#include "consumer.hpp"

#include <iostream>
#include <userver/components/component_context.hpp>
#include <userver/kafka/consumer_component.hpp>

#include "consume.hpp"


namespace kafka_consume {
ConsumerHandler::ConsumerHandler(const components::ComponentConfig& config,
                                 const components::ComponentContext& context)
  : components::ComponentBase{config, context},
    consumer_{context.FindComponent<kafka::ConsumerComponent>().GetConsumer()} {
  consumer_.Start([this](kafka::MessageBatchView messages) {
    auto result_json = formats::json::ToString(Consume(messages));
    std::cout << result_json << std::endl;
    consumer_.AsyncCommit();
  });
}
}