#ifndef REACTIVE_CURL_CARDANO_API_HPP_
#define REACTIVE_CURL_CARDANO_API_HPP_

#include "cardano-types.hpp"
#include "../../../include/rxcpp/rx.hpp"
#include "../../http-client/http-client.hpp"

#include <vector>

class CardanoApi
{
public:
    CardanoApi(HttpClient* client);
    ~CardanoApi();

    rxcpp::observable<std::vector<ns::RawUtxo>> get_utxos_of_addresses(
        std::vector<std::string> address_list, NetworkType network_type);

private:
    HttpClient* http_client;
};

#endif //REACTIVE_CURL_CARDANO_API_HPP_






































/* Clipboard Section */

// static rxcpp::observable<long> get_slot_num(NetworkType network_type);

// static rxcpp::observable<std::string> submit_signed_tx(std::vector<uint8_t> signed_transaction
//     , NetworkType network_type);

// static rxcpp::observable<std::list<SpentTransaction>> retrieve_transactions_desc(std::string address, long page
//     , int offset, NetworkType network_type);

// static void validate_block_frost_response(std::string operation_title, std::string operation_tag /*TODO: add Result object */ );
