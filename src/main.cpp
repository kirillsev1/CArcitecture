#include <api/v1/photo/consumer.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>

#include "api/v1/hello/hello.hpp"
#include "api/v1/photo/load.hpp"
#include "api/v1/user/register.hpp"
#include "api/v1/user/info.hpp"
#include "userver/utils/daemon_run.hpp"

#include <userver/storages/secdist/component.hpp>
#include <userver/storages/secdist/provider_component.hpp>

#include <userver/kafka/producer_component.hpp>
#include <userver/kafka/consumer_component.hpp>

int main(const int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<kafka_produce::ProducerHandler>()
                            .Append<kafka_consume::ConsumerHandler>()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::server::handlers::TestsControl>()
                            .Append<userver::components::Postgres>("postgres-db")
                            .Append<userver::kafka::ProducerComponent>("kafka-producer")
                            .Append<userver::kafka::ConsumerComponent>("kafka-consumer")
                            .Append<userver::clients::dns::Component>()
                            .Append<userver::components::Secdist>()
                            .Append<userver::components::DefaultSecdistProvider>()
  ;

  service_template::AppendHello(component_list);
  registration::AppendRegister(component_list);
  info::AppendInfo(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
