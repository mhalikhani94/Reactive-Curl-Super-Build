#ifndef REACTIVE_CURL_ETHEREUM_TYPES_HPP_
#define REACTIVE_CURL_ETHEREUM_TYPES_HPP_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <vector>
#include <list>
#include <variant>

//TODO: maybe we need to change these public members to private.

struct JsonRpcRequestBody
{
    std::string jsonrpc;
    std::string method;
    std::vector<std::any> params;
    int id;
};

struct BigInteger
{
	double big_integer;

    std::string to_string()
    {
        return std::string{};
    }
};

enum class EtherNetworks
{
    kMainnet,
    kRopsten,
    kRinkeby,
    kGoerli,
    kKovan,
};

struct JsonRpcResponseErrorDetail
{
    std::string code{};
    std::string message{};
};

struct JsonRpcStringResponse 
{
    int id{0};
    std::string jsonrpc{};
    std::string result{};
    std::string status{};
    std::string message{};
    JsonRpcResponseErrorDetail error{};
};

struct EtherTransaction
{
    std::string block_number;
    std::string hash;
    std::string nonce;
    std::string block_hash;
    std::string transaction_index;
    std::string from;
    std::string to;
    std::string value;
    std::string gas;
    std::string gas_price;
    std::string is_error; //TODO this parameter need more check.
    std::string tx_receipt_status; // "0" = execution failed & "1" = execution succeeded
    std::string input;
    std::string contract_address;
    std::string cumulative_gas_used;
    std::string gas_used;
    std::string confirmations;
};

struct EtherTransactionsPage
{
    std::vector<EtherTransaction> transactions;
};

struct EtherTokenTransferTransaction
{
    std::string block_number;
    std::string time_stamp;
    std::string hash;
    std::string nonce;
    std::string block_hash;
    std::string from;
    std::string contract_address;
    std::string to;
    std::string value;
    std::string token_name;
    std::string token_symbol;
    std::string token_decimal;
    std::string transaction_index;
    std::string gas;
    std::string gas_price;
    std::string gas_used;
    std::string cumulative_gas_used;
    std::string input;
    std::string confirmations;
};

struct EtherTokenTransferTransactionsPage
{
    std::vector<EtherTokenTransferTransaction> transactions;
};

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