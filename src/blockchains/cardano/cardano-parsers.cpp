#include "cardano-parsers.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>

long CardanoParser::parse_get_slot_num_response(const HttpResponse& response)
{
    std::cout << "1" << response.body << std::endl;
    std::cout << "2" << response.curl_code << std::endl;
    std::cout << "3" << response.error_msg << std::endl;
    std::cout << "4" << response.header << std::endl;
    std::cout << "5" << response.http_response_code << std::endl;
    // json received_json = json::parse(response.body);
    // return received_json["slot"].get<long>();
    return 2;
}