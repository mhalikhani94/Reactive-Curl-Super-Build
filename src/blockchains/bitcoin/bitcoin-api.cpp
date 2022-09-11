#include "bitcoin-api.hpp"

#include <assert.h>

BitcoinApi::BitcoinApi(HttpClient* client, bool for_test_net_use) : http_client(client)
{
    if(!for_test_net_use)
    {
        base_url = "https://btcbook.nownodes.io/api/v2/";
    } else {
        base_url = "https://btcbook-testnet.nownodes.io/api/v2/";
    }

}

BitcoinApi::~BitcoinApi()
{

}

rxcpp::observable<BigInteger> BitcoinApi::retrieve_address_balance(std::string address)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(base_url + "address" + "/" + address)
            .add_header("api-key",api_key)
            .add_header("details", "basic")
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

rxcpp::observable<AddressTransactionsPage> BitcoinApi::retrieve_transactions_desc(std::string address,
        long page, int page_size)
{

    //Check for error with page and page size.

    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(base_url + "address" + "/" + address)
            .add_header("api-key",api_key)
            .add_header("page", std::to_string(page))
            .add_header("pageSize", std::to_string(page_size))
            .add_header("details", "txs")
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
        //TODO: parse result.body to AddressTransactionsPage!
        AddressTransactionsPage address_transactions_page{};
        return address_transactions_page;
    });
}

rxcpp::observable<std::vector<UtxoInfo>> BitcoinApi::retrieve_address_confirmed_utxos(std::string address)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(base_url + "utxo" + "/" + address)
            .add_header("api-key",api_key)
            .add_header("confirmed", "true")
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
        //TODO: parse result.body to AddressTransactionsPage!
        std::vector<UtxoInfo> utxo_info_result{};
        return utxo_info_result;
    });
}

rxcpp::observable<Transaction> BitcoinApi::retrieve_transaction_data(std::string tx_id)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(base_url + "tx-specific" + "/" + tx_id)
            .add_header("api-key",api_key)
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
        //TODO: parse result.body to AddressTransactionsPage!
        Transaction transaction_result{};
        return transaction_result;
    });
}

rxcpp::observable<SendTransactionResult> BitcoinApi::send_raw_transaction(std::string tx_hex_str)
{
    auto worker_thread = rxcpp::observe_on_event_loop();

    return rxcpp::observable<>::create<HttpResponse>([&](rxcpp::subscriber<HttpResponse> out)
    {
        try
        {
            http_client->build()->Get(base_url + "sendtx" + "/" + tx_hex_str)
            .add_header("api-key",api_key)
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
        //TODO: parse result.body to AddressTransactionsPage!
        SendTransactionResult send_transaction_result{};
        return send_transaction_result;
    });   
}