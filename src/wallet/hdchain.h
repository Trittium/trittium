// Copyright (c) 2020 The PIVX developers
// Copyright (c) 2021 The DECENOMY Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_HDCHAIN_H
#define PIVX_HDCHAIN_H

#include "key.h"

namespace HDChain {
    namespace ChangeType {
        static const uint8_t EXTERNAL   = 0;
        static const uint8_t INTERNAL   = 1;
        static const uint8_t STAKING    = 2; // obsolete
        static const uint8_t ECOMMERCE  = 3;
    };

    namespace ChainCounterType {
        static const uint8_t Standard  = 0;
    };
}

/* Simple HD chain data model */
class CHDChain
{
private:
    int nVersion;
    CKeyID seed_id;

public:
    // Standard hd chain
    static const int CURRENT_VERSION = 3;
    // Single account counters.
    uint32_t nExternalChainCounter{0};
    uint32_t nInternalChainCounter{0};
    uint32_t nECommerceChainCounter{0};
    // Chain counter type
    uint8_t chainType;

    CHDChain(const uint8_t& _chainType = HDChain::ChainCounterType::Standard) : chainType(_chainType) { SetNull(); }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action)
    {
        READWRITE(nVersion);
        READWRITE(seed_id);
        READWRITE(nExternalChainCounter);
        READWRITE(nInternalChainCounter);
        uint32_t nStakingChainCounter{0};
        READWRITE(nStakingChainCounter);
        if (nVersion == 3) {
            READWRITE(nECommerceChainCounter);
        }
        if (nVersion == 1) chainType = HDChain::ChainCounterType::Standard;
        else READWRITE(chainType);
    }

    bool SetNull();
    bool IsNull() const;

    bool SetSeed(const CKeyID& seedId);
    CKeyID GetID() const { return seed_id; }

    uint32_t& GetChainCounter(const uint8_t& type = HDChain::ChangeType::EXTERNAL) {
        switch (type) {
            case HDChain::ChangeType::EXTERNAL:
                return nExternalChainCounter;
            case HDChain::ChangeType::INTERNAL:
                return nInternalChainCounter;
            case HDChain::ChangeType::ECOMMERCE:
                return nECommerceChainCounter;
            default:
                throw std::runtime_error("HD chain type doesn't exist.");
        }
    }
};

#endif // PIVX_HDCHAIN_H
