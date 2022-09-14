#ifndef REACTIVE_CURL_CARDANO_PARSERS_HPP_
#define REACTIVE_CURL_CARDANO_PARSERS_HPP_

#include "../../http-client/http-response.hpp"
#include "cardano-types.hpp"

class CardanoParser
{
public:
    static long parse_get_slot_num_response(const HttpResponse& response);
};

#endif //REACTIVE_CURL_CARDANO_PARSERS_HPP_