#include "cardano-api.hpp"
#include "cardano-parsers.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

CardanoApi::CardanoApi(HttpClient* client) : http_client(client)
{
}

CardanoApi::~CardanoApi()
{
}

rxcpp::observable<std::vector<ns::RawUtxo>> CardanoApi::get_utxos_of_addresses(
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

rxcpp::observable<long> CardanoApi::get_slot_num(NetworkType network_type)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(network_type.yoroi_api_root + "/blocks/latest")
            .add_header("project_id", network_type.block_frost_project_id)
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
<<<<<<< HEAD
        return CardanoParser::parse_get_slot_num_response(result);
    });
}

rxcpp::observable<std::string> CardanoApi::submit_signed_tx(std::vector<uint8_t> signed_transaction
=======
        //TODO: parse result.body to get result!
        long long_result{};
        return long_result;
    });
}

rxcpp::observable<std::string> submit_signed_tx(std::vector<uint8_t> signed_transaction
>>>>>>> 30648289464473a3d0012b5942e12245054fcf07
    , NetworkType network_type)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(network_type.yoroi_api_root + "/tx/submit")
            .add_header("Content-Type","application/cbor")
            .add_header("project_id", network_type.block_frost_project_id)
            .add_body(signed_transaction)
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
        std::string string_result{};
        return string_result;
    });
}

rxcpp::observable<std::list<SpentTransaction>> CardanoApi::retrieve_transactions_desc(std::string address, long page
    , int offset, NetworkType network_type)
{
        auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(network_type.yoroi_api_root + "/tx/submit")
            .add_header("Content-Type","application/cbor")
            .add_header("project_id", network_type.block_frost_project_id)
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
    .concat_map([&](const HttpResponse& result)
    {
        //TODO: parse result.body to get result!
        std::list<std::string> string_list_results{};

        return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out1)
        {
            for(const auto& str : string_list_results)
            {
                try
                {
                    http_client->build()->Get(network_type.yoroi_api_root + "/tx/submit")
                    .add_header("Content-Type","application/cbor")
                    .add_header("project_id", network_type.block_frost_project_id)
                    .process_response([&](const HttpResponse& result)
                    {
                        if (result.is_ok())
                        {
                            try
                            {
                                out1.on_next(result);
                                out1.on_completed();
                            } catch ( ... ) {
                                out1.on_error(std::current_exception());
                            }
                        } else {
                            out1.on_next(result);
                            out1.on_completed();
                        }
                    })
                    .send_request();
                }
                catch(...)
                {
                    out1.on_error(std::current_exception());
                }
            }
            
        }).subscribe_on(worker_thread)
        .map([&](const HttpResponse& result)
        {
            //TODO: parse result.body to get result!
            std::list<SpentTransaction> spent_transaction_list{};
            return spent_transaction_list;
        });
    });
}