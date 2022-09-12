#ifndef REACTIVE_CURL_CARDANO_TYPES_HPP_
#define REACTIVE_CURL_CARDANO_TYPES_HPP_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <list>

inline constexpr std::string kBlockFrostMainNetUrl{"https://cardano-mainnet.blockfrost.io/api/v0"};
inline constexpr std::string kBlockFrostTestNetUrl{"https://cardano-testnet.blockfrost.io/api/v0"};

//TODO: maybe we need to change these public members to private.
struct NetworkType
{
    explicit NetworkType(int id_, std::string yoroi_api_root_, std::string block_frost_api_root_,
        std::string block_frost_project_id_, std::string explorer_trx_url_): id(id_)
        , yoroi_api_root(std::move(yoroi_api_root_))
        , block_frost_api_root(std::move(block_frost_api_root_))
        , block_frost_project_id(std::move(block_frost_project_id_))
        , explorer_trx_url(std::move(explorer_trx_url_))
    {
    }

    int id;
	std::string yoroi_api_root;
	std::string block_frost_api_root;
	std::string block_frost_project_id;
	std::string explorer_trx_url;
};

inline constexpr NetworkType kShelleyMainNet
    { 1
    , "https://iohk-mainnet.yoroiwallet.com/api"
    , kBlockFrostMainNetUrl
    , "mainnetrCopBXtdL4ewkO03TzdIutti9g1kkwiT"
    , "https://explorer.cardano.org/en/transaction?id="
    };

inline constexpr NetworkType kShelleyTestNet
    { 0
    , "https://testnet-backend.yoroiwallet.com/api"
    , kBlockFrostTestNetUrl
    , "testnetZ7v804pEgYIUL8Q4aMP3BlFRDqlbV3aH"
    , "https://explorer.cardano-testnet.iohkdev.io/en/transaction?id="
    };

struct BigInteger //TODO: create file common types for all blockchains.
{
	double big_integer;

    std::string to_string()
    {
        return std::string{};
    }
};

struct TxoAmount
{
    BigInteger big_integer;
    std::string asset_id;
};

struct TxOutputAmount
{
    std::string unit;
    std::string quantity;
};

struct TxoInOut
{
    std::string address;
    std::list<TxoAmount> amounts;
};

struct TxContentOutputAmount
{
    std::string unit;
    std::string quantity;
};

struct TxContentUtxoInputs
{
    std::string address;
    std::list<TxContentOutputAmount> amount;
};

struct TxContentUtxoOutputs
{
    std::string address;
    std::list<TxContentOutputAmount> amount;
};

struct TransactionContent
{
    std::string block;
    int block_height;
    int slot;
    int index;
    std::list<TxOutputAmount> output_amount;
    std::string fees;
    std::string deposit;
    int size;
    std::string invalid_before;
    std::string invalid_hereafter;
    int utxo_count;
    int withdrawal_count;
    int delegation_count;
    int stake_cert_count;
    int pool_update_count;
    int pool_retire_count;
};

struct SpentTransaction
{
	std::string hash;
	std::string block;
	uint32_t block_tTime;
	int block_height;
	int slot;
	int index;
	std::list<TxoInOut> inputs;
	std::list<TxoInOut> outputs;
	std::string fees;
	std::string deposit;
	int size;
	std::string invalid_before;
	std::string invalid_hereafter;
	int utxo_count;
	int withdrawal_count;
	int delegation_count;
	int stake_cert_count;
	int pool_update_count;
	int pool_retire_count;

    SpentTransaction(std::string tx_hash, std::list<TxContentUtxoInputs> inputs
        , std::list<TxContentUtxoOutputs> outputs, uint32_t block_time_, TransactionContent tx_content)
    {
        hash = tx_hash;
		block = tx_content.block;
		blockTime = block_time_;
		blockHeight = tx_content.block_height;
		slot = tx_content.slot;
		index = tx_content.index;
		init_inputs(inputs);
		init_outputs(outputs);
		fees = tx_content.fees;
		deposit = tx_content.deposit;
		size = tx_content.size;
		invalidBefore = tx_content.invalid_before;
		invalidHereafter = tx_content.invalid_hereafter;
		utxoCount = tx_content.utxo_count;
		withdrawalCount = tx_content.withdrawal_count;
		delegationCount = tx_content.delegation_count;
		stakeCertCount = tx_content.stake_cert_count;
		poolUpdateCount = tx_content.pool_update_count;
		poolRetireCount = tx_content.pool_retire_count;
    }

	void init_inputs(std::list<TxContentUtxoInputs> externalInputs)
    {
		for (TxContentUtxoInputs exInput : externalInputs)
        {
			std::list<TxoAmount> amounts = get_txo_amounts(exInput.amount);
			inputs.emplace_back(exInput.address, amounts);
		}
	}

	void init_outputs(std::list<TxContentUtxoOutputs> externalOutputs)
    {
		for (TxContentUtxoOutputs exOutput : externalOutputs)
        {
			std::list<TxoAmount> amounts = get_txo_amounts(exOutput.amount);
			outputs.emplace_back(exOutput.address, amounts);
		}
	}

	std::list<TxoAmount> get_txo_amounts(std::list<TxContentOutputAmount> exAmounts)
    {
        std::list<TxoAmount> amounts;
		for(TxContentOutputAmount exAmount : exAmounts)
        {
			amounts.emplace_back(BigInteger{}, exAmount.unit);
		}
		return amounts;
	}

};


class Block
{
    long time;
    long height;
    std::string hash;
    long slot;
    int epoch;
    int epoch_slot;
    std::string slot_leader;
    int size;
    int tx_count;
    std::string output;
    std::string fees;
    std::string block_vrf;
    std::string previous_block;
    std::string next_block;
    int confirmations;
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