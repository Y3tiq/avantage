// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2018 Oxid developers
// Copyright (c) 2019 Avantage developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "libzerocoin/Params.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"
#include <assert.h>
#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 520159231 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits)
{
    int32_t nVersion = 1;
    const char* pszTimestamp = "Those who cannot change their minds cannot change anything";
    const CScript genesisOutputScript = CScript() << ParseHex("0406c82b219cd93205007a01c193416c840b1e1134173c6261e5c9dca4e6ca78a188460cf4c67dd9dbec449cb5511b067b71c28424c40462095acf31ce31282e1b") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, 100 * COIN);
}

/**
 * Main network
 */

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
	(0, uint256("00007c53c2d79e066757eb97ae1db3da406a2d636342614f7edbb04e7fdb82cb"))
	(1000, uint256("000006848ceb534e7d80e187658b0a4dbe161f19d77784247df5b43af435856e"));

    static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1554203700, // * UNIX timestamp of last checkpoint block
        1022,     		// * total number of transactions between genesis and last checkpoint (the tx=... number in the SetBestChain debug.log lines)
        1000};      // * estimated number of transactions per day after checkpoint

    static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of(0, uint256("00007c53c2d79e066757eb97ae1db3da406a2d636342614f7edbb04e7fdb82cb"));
    static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1554203700,
        0,
        0};

    static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of(0, uint256("00007c53c2d79e066757eb97ae1db3da406a2d636342614f7edbb04e7fdb82cb"));
    static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        1554203700,
        0,
        0};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);
    return &ZCParams;
}


class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        pchMessageStart[0] = 0xc9;
		pchMessageStart[1] = 0xf6;
		pchMessageStart[2] = 0xd3;
		pchMessageStart[3] = 0xa0;
        vAlertPubKey = ParseHex("04376f3902d3ca6fe15dc8ddf0536b7a9d6e7946cecd8f99b48cec36338446383012d69c339f40317c135f52087ab81760a683c2ec5bf98945f11eab04a705f1be");
        nDefaultPort = 7633;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 1006;
        nRejectBlockOutdatedMajority = 1007;
        nToCheckBlockUpgradeMajority = 1008;
        nMinerThreads = 0;
        nTargetTimespan = 24 * 60 * 60; // Avantage: 1 day
        nTargetSpacing = 1 * 60;        // Avantage: 1 minute
        nMaturity = 10;
        nMasternodeCountDrift = 20; // ---
        nMaxMoneyOut = 400000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 10000;
        nModifierUpdateBlock = 0;             // 0
        nBlockRecalculateAccumulators = 1005; // Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 1003;         // First block that bad serials emerged
        nBlockLastGoodCheckpoint = 1005;      // Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 1001;      // Start enforcing the invalid UTXO's
        nZerocoinStartHeight = 44000000;      //
        nBlockEnforceSerialRange = 1004;      // Enforce serial range starting this block
        nZerocoinStartTime = 4102444799;      //

        genesis = CreateGenesisBlock(1554203700, 24618, 0x1f00ffff);
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00007c53c2d79e066757eb97ae1db3da406a2d636342614f7edbb04e7fdb82cb"));
        assert(genesis.hashMerkleRoot == uint256("0x5f78bd881338a3f875120cc79fecb54a627b3b7a226609640e82bc8ea509ce6d"));
		
		vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 23); // A
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 70); // V
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 53);     // N
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x13)(0x00)(0x00)(0x80).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true; // false
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "048fe415aa74fc972959190d2ff25467308a5b9160cba6d7a91dd74e4f631ce98cf22fdca4131498f160d39a29dfa64a5793d6b1e2018bc34e481861897f069e41";
        strObfuscationPoolDummyAddress = "";
        nStartMasternodePayments = 1540710800;

        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                          "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                          "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                          "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                          "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                          "31438167899885040445364023527381951378636564391212010397122822120720357";

        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT;       // high fee required for zerocoin mints
        nMintRequiredConfirmations = 20;      // the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 2;
        nDefaultSecurityLevel = 100;          // full security level for accumulators
        nZerocoinHeaderVersion = 4;           // Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6;        // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x92;
		pchMessageStart[1] = 0xd4;
		pchMessageStart[2] = 0xd8;
		pchMessageStart[3] = 0xba;
        vAlertPubKey = ParseHex("040a7c502be48518cf78563754b3f4a2f1e6cded4ddede1347476f06c822915620356c146616750543200ba7baa890ee9a882886ea9958f1eae65ceaf1bf6c6ee4");
        nDefaultPort = 17633;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 24 * 60 * 60; // Avantage: 1 day
        nTargetSpacing = 1 * 60;        // Avantage: 1 minute
        nLastPOWBlock = 100;
        nMaturity = 10;
        nModifierUpdateBlock = 0;
        nMaxMoneyOut = 400000000 * COIN;

        genesis = CreateGenesisBlock(1554203700, 24618, 0x1f00ffff);
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00007c53c2d79e066757eb97ae1db3da406a2d636342614f7edbb04e7fdb82cb"));
        assert(genesis.hashMerkleRoot == uint256("0x5f78bd881338a3f875120cc79fecb54a627b3b7a226609640e82bc8ea509ce6d"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 83); // Testnet Avantage addresses start with 'a'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 127);  // Testnet Avantage script addresses start with 't'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);      // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet Avantage BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Avantage BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet Avantage BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x01)(0x00)(0x00)(0x80).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04fea283868692deebe17f38febbaa51e2bc80adb39c4364e3efb12e4b6fd6c29618f57e18c3a67dfe0626d8259bb076724f22439c6fe02d36a4f2b3db8ca09650";
        strObfuscationPoolDummyAddress = "";
        nStartMasternodePayments = 1540711800;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xd9;
		pchMessageStart[1] = 0x9f;
		pchMessageStart[2] = 0xf2;
		pchMessageStart[3] = 0xf6;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Avantage: 1 day
        nTargetSpacing = 1 * 60;        // Avantage: 1 minute
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nDefaultPort = 17744;

        genesis = CreateGenesisBlock(1554203700, 24618, 0x1f00ffff);
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00007c53c2d79e066757eb97ae1db3da406a2d636342614f7edbb04e7fdb82cb"));
        assert(genesis.hashMerkleRoot == uint256("0x5f78bd881338a3f875120cc79fecb54a627b3b7a226609640e82bc8ea509ce6d"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 83);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 127);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x01)(0x00)(0x00)(0x80).convert_to_container<std::vector<unsigned char> >();

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 31240;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
