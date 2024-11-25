#include "register.hpp"

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/kafka/consumer_component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include <userver/clients/http/response.hpp>

namespace registration {

    namespace {

        class Register final : public userver::server::handlers::HttpHandlerBase {
        public:
            static constexpr std::string_view kName = "handler-user-register";
            userver::storages::postgres::ClusterPtr pg_cluster_;

            Register(const userver::components::ComponentConfig& config,
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
                const auto& password = request.GetArg("password");
                const auto result = pg_cluster_->Execute(
                    userver::storages::postgres::ClusterHostType::kMaster,
                    "INSERT INTO auth_schema.users(name, password) VALUES($1, $2) "
                    "ON CONFLICT (name) "
                    "DO NOTHING "
                    "RETURNING users.id",
                    name, password);
                auto test = userver::clients::http::Response();
                return fmt::format("Hi {}!\n", name);
            }
        };

    }  // namespace

    void AppendRegister(userver::components::ComponentList& component_list) {
        component_list.Append<registration::Register>();
    }

}  // namespace registration