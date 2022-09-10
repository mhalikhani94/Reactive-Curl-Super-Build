#ifndef REACTIVE_CURL_BITCOIN_TYPES_HPP_
#define REACTIVE_CURL_BITCOIN_TYPES_HPP_

#include <string>
#include <list>
#include <list>


//TODO: maybe we need to change these public members to private.
struct TransactionInput
{
	std::string tx_id;
	long sequence;
	long n;
	std::list<std::string> addresses;
	bool is_address;
	std::string value;
};

struct TransactionOutput
{
	std::string value;
	long n;
	std::string hex;
	std::list<std::string> addresses;
	bool is_address;
};

struct Transaction
{
	std::string tx_id;
	int version;
	std::list<TransactionInput> vin;
	std::list<TransactionOutput> vout;
	std::string block_hash;
	long block_height;
	long confirmations;
	long block_time;
	std::string value;
	std::string value_in;
	std::string fees;
	std::string hex;
};

struct AddressBasicInfo {
	std::string address;
	std::string balance;
	std::string total_received;
	std::string total_sent;
	std::string unconfirmed_balance;
	long unconfirmed_txs;
	long txs;
};

struct AddressTransactionsPage : public AddressBasicInfo
{
	int page;
	int total_pages;
	int items_on_page;
	std::list<Transaction> transactions;
};

struct UtxoInfo
{
	std::string txid;
	long vout;
	std::string value;
	long height;
	long confirmations;
	std::string txHex;
};

struct SendTransactionResult
{
	std::string result;
};

#endif //REACTIVE_CURL_BITCOIN_TYPES_HPP_



























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