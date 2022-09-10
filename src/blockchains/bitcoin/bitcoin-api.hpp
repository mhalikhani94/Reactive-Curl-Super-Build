#ifndef REACTIVE_CURL_BITCOIN_API_HPP_
#define REACTIVE_CURL_BITCOIN_API_HPP_

#include "../../types.hpp"
#include "bitcoin-types.hpp"
#include "../../../include/rxcpp/rx.hpp"
#include "../../http-client/http-client.hpp"

#include <vector>

class BitcoinApi
{
public:
    BitcoinApi(HttpClient* client, bool for_test_net_use);
    ~BitcoinApi();

    rxcpp::observable<double> retrieve_address_balance(std::string address);

    rxcpp::observable<AddressTransactionsPage> retrieve_transactions_desc(std::string address,
        long page, int page_size);
    
    rxcpp::observable<std::vector<UtxoInfo>> retrieve_address_confirmed_utxos(std::string address);

    rxcpp::observable<Transaction> retrieve_transaction_data(std::string tx_id);

    rxcpp::observable<SendTransactionResult> send_raw_transaction(std::string tx_id);

private:
    HttpClient* http_client;
    std::string api_key{"52ab97d0-c01e-4dda-8d37-77b705d5560d"};
    std::string base_url;
};

#endif //REACTIVE_CURL_BITCOIN_API_HPP_