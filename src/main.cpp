#include <userver/components/minimal_server_component_list.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/clients/dns/component.hpp>

#include "api/v1/hello/hello.hpp"
#include "api/v1/user/registre.hpp"
#include "userver/utils/daemon_run.hpp"

int main(const int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::server::handlers::TestsControl>()
                            .Append<userver::components::Postgres>("postgres-db")
                            .Append<userver::clients::dns::Component>();

  service_template::AppendHello(component_list);
  user_create::AppendUserCreate(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
