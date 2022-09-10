#ifndef REACTIVE_CURL_CARDANO_TYPES_HPP_
#define REACTIVE_CURL_CARDANO_TYPES_HPP_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <list>

//TODO: maybe we need to change these public members to private.
struct NetworkType
{
    int id;
	std::string yoroi_api_root;
	std::string block_frost_api_root;
	std::string get_block_frost_project_id;
	std::string explorer_trx_url;
};

namespace ns
{
    struct RawUtxo
    {
        std::string amount;
        std::string receiver;
        std::string tx_hash;
        int tx_index;
        std::string utxo_id;
    };

    void to_json(json& j, const RawUtxo& p)
    {
        j = json
        {
            {"amount", p.amount},
            {"receiver", p.receiver},
            {"tx_hash", p.tx_hash},
            {"tx_index", p.tx_index},
            {"utxo_id", p.utxo_id}
        };
    }

    void from_json(const json& j, RawUtxo& p)
    {
        j.at("amount").get_to(p.amount);
        j.at("receiver").get_to(p.receiver);
        j.at("tx_hash").get_to(p.tx_hash);
        j.at("tx_index").get_to(p.tx_index);
        j.at("utxo_id").get_to(p.utxo_id);
    }
} //namespace ns

#endif //REACTIVE_CURL_CARDANO_TYPES_HPP_


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