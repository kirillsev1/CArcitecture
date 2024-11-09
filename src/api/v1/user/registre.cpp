#include "registre.hpp"

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace user_create {

namespace {

class UserCreate final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-user-create";

  UserCreate(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db")
                .GetCluster()) {}

  userver::storages::postgres::ClusterPtr pg_cluster_;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    const auto& name = request.GetArg("name");
    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO hello_schema.users(name, count) VALUES($1, 1) "
        "ON CONFLICT (name) "
        "DO UPDATE SET count = users.count + 1 "
        "RETURNING users.count",
        name);

    return fmt::format("Hi {}, {}!\n", name, result.AsSingleRow<int>());
  }
};

}  // namespace

void AppendUserCreate(userver::components::ComponentList& component_list) {
  component_list.Append<user_create::UserCreate>();
}

}  // namespace user_create