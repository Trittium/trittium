// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
// Copyright (c) 2021 The DECENOMY Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

#define DISABLED 0x7FFFFFFE;

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
	const char* pszTimestamp = "Bitcoin 'better than gold' if you study it, fund manager tells mainstream media, COINTELEGRAPH, 18/03/2021";
    const CScript genesisOutputScript = CScript() << ParseHex("04bcfbea59ec97482475f554db08f7f67c5c5a95a52a356b052d0ed87862815796fe6022c33a0baf09fcd8d3ee98ebb6aa6a77fb43d716641242cfd45369a88e7d") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256S("00000e41db923032d1be957ad4be59aa287c9727ca7602dd403cf7312d259968"))
    (1645, uint256S("2563b5fcdec45e692d2c07d939e351b35a37f3132e605d2a4bd8a6cad5730cee"))
    (3271, uint256S("5002750c5f07218efa874ab515f63f5184b621c80ca67d8708c270c9f719a763"))
    (4879, uint256S("1776e27d36ae6dcb42f02b64b1b23e8f56a67676d3edf5229751ac8541e5febf"))
    (6505, uint256S("1cfdc8e81f1a0217df7346dc1054c9a2590f17a7965074eb0f0f9325bd282e05"))
    (8135, uint256S("468aca29161cf011766ddccc803cef92bae5574d2e9953a78b9a8283f7262ac0"))
    (9759, uint256S("c87add595fdb3b65b902c66b21476aebd6575051223e643eaf66bf1a136232f0"))
    (11383, uint256S("491e41efbbf757959c2e1c6d300a25581210ceb43f94cb39474599fef4c43001"))
    (13009, uint256S("7b6b38c17b636444affdcc6b966caeb7834aee8d2f4d3e08911d51c4e776e783"))
    (14633, uint256S("0e65979cff761cc67c855363148cc0adc38f8aaec3efe5c4be34d520136c4d93"))
    (16265, uint256S("8132b3b219e9b92d483ad308b6c320938b140e2bbced25173223db5357cdb749"))
    (17891, uint256S("deb424cb01e983507454abceed4dbc95c40fd734958b63cd2ec1c85244cba3b0"))
    (19525, uint256S("a6580e1812175570c961e026b59e5af482645d825fb516eebf22a4d30076de26"))
    (21153, uint256S("91129093bbd18f9260879ed2e1c9734cfc94c4213327cdc76a4076d499d0a61b"))
    (22787, uint256S("1ab0f3dd39c233cf1b616405f8138bb25f15fe73e2d70cd2699b14b9e156cedf"))
    (24415, uint256S("8a18c5a262271fefa8ae92047fe618f61be82f166a04ad5a631f876b74eea906"))
    (26037, uint256S("587004633183a9f5a0ca76fd86f8952c3f64420c52565f28847e4c3c3efdbf5c"))
    (27651, uint256S("e5c0eaaf0dca8e3325e2128a88c4fb23b142add8cb147a2a9113b5da5e3720ac"))
    (29275, uint256S("eb49648352cfbbf1306773e84e1886f8e8263e362b3dfbec14c901927f3372fc"))
    (30889, uint256S("7b9afa0ebff6309eed8193fa0d139d8d84654b2e70d39c284dc8b6a9e01393fd"))
    (32519, uint256S("ca1c36b1491e1828e382a21a497486983f815a03bafc9cd42949307caa9d0e8e"))
    (34151, uint256S("9b39b64f37d3b50b00145214424f19478cac07001f979c4629b45b5891cf5ec5"))
    (35781, uint256S("cc43ca59827cdab2f26168a6664e0f61a07193447db30ecc6852d502e6e2647f"))
    (37399, uint256S("75590690113a8abb0cd489b64e022f1688f38d24eea5a1f0b56a94ced3b17d56"))
    (39039, uint256S("cfe448e35276df20463d862f469e6403f846c5f776a17933ccb4bdef61816bde"))
    (40667, uint256S("7ba623a51c255c0003c86d60f14d9d12fcc927f4d681773da49924850dcacb96"))
    (42297, uint256S("bf663fbdee1b74f1fc99200daacabc0b01a6e4966f1102ca6bd0d57d35a02fec"))
    (43929, uint256S("06ed5cb95cc47e8380df41a51df70e4cc3e5c8f4d7b968829b2f62306b80d27c"))
    (45567, uint256S("ad3f9a84756fc23c9611572466e295c09262793a37391fdb60f759184ecac7a9"))
    (47197, uint256S("e1bc6bde6497440eff96096e44c86b786c52cdcec2b419be17b46d87ddaa483e"))
    (48827, uint256S("1ea9f8ef72195b50956b1ef8e82f1a276e71768133774ff59cfbf7aa887c2ec5"))
    (50459, uint256S("81e2333e41a65bc7270fb2e334d68e0b35f84e92d07002b190fa19c27cddc145"))
    (52089, uint256S("ff848e230a574ed8e95398c655ac0d13b979f4a19d446e697fd94f63a1d2ec53"))
    (53721, uint256S("2290dbe17d7d4c10f9ad08afe88633bc448e701eaf9b080b8fae02ee6b6c85e0"))
    (55355, uint256S("f207e1900d2d35ad41af5ebe24e655c1d60325e495de07bce2be3e196a9deaf8"))
    (56985, uint256S("1aaac75fb82cb75b80081fa637382bd9cbd234f4f3362bc2cb120e11222d5350"))
    (58613, uint256S("7a561aa223ac9c3beaa48446238dc3fe62e4cf26f35baa07271de51ff1034d7b"))
    (60247, uint256S("49feb503dc15ce05384f0c1d85fa23be7d8092897c1734b8533ec957f0cbba78"))
    (61885, uint256S("5c0015a98d708fddcd98b0130cb43c2f587891217bc6b1f7c79808ff1c2f9a5f"))
    (63521, uint256S("c680ccbb53d0cad02bdc53cceac0c494c59e649b38adc257a5879ef9556ced91"))
    (65161, uint256S("0a9e37d8404e450bc611281bcfd8a48a155613f4b65bf7f025e475f0cbcbd8b4"))
    (66795, uint256S("db76790f964a7d575c86b762317a2460496f31d644299489dbb12463526a76ed"))
    (68425, uint256S("5f667a57f0bf0795997b2d51a4ab1db21a624c8240087a5ecc343519b847fcb1"))
    (70063, uint256S("9a5fffdb0297875d62ed73ae6119fdad20f646660ced2fe09ea5f7e677510703"))
    (71693, uint256S("0caca2e584257c195d9161279892196ee031d9b234ddfafb05e322e385c9fc47"))
    (73331, uint256S("f96483470882a95a63592eb0501779e9cbacf5059ea1cd0737933f6e008f9550"))
    (74965, uint256S("198390abd3df94bf6e8fad83311e9ff6a11c6279a09e561de9bc495ecc4216a7"))
    (76607, uint256S("1bed2920b31937973555ed098ebf7434ae4f550abd7d2f727e8c608215f681cb"))
    (78241, uint256S("6013e026cf9c61d2dc5775698194b8099b8bfa82e6f5cbb5633cb347c5bd3152"))
    (79875, uint256S("ccf76823f74cef4742bf6cde15ae9e848ad6ec8c9a0d0e40edb903276c339329"))
    (81515, uint256S("8633bfe7d1ed48c5acef55775638350b1df02e706c5f02a13cae64e01141e8fe"))
    (83153, uint256S("4d3d911ed24c8a985e7517ce5e697fe8572f45e9800f66b8edd7c836fa9a6012"))
    (84789, uint256S("14a8478f7eca5c32b517f186434e4b5b6955addbc093665a8bb3647cb243a56d"))
    (86427, uint256S("34b6f6257c133543f03cedbcf80c549d6bec329eb735c2cd263ade93b97a6260"))
    (88059, uint256S("50c988d9558ec0a6d18df4975e21161d4746b69ff2d5aa2eefe21f470b7570b9"))
    (89697, uint256S("418f996d2a67150ef54d4d80f2fb92f0be83c48394a70e46ea6c60669dce47e7"))
    (91325, uint256S("3eb9df273d452d77b46a723e13029c41a84ac4b10e673260d88a85dfbca6b6df"))
    (92959, uint256S("af5bc8592b8856c7b445ed46b870d7313e421339a68b435422b82fc9f6d4d893"))
    (94585, uint256S("b4deadf58c260b94e489e20b8c7dbe6c1b45723f6935ca2e08c99fd50893db76"))
    (96223, uint256S("983edda08113d799312c817ce0e841bfecfaf6003a1f71bc8411d718652edf10"))
    (97863, uint256S("8695daa38ec64a10bca848f247be420c89091f17369f0845afc3bade375e7f7a"))
    (99503, uint256S("41e146ae5279668b8ff938418f32879f1ae9ce2beafd11cb97a9caf64844612f"))
    (101149, uint256S("06ae34b94bba07560bb16c9fe3bcb02acbb0fa8bdb09f921b434d1ac611ae7b7"))
    (102783, uint256S("b03c75c54643bbbd8cee306132c30a025cdbd2f95df14030a7fc1e186499cddd"))
    (104415, uint256S("963916ba554360a757a89365e20a15822cb5bfe41f59418364d833c57ae2a233"))
    (106047, uint256S("8acfeed198c0cea3236ade1ffda0309a5880b1601c00d90101048bbc1b2e9d88"))
    (107677, uint256S("dfa762206583431f5ebf94bd587ad2597c0ecb48a558b19324fa0baf9c08a214"))
    (109313, uint256S("a9b89adc0ebb380013d2562b6c4beb870f91f0f24113e6bbef202cb9bda8c4b3"))
    (110947, uint256S("fbb52b1c266c8a65f0daea7e8f0cbbee29645d6bbd9bf60a0a973b8879569adb"))
    (112583, uint256S("e4531099995eedcdeb06ee6b0b1bb432759bf8ae4e3c58d0697e46d70a344093"))
    (114227, uint256S("a1612b16feff21fc4a584271be2ff39da7bbb24534c793b0bab2b1851f5e3596"))
    (115855, uint256S("d21517e4157c33d39bb5c0c7bb05ff7f4e2695e4839bbfe7fd7e77a5879f1c80"))
    (117487, uint256S("67d3fe7a259c5f608180a71fdae91cb68637c79dc4ee4e7ad326a42fe5d858c1"))
    (119107, uint256S("e22d976c2e2f75002d85338d2117d4005e8590afc7045ab9da22cc963a4df450"))
    (120741, uint256S("1035bbb70acfe8f98afd8a5768738427c82f01d64438bb7a0099d8fd48f66811"))
    (122381, uint256S("fa5110aef7b2aa199032504cee5b9bd804b5de4142aa25f6427987dbbcb10df4"))
    (124017, uint256S("4d8c93f4d381ea05016e8333ca164bb8038ff9647cbdcea3f6c7b409d5265c4b"))
    (125645, uint256S("0aa823f75b092727af574e50b9779252e3d2530e4507c4e39ddaa9209abe8392"))
    (127275, uint256S("612d671f380da2e191ca729255a306555c184200a8f5e998a9298c535808315d"))
    (128911, uint256S("209fccc4832348e08a5e416cb22fa4b71c91626e205ce61d1be79a57dd801f3a"))
    (130557, uint256S("0cbdd943c1332ddd5c3a14392ac245755240690a74ed207871f75a13c1a73a90"))
    (132189, uint256S("5422c657729d7236cd14da8c3326477f17e57537854c00b6bde36342dffeba4e"))
    (133821, uint256S("96cd5cafea949c8070076842fb468c41a8e90f68be1dce156f5e9939398ced1e"))
    (135451, uint256S("83d5111cdbb43ffb6eb09ed5d2d9b1efb3d2f34400fa61787975640a763126a7"))
    (137081, uint256S("50500fa5814717a134f932effc837ade951cde6ca7348eb568ff036d8422573e"))
    (138715, uint256S("739f7baf27a2bb1b3d9430a1dd93f313084a8218d5c58dc5eaa7e2e29ec539e4"))
    (140353, uint256S("2c257e1fa4a07a2f8e5262502b7532ebe18ed928b1f86159e85a012e5197d799"))
    (141991, uint256S("ebe47a83162b2029c8e39f0b9dcacfc4fb284cb51e7ba4df4349191dfb35a265"))
    (143623, uint256S("efba1ae1286e6ef35663ab7e639ffcbe3615fbb62c88192819fed96d151ac7ba"))
    (145251, uint256S("c70252825e8065c4cf9bfa460166eedc66f4d3c7a0bdf0cedcdab5c1dbbc32ce"))
    (146883, uint256S("4b08830cf2269a9f525dea2f42ab74fc1e72687971ba43458de216a01bdd5f34"))
    (148523, uint256S("64f104464c52857c738a0e082e4a95f64b14efeb11a2e41d8bb26a4683e56c97"))
    (150161, uint256S("0c97c8267cf7ac409871d2c4ee972e1c78636def10ecceed8a9e2bf40d87a53c"))
    (151795, uint256S("bf933b919aee96588a1fd088b9873883e5d045d08df36b5f1129097ee583768d"))
    (153431, uint256S("0ac65077712905c391975589b00c265769bd3046c984d9e65ccb196fae7e276b"))
    (155067, uint256S("0a85fc6c22123a32b31117eb28517195344589b54c8b474c349a9b6734e57c20"))
    (156699, uint256S("9247886cf2be8ada4ffe41155b6c4e440b081336747a63baf3485c78d1488b09"))
    (158345, uint256S("57db4626226845e876d6baf4cd209a0a1c040871dd4fe71a10e2c41eba362b74"))
    (159991, uint256S("6260f58c5ca2826592a7bf56881a9561505544c5adda87a7deaa8c0cb4261c33"))
    (161637, uint256S("5b4cbe48b7f8645d37238c3f4aa356cdb3886986d3a535401c4d5f58a93c7660"))
    (163283, uint256S("0eb696283b227c0e7a52c93d37cc3c101f1edc24a333c5b15b3acbede7add5db"))
    (163366, uint256S("9ff041312ea955b8c1bd10dd0ecfe8a571c566c6f268e92222e93cf7796fb51e")); 

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1626187830, // * UNIX timestamp of last checkpoint block
    377306,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the UpdateTip debug.log lines)
    2880        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256("0x000034fe1805f04a440c06c1b96b1234b45d9fdba0fcb055d5caf4d83689428c"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1616082640,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of
	(0, uint256("0x003279ba296f0376d7a8417354e0ef9bd00fa29f92ac5d9cdf97c160673474af")
);

static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1616082641,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        // // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
        // /////////////////////////////////////////////////////////////////

        // uint32_t nGenesisTime = 1616082639; // 2021-03-18T15:50:39+00:00

        // arith_uint256 test;
        // bool fNegative;
        // bool fOverflow;
        // test.SetCompact(0x1e0ffff0, &fNegative, &fOverflow);
        // std::cout << "Test threshold: " << test.GetHex() << "\n\n";

        // int genesisNonce = 0;
        // uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        // uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // for (int i=0;i<40000000;i++) {
        //     genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e0ffff0, 1, 0 * COIN);
        //     //genesis.hashPrevBlock = TempHashHolding;
        //     consensus.hashGenesisBlock = genesis.GetHash();

        //     arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
        //     if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
        //         BestBlockHash = consensus.hashGenesisBlock;
        //         std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
        //         std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        //     }

        //     TempHashHolding = consensus.hashGenesisBlock;

        //     if (BestBlockHashArith < test) {
        //         genesisNonce = i - 1;
        //         break;
        //     }
        //     //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        // }
        // std::cout << "\n";
        // std::cout << "\n";
        // std::cout << "\n";

        // std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        // std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        // std::cout << "Genesis Merkle 0x" << genesis.hashMerkleRoot.GetHex() << std::endl;

        // exit(0);

        // /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(1616082639, 933427, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000e41db923032d1be957ad4be59aa287c9727ca7602dd403cf7312d259968"));
        assert(genesis.hashMerkleRoot == uint256S("0xbeb39df5e5c3029bb4a6d66357a41d06c275ea69a96c9081a3e6f8d2f8efe918"));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.powLimit   = ~UINT256_ZERO >> 20;  // trittium starting difficulty is 1 / 2^20
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 30 * 24 * 60; // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations = 6;  // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 20;       // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 500000000 * COIN;
        consensus.nPoolMaxTransactions = 3;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;    // must be at least a day old to make it into a budget
        consensus.nStakeMinAge = 60 * 60; // 1h
        consensus.nStakeMinDepth = 100;
        consensus.nStakeMinDepthV2 = 600;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        // spork keys
        consensus.strSporkPubKey = "04709f473a079113abd0da53176c9ecdc2f67c86ef6379459786d757d6d03a1954c0d4a902bb17782607f014d3476b3c73bbff47aa5a4d8b3d870cae33aa497a10";
        consensus.strSporkPubKeyOld = "";
        // consensus.strSporkPubKey = "029e54031b41982ac1c22b780c1101cf4b0efb71f7beb435e46fc0b406ff545ef6";
        // consensus.strSporkPubKeyOld = "0464b1f34052a9fa87d18516fd97bfd70ed78a3424d59e9b9ffa9ea7fa35b50d376ec01b0923a2c7a8edfbfd2787247aadea861d4c6d8fb5814b708ddfb153aafc";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height-based activations
        consensus.height_last_ZC_AccumCheckpoint    = DISABLED;
        consensus.height_last_ZC_WrappedSerials     = DISABLED;
        consensus.height_start_InvalidUTXOsCheck    = DISABLED;
        consensus.height_start_ZC_InvalidSerials    = DISABLED;
        consensus.height_start_ZC_SerialRangeCheck  = DISABLED;
        consensus.height_ZC_RecalcAccumulators      = DISABLED;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1893456000;        // 01/01/2030 @ 12:00am (UTC)
        consensus.ZC_WrappedSerialsSupply = 0; //4131563 * COIN;   // zerocoin supply at height_last_ZC_WrappedSerials

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight                   = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight              = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight                    = 1001;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight                 = 1441;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight                     = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight                  = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight                  = 1441;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight              = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MODIFIER_V2].nActivationHeight      = 1541;
        consensus.vUpgrades[Consensus::UPGRADE_TIME_PROTOCOL_V2].nActivationHeight       = 1641;
        consensus.vUpgrades[Consensus::UPGRADE_P2PKH_BLOCK_SIGNATURES].nActivationHeight = 1741;
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MIN_DEPTH_V2].nActivationHeight     = 5001;
        consensus.vUpgrades[Consensus::UPGRADE_MASTERNODE_RANK_V2].nActivationHeight     = 166929;

        consensus.vUpgrades[Consensus::UPGRADE_POS].hashActivationBlock                    = uint256S("e4cd39496b46796b69b53c674dfdaaa789396c907f8f0252d72d89effdef8b45");
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].hashActivationBlock                 = uint256S("80dd173daeebb1fc25e340d05f78dc17bc19a05f9f210e46db312c0c0532e9b2");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock                  = uint256S("80dd173daeebb1fc25e340d05f78dc17bc19a05f9f210e46db312c0c0532e9b2");
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MODIFIER_V2].hashActivationBlock      = uint256S("506c9d4f33ce7334d429a6d5f8e4b608bb8d7c0c2f233fb7becb2b962e7f53a5");
        consensus.vUpgrades[Consensus::UPGRADE_TIME_PROTOCOL_V2].hashActivationBlock       = uint256S("bfae8546b921841766191b5774e18405aa32986806767a5ac159ceefdd0cb3ec");
        consensus.vUpgrades[Consensus::UPGRADE_P2PKH_BLOCK_SIGNATURES].hashActivationBlock = uint256S("05fb6d596455cb9b24af308171a3a5b3902aac2dc4cb0c582289b5df4dbbf774");
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MIN_DEPTH_V2].hashActivationBlock     = uint256S("ce797d5715d07dcabe6e6c3557ed10e9e34ea92e9ec1a54c6eb02905f665c0b6");
        consensus.vUpgrades[Consensus::UPGRADE_MASTERNODE_RANK_V2].hashActivationBlock     = uint256S("0x0");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x31;
        pchMessageStart[1] = 0x41;
        pchMessageStart[2] = 0x51;
        pchMessageStart[3] = 0xf1;
        nDefaultPort = 31001;

        vSeeds.push_back(CDNSSeedData("seeder", "seeder.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed1",  "seed1.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed2",  "seed2.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed3",  "seed3.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed4",  "seed4.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed5",  "seed5.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed6",  "seed6.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed7",  "seed7.trittium.net"));
        vSeeds.push_back(CDNSSeedData("seed8",  "seed8.trittium.net"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 65); // T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19); // 8 or 9
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);  // 
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x03)(0x46).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main)); // added
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v1)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        // // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
        // /////////////////////////////////////////////////////////////////

        // uint32_t nGenesisTime = 1616082640; // Thu Mar 18 2021 15:50:40 GMT+0000

        // arith_uint256 test;
        // bool fNegative;
        // bool fOverflow;
        // test.SetCompact(0x1efffff0, &fNegative, &fOverflow);
        // std::cout << "Test threshold: " << test.GetHex() << "\n\n";

        // int genesisNonce = 0;
        // uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        // uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // for (int i=0;i<40000000;i++) {
        //     genesis = CreateGenesisBlock(nGenesisTime, i, 0x1efffff0, 1, 0 * COIN);
        //     //genesis.hashPrevBlock = TempHashHolding;
        //     consensus.hashGenesisBlock = genesis.GetHash();

        //     arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
        //     if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
        //         BestBlockHash = consensus.hashGenesisBlock;
        //         std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
        //         std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        //     }

        //     TempHashHolding = consensus.hashGenesisBlock;

        //     if (BestBlockHashArith < test) {
        //         genesisNonce = i - 1;
        //         break;
        //     }
        //     //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        // }
        // std::cout << "\n";
        // std::cout << "\n";
        // std::cout << "\n";

        // std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        // std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        // std::cout << "Genesis Merkle 0x" << genesis.hashMerkleRoot.GetHex() << std::endl;

        // exit(0);

        // /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(1616082640, 178720, 0x1efffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000034fe1805f04a440c06c1b96b1234b45d9fdba0fcb055d5caf4d83689428c"));
        assert(genesis.hashMerkleRoot == uint256S("0xbeb39df5e5c3029bb4a6d66357a41d06c275ea69a96c9081a3e6f8d2f8efe918"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 12;   // trittium starting difficulty is 1 / 2^12
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144; // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3; // (only 8-blocks window for finalization on testnet)
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 500000000 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nStakeMinDepthV2 = 200;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        // spork keys
        consensus.strSporkPubKey = "04ae5f52611fc9f3dcd3f2db88ad4a9b10ab218389cacd1ed07cc893af6ad2f3e6618090edb54038e5a26d2c6d04db4a6c901a8bf52a72fef484f478519fdf0b02"; // Testnet spork key
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint    = DISABLED;
        consensus.height_last_ZC_WrappedSerials     = DISABLED;
        consensus.height_start_InvalidUTXOsCheck    = DISABLED;
        consensus.height_start_ZC_InvalidSerials    = DISABLED;
        consensus.height_start_ZC_SerialRangeCheck  = DISABLED;
        consensus.height_ZC_RecalcAccumulators      = DISABLED;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 20;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 200;
        consensus.ZC_TimeStart = 1501776000;
        consensus.ZC_WrappedSerialsSupply = 0;   // WrappedSerials only on main net

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight                      = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight                 = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight                       = 501;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight                    = 721;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight                        = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight                     = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight                     = 721;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight                 = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MODIFIER_V2].nActivationHeight         = 1541;
        consensus.vUpgrades[Consensus::UPGRADE_TIME_PROTOCOL_V2].nActivationHeight          = 1641;
        consensus.vUpgrades[Consensus::UPGRADE_P2PKH_BLOCK_SIGNATURES].nActivationHeight    = 1741;
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MIN_DEPTH_V2].nActivationHeight        = 1841;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock                      = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock                   = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock                   = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock               = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MODIFIER_V2].hashActivationBlock       = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_TIME_PROTOCOL_V2].hashActivationBlock        = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_P2PKH_BLOCK_SIGNATURES].hashActivationBlock  = uint256S("0x0");
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MIN_DEPTH_V2].hashActivationBlock      = uint256S("0x0");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x32;
        pchMessageStart[1] = 0x42;
        pchMessageStart[2] = 0x52;
        pchMessageStart[3] = 0xf2;
        nDefaultPort = 41001;

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("testnet1", "testnet1.4444.tools", true));
        vSeeds.push_back(CDNSSeedData("testnet2", "testnet2.4444.tools", true));
        vSeeds.push_back(CDNSSeedData("testnet.572133.club", "testnet.572133.club", true));
        vSeeds.push_back(CDNSSeedData("testnet2.572133.club", "testnet2.572133.club", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 127); // Testnet Trittium addresses start with 't'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet Trittium script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet Trittium BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet Trittium BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet Trittium BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
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

        // // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
        // /////////////////////////////////////////////////////////////////

        // uint32_t nGenesisTime = 1616082641; // Thu Mar 18 2021 15:50:41 GMT+0000

        // arith_uint256 test;
        // bool fNegative;
        // bool fOverflow;
        // test.SetCompact(0x1fffffff, &fNegative, &fOverflow);
        // std::cout << "Test threshold: " << test.GetHex() << "\n\n";

        // int genesisNonce = 0;
        // uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        // uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // for (int i=0;i<40000000;i++) {
        //     genesis = CreateGenesisBlock(nGenesisTime, i, 0x1fffffff, 1, 0 * COIN);
        //     //genesis.hashPrevBlock = TempHashHolding;
        //     consensus.hashGenesisBlock = genesis.GetHash();

        //     arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
        //     if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
        //         BestBlockHash = consensus.hashGenesisBlock;
        //         std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
        //         std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        //     }

        //     TempHashHolding = consensus.hashGenesisBlock;

        //     if (BestBlockHashArith < test) {
        //         genesisNonce = i - 1;
        //         break;
        //     }
        //     //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        // }
        // std::cout << "\n";
        // std::cout << "\n";
        // std::cout << "\n";

        // std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        // std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        // std::cout << "Genesis Merkle 0x" << genesis.hashMerkleRoot.GetHex() << std::endl;

        // exit(0);

        // /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(1616082641, 333, 0x1fffffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x003279ba296f0376d7a8417354e0ef9bd00fa29f92ac5d9cdf97c160673474af"));
        assert(genesis.hashMerkleRoot == uint256S("0xbeb39df5e5c3029bb4a6d66357a41d06c275ea69a96c9081a3e6f8d2f8efe918"));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit   = ~UINT256_ZERO >> 2;   // trittium starting difficulty is 1 / 2^2
        consensus.posLimitV1 = ~UINT256_ZERO >> 24;
        consensus.posLimitV2 = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 5;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMasternodeCountDrift = 4;        // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut = 43199500 * COIN;
        consensus.nPoolMaxTransactions = 2;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 2;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "04d45416e4a64b1b051e2a2ebd80ced5efe148cf5fbcb70e56860957675a2da1a21fd522c42c1ed18a1ec42641589a09cf3f58678d213825dc21798183a005a984"; // Sapphire testnet spork key
        consensus.strSporkPubKeyOld = "04d45416e4a64b1b051e2a2ebd80ced5efe148cf5fbcb70e56860957675a2da1a21fd522c42c1ed18a1ec42641589a09cf3f58678d213825dc21798183a005a984"; // Sapphire testnet old spork key
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // height based activations
        consensus.height_last_ZC_AccumCheckpoint = 310;     // no checkpoints on regtest
        consensus.height_last_ZC_WrappedSerials = -1;
        consensus.height_start_InvalidUTXOsCheck = 999999999;
        consensus.height_start_ZC_InvalidSerials = 999999999;
        consensus.height_start_ZC_SerialRangeCheck = 300;
        consensus.height_ZC_RecalcAccumulators = 999999999;

        // Zerocoin-related params
        consensus.ZC_Modulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                "31438167899885040445364023527381951378636564391212010397122822120720357";
        consensus.ZC_MaxPublicSpendsPerTx = 637;    // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx = 7;            // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations = 10;
        consensus.ZC_MinMintFee = 1 * CENT;
        consensus.ZC_MinStakeDepth = 10;
        consensus.ZC_TimeStart = 0;                 // not implemented on regtest
        consensus.ZC_WrappedSerialsSupply = 0;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight 			            = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight 	            = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight                       = 151;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight                    = 251;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight                        = 300;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight                     = 300;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight                     = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight                 = 400;
        consensus.vUpgrades[Consensus::UPGRADE_STAKE_MODIFIER_V2].nActivationHeight         = 251;
        consensus.vUpgrades[Consensus::UPGRADE_TIME_PROTOCOL_V2].nActivationHeight          = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_P2PKH_BLOCK_SIGNATURES].nActivationHeight    = 300;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x33;
        pchMessageStart[1] = 0x43;
        pchMessageStart[2] = 0x53;
        pchMessageStart[3] = 0xf3;
        nDefaultPort = 51001;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

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

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}

