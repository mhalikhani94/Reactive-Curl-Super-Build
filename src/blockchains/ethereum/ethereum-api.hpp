#ifndef REACTIVE_CURL_ETHEREUM_API_HPP_
#define REACTIVE_CURL_ETHEREUM_API_HPP_

#include "ethereum-types.hpp"
#include "../../../include/rxcpp/rx.hpp"
#include "../../http-client/http-client.hpp"

#include <vector>
#include <string>

class EthereumApi
{
public:
    EthereumApi(HttpClient* client, EtherNetworks ether_network);
    
    ~EthereumApi();

    rxcpp::observable<BigInteger> get_last_block_base_fee_per_gas();
    
    rxcpp::observable<BigInteger> retrieve_address_balance(std::string address);
    
    rxcpp::observable<BigInteger> get_gas_price();

    rxcpp::observable<BigInteger> estimate_gas_units(std::string from, std::string to, BigInteger gas_price
        , BigInteger value, std::string data);
    
    void send_raw_transaction(std::string tx_signed_encoded_envelope, std::string& result);

    rxcpp::observable<EtherTransactionsPage> retrieve_Address_transactions_in_desc_order(std::string address
        , long page, int offset);

    rxcpp::observable<EtherTokenTransferTransactionsPage> retrieve_Address_transactions_in_desc_order(
        std::string address, std::string contract_address, long page, int offset);
    
private:
    void post_string_based_json_rpc_request(json input_json, std::string& result);

    void post_string_based_json_rpc_request_and_relay_exact_string_response(json input_json, std::string& result);

    HttpClient* http_client;

    std::string base_url;

    std::string api_key;

    std::string network_name;
};

#endif //REACTIVE_CURL_ETHEREUM_API_HPP_
