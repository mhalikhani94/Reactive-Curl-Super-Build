#include "ethereum-api.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

EthereumApi::EthereumApi(HttpClient* client) : http_client(client)
{
}

EthereumApi::~EthereumApi()
{
}

rxcpp::observable<std::vector<ns::RawUtxo>> EthereumApi::get_utxos_of_addresses(
        std::vector<std::string> address_list, NetworkType network_type)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Post(network_type.yoroi_api_root + "/txs/utxoForAddresses")
            .add_header("Content-Type: application/json; charset=utf-8")
            .process_response([&](const HttpResponse& result)
            {
                if (result.is_ok())
                {
                    try
                    {
                        out.on_next(result);
                        out.on_completed();
                    } catch ( ... ) {
                        out.on_error(std::current_exception());
                    }
                } else {
                    out.on_next(result);
                    out.on_completed();
                }
            })
            .send_request();
        }
        catch(...)
        {
            out.on_error(std::current_exception());
        }
        
    }).subscribe_on(worker_thread)
    .map([&](const HttpResponse& result)
    {
        //TODO: parse result.body to get result!
        std::vector<ns::RawUtxo> results{};
        return results;
    });
}