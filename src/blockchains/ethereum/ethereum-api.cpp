#include "ethereum-api.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

EthereumApi::EthereumApi(HttpClient* client, EtherNetworks ether_network) : http_client(client)
, api_key("593TI6PRCQB4P85FGPCF534MTZMF3TS99A")
{
    switch (ether_network)
    {
    case EtherNetworks::kMainnet:
        base_url = "https://apis.ankr.com/9f35837baca04df08e72110a55aefe39/66ec4ea710b8b59b0926c705380eef52/eth/fast/main";
        break;
    case EtherNetworks::kRopsten:
        base_url = "https://apis.ankr.com/adc40ffef9a34b8a9b956f7af16301c2/66ec4ea710b8b59b0926c705380eef52/eth/fast/ropsten";
        break;
    case EtherNetworks::kRinkeby:
        base_url = "https://apis.ankr.com/6eb470e58c2847c4a23b138a76d2d613/66ec4ea710b8b59b0926c705380eef52/eth/fast/rinkeby";
        break;
    case EtherNetworks::kGoerli:
        base_url = "https://apis.ankr.com/897ae9db18ab441f9a490ec9b2701e78/66ec4ea710b8b59b0926c705380eef52/eth/fast/goerli";
        break;
    case EtherNetworks::kKovan:
        base_url = "";
        break;
    }
}

EthereumApi::~EthereumApi()
{
}

rxcpp::observable<BigInteger> EthereumApi::get_last_block_base_fee_per_gas()
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            json json_body{};
            json_body["jsonrpc"] = "2.0";
            json_body["method"] = "eth_getBlockByNumber";
            json_body["params"] = {"latest",false};
            json_body["id"] = 0;

            http_client->build()->Post(base_url)
            .add_json_body(json_body.dump())
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
        //TODO: parse result.body to get balance!
        BigInteger big_integer_result{};
        return big_integer_result;
    });
}

rxcpp::observable<BigInteger> EthereumApi::retrieve_address_balance(std::string address)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Post(base_url)
            .add_header("module","account")
            .add_header("action","balance")
            .add_header("address",address)
            .add_header("tag","latest")
            .add_header("apikey",api_key)
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
        //TODO: parse result.body to get balance!
        BigInteger big_integer_result{};
        return big_integer_result;
    });
}

rxcpp::observable<BigInteger> EthereumApi::get_gas_price()
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Post(base_url)
            .add_header("module","proxy")
            .add_header("action","eth_gasPrice")
            .add_header("tag","latest")
            .add_header("apikey",api_key)
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
        //TODO: parse result.body to get balance!
        BigInteger big_integer_result{};
        return big_integer_result;
    });
}

rxcpp::observable<BigInteger> EthereumApi::estimate_gas_units(std::string from, std::string to, BigInteger gas_price
        , BigInteger value, std::string data)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Post(base_url)
            .add_header("module", "proxy")
            .add_header("action", "eth_estimateGas")
            .add_header("data", data)
            .add_header("to", to)
            .add_header("from", from)
            .add_header("value", value.to_string())
            .add_header("gasPrice", gas_price.to_string())
            .add_header("apikey", api_key)
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
        //TODO: parse result.body to get balance!
        BigInteger big_integer_result{};
        return big_integer_result;
    });
}

void EthereumApi::send_raw_transaction(std::string tx_signed_encoded_envelope, std::string& result)
{
    http_client->build()->Post(base_url)
    .add_header("module", "proxy")
    .add_header("action", "eth_sendRawTransaction")
    .add_header("hex", tx_signed_encoded_envelope)
    .store_response(result)
    .process_response([&](const HttpResponse& result)
    {
    })
    .send_request();
}

rxcpp::observable<EtherTransactionsPage> EthereumApi::retrieve_Address_transactions_in_desc_order(std::string address,
        long page, int offset)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Post(base_url)
            .add_header("module", "account")
            .add_header("action", "txlist")
            .add_header("address", address)
            .add_header("startblock", "0")
            .add_header("endblock", "latest")
            .add_header("page", std::to_string(page))
            .add_header("offset", std::to_string(offset))
            .add_header("sort", "desc")
            .add_header("apikey", api_key)
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
        //TODO: parse result.body to get balance!
        EtherTransactionsPage ether_transactions_page_result{};
        return ether_transactions_page_result;
    });
}

rxcpp::observable<EtherTokenTransferTransactionsPage> EthereumApi::retrieve_Address_transactions_in_desc_order(
        std::string address, std::string contract_address, long page, int offset)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Post(base_url)
            .add_header("module", "account")
            .add_header("action", "tokentx")
            .add_header("contractaddress", contract_address)
            .add_header("address", address)
            .add_header("page", std::to_string(page))
            .add_header("offset", std::to_string(offset))
            .add_header("startblock", "0")
            .add_header("endblock", "latest")
            .add_header("sort", "desc")
            .add_header("apikey", api_key)
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
        //TODO: parse result.body to get balance!
        EtherTokenTransferTransactionsPage ether_token_transfer_transactions_page_result{};
        return ether_token_transfer_transactions_page_result;
    });
}

void EthereumApi::post_string_based_json_rpc_request(json input_json, std::string& result)
{
    http_client->build()->Post(base_url)
    .add_json_body(input_json.dump())
    .store_response(result)
    .process_response([&](const HttpResponse& result)
    {
    })
    .send_request();
}

void EthereumApi::post_string_based_json_rpc_request_and_relay_exact_string_response(json input_json
    , std::string& result)
{
    http_client->build()->Post(base_url)
    .add_json_body(input_json.dump())
    .store_response(result)
    .process_response([&](const HttpResponse& result)
    {
    })
    .send_request();
}