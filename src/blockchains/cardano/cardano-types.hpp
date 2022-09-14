#ifndef REACTIVE_CURL_CARDANO_TYPES_HPP_
#define REACTIVE_CURL_CARDANO_TYPES_HPP_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <string>
#include <list>

inline constexpr std::string_view kBlockFrostMainNetUrl{"https://cardano-mainnet.blockfrost.io/api/v0"};
inline constexpr std::string_view kBlockFrostTestNetUrl{"https://cardano-testnet.blockfrost.io/api/v0"};

//TODO: maybe we need to change these public members to private.
struct NetworkType
{
    explicit NetworkType(int id_, std::string yoroi_api_root_, std::string_view block_frost_api_root_,
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
    TxoAmount(BigInteger big_integer_, std::string asset_id_)
    {
        big_integer = big_integer_;
        asset_id = asset_id_;
    }

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
    TxoInOut(std::string address_, std::list<TxoAmount> amounts_)
    {
        address = address_;
        amounts = amounts_;
    }

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
	uint32_t block_time;
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
		block_time = block_time_;
		block_height = tx_content.block_height;
		slot = tx_content.slot;
		index = tx_content.index;
		init_inputs(inputs);
		init_outputs(outputs);
		fees = tx_content.fees;
		deposit = tx_content.deposit;
		size = tx_content.size;
		invalid_before = tx_content.invalid_before;
		invalid_hereafter = tx_content.invalid_hereafter;
		utxo_count = tx_content.utxo_count;
		withdrawal_count = tx_content.withdrawal_count;
		delegation_count = tx_content.delegation_count;
		stake_cert_count = tx_content.stake_cert_count;
		pool_update_count = tx_content.pool_update_count;
		pool_retire_count = tx_content.pool_retire_count;
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

    // void to_json(json& j, const RawUtxo& p)
    // {
    //     j = json
    //     {
    //         {"amount", p.amount},
    //         {"receiver", p.receiver},
    //         {"tx_hash", p.tx_hash},
    //         {"tx_index", p.tx_index},
    //         {"utxo_id", p.utxo_id}
    //     };
    // }

    // void from_json(const json& j, RawUtxo& p)
    // {
    //     j.at("amount").get_to(p.amount);
    //     j.at("receiver").get_to(p.receiver);
    //     j.at("tx_hash").get_to(p.tx_hash);
    //     j.at("tx_index").get_to(p.tx_index);
    //     j.at("utxo_id").get_to(p.utxo_id);
    // }
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