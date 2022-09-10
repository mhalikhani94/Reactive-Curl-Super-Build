#ifndef REACTIVE_CURL_ETHEREUM_TYPES_HPP_
#define REACTIVE_CURL_ETHEREUM_TYPES_HPP_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <vector>
#include <list>

//TODO: maybe we need to change these public members to private.

namespace ns
{
    struct JsonRpcRequestBody
    {
        std::string jsonrpc;
        std::string method;
        std::vector<std::any> params;
        int id;
    };

    void to_json(json& j, const RawUtxo& p)
    {
        j = json
        {
            {"jsonrpc", p.jsonrpc},
            {"method", p.method},
            {"params", p.params},
            {"id", p.id}
        };
    }

    void from_json(const json& j, RawUtxo& p)
    {
        j.at("jsonrpc").get_to(p.jsonrpc);
        j.at("method").get_to(p.method);
        j.at("params").get_to(p.params);
        j.at("id").get_to(p.id);
    }
} //namespace ns

#endif //REACTIVE_CURL_ETHEREUM_TYPES_HPP_

































/* Garbage Section */

// struct TxoAmount
// {
// private:
//     uint64_t big_integer; // TODO: change with Java BigObject Class
//     std::string asset_id;

// public:
//     uint64_t get_big_integer(){return big_integer;}

//     std::string get_asset_id(){return asset_id;}
// };

// struct TxoInOut
// {
// private:
//     std::string address;
//     std::list<TxoAmount> amounts;

// public:
//     std::string get_address(){return address;}

//     std::list<TxoAmount> get_amounts(){return amounts;}
// };


// struct SpentTransaction
// {
//     //TODO: add getter functions.
//     //TODO: add proper constructor.
//     //TODO: we need add TxContentUtxoInputs, TxContentUtxoOutputs, TransactionContent
//     //TODO: add date time library for block_time.
//     std::string hash;
// 	std::string block;
// 	uint32_t block_time;
// 	int block_height;
// 	int slot;
// 	int index;
// 	std::list<TxoInOut> inputs;
// 	std::list<TxoInOut> outputs;
// 	std::string fees;
// 	std::string deposit;
// 	int size;
// 	std::string invalidBefore;
// 	std::string invalidHereafter;
// 	int utxoCount;
// 	int withdrawalCount;
// 	int delegationCount;
// 	int stakeCertCount;
// 	int poolUpdateCount;
// 	int poolRetireCount;
// };