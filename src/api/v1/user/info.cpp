#include "info.hpp"

#include <userver/components/component.hpp>
#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace info {

namespace {

class Info final : public userver::server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-user-info";
  userver::storages::postgres::ClusterPtr pg_cluster_;

  Info(const userver::components::ComponentConfig& config,
       const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    const auto& name = request.GetArg("name");
    const auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "SELECT * FROM auth_schema.users WHERE name = $1", name);
    if (result.IsEmpty()) {
      request.SetResponseStatus(userver::server::http::HttpStatus::NotFound);
    };
    return "";
  }
};

}  // namespace

void AppendInfo(userver::components::ComponentList& component_list) {
  component_list.Append<info::Info>();
}

}  // namespace info