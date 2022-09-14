#include "cardano-parsers.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

long CardanoParser::parse_get_slot_num_response(const HttpResponse& response)
{
    json received_json = json::parse(response.body);
    return received_json["slot"].get<long>();
}