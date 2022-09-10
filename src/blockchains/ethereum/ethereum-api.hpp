#ifndef REACTIVE_CURL_ETHEREUM_API_HPP_
#define REACTIVE_CURL_ETHEREUM_API_HPP_

#include "cardano-types.hpp"
#include "../../../include/rxcpp/rx.hpp"
#include "../../http-client/http-client.hpp"

#include <vector>

class EthereumApi
{
public:
    EthereumApi(HttpClient* client);
    ~EthereumApi();

    void get_last_block_base_fee_per_gas();

private:
    HttpClient* http_client;
};

#endif //REACTIVE_CURL_ETHEREUM_API_HPP_
