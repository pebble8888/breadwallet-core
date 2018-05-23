//
//  BREthereumLightNodePrivate.h
//  BRCore
//
//  Created by Ed Gamble on 5/7/18.
//  Copyright (c) 2018 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#ifndef BR_Ethereum_Light_Node_Private_H
#define BR_Ethereum_Light_Node_Private_H

#include <pthread.h>
#include "BREthereumLightNode.h"
#include "../blockchain/BREthereumBlockChain.h"
#include "../event/BREvent.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// Light Node Listener
//
typedef struct  {
    BREthereumListenerContext context;
    BREthereumListenerLightNodeEventHandler lightNodeEventHandler;
    BREthereumListenerPeerEventHandler peerEventHandler;
    BREthereumListenerWalletEventHandler walletEventHandler;
    BREthereumListenerBlockEventHandler blockEventHandler;
    BREthereumListenerTransactionEventHandler transactionEventHandler;
} BREthereumLightNodeListener;

extern void
lightNodeListenerAnnounceWalletEvent(BREthereumLightNode node,
                                     BREthereumWalletId wid,
                                     BREthereumWalletEvent event,
                                     BREthereumStatus status,
                                     const char *errorDescription);

extern void
lightNodeListenerAnnounceBlockEvent(BREthereumLightNode node,
                                    BREthereumBlockId bid,
                                    BREthereumBlockEvent event,
                                    BREthereumStatus status,
                                    const char *errorDescription);

extern void
lightNodeListenerAnnounceTransactionEvent(BREthereumLightNode node,
                                          BREthereumWalletId wid,
                                          BREthereumTransactionId tid,
                                          BREthereumTransactionEvent event,
                                          BREthereumStatus status,
                                          const char *errorDescription);

extern void
lightNodeListenerAnnouncePeerEvent(BREthereumLightNode node,
                                   // BREthereumWalletId wid,
                                   // BREthereumTransactionId tid,
                                   BREthereumPeerEvent event,
                                   BREthereumStatus status,
                                   const char *errorDescription);

extern void
lightNodeListenerAnnounceLightNodeEvent(BREthereumLightNode node,
                                        // BREthereumWalletId wid,
                                        // BREthereumTransactionId tid,
                                        BREthereumLightNodeEvent event,
                                        BREthereumStatus status,
                                        const char *errorDescription);

extern const BREventType *listenerEventTypes[];
extern const unsigned int listenerEventTypesCount;

//
// Light Node
//

#define DEFAULT_LISTENER_CAPACITY 3
#define DEFAULT_WALLET_CAPACITY 10
#define DEFAULT_BLOCK_CAPACITY 100
#define DEFAULT_TRANSACTION_CAPACITY 1000

typedef enum {
    LIGHT_NODE_CREATED,
    LIGHT_NODE_CONNECTING,
    LIGHT_NODE_CONNECTED,
    LIGHT_NODE_DISCONNECTING,
    LIGHT_NODE_DISCONNECTED,
    LIGHT_NODE_ERRORED
} BREthereumLightNodeState;

/**
 *
 */
struct BREthereumLightNodeRecord {
    /**
     * The State
     */
    BREthereumLightNodeState state;

    /**
     * The Type of this Light Node
     */
    BREthereumType type;

    /**
     * The network
     */
    BREthereumNetwork network;

    /**
     * The Client supporting this Light Node
     */
    BREthereumClient client;

    /**
     * The account
     */
    BREthereumAccount account;

    /**
     * The wallets 'managed/handled' by this node.  There can be only one wallet holding ETHER;
     * all the other wallets hold TOKENs and only one wallet per TOKEN.
     */
    BREthereumWallet *wallets;  // for now
    BREthereumWallet  walletHoldingEther;

    /**
     * The transactions seen/handled by this node.  These are used *solely* for the TransactionId
     * interface in LightNode.  *All* transactions must be accesses through their wallet.
     */
    BREthereumTransaction *transactions; // BRSet

    /**
     * The blocks handled by this node.  [This is currently just those handled for transactions
     * (both Ethererum transactions and logs.  It is unlikely that the current block is here.]
     */
    BREthereumBlock *blocks; // BRSet

    /**
     * The BlockHeight is the largest block number seen or computed.  [Note: the blockHeight may
     * be computed from a Log event as (log block number + log confirmations).
     */
    uint64_t blockHeight;

    /**
     * An identiifer for a LES/JSON_RPC Request
     */
    unsigned int requestId;

    /**
     * The listeners
     */
    BREthereumLightNodeListener *listeners; // BRArray

    /**
     * An EventHandler for Listeners.  All callbacks to the Listener interface occur on a
     * separate thread.
     */
    BREventHandler handlerForListener;

    /**
     * An EventHandler for Main.  All 'announcements' (via LES (or JSON_RPC) hit here.
     */
    BREventHandler handlerForMain;

    /**
     * The Lock ensuring single thread access to Node state.
     */
    pthread_mutex_t lock;
};

extern BREthereumWalletId
lightNodeLookupWalletId(BREthereumLightNode node,
                        BREthereumWallet wallet);

extern BREthereumWallet
lightNodeLookupWalletByTransaction (BREthereumLightNode node,
                                    BREthereumTransaction transaction);
    
extern BREthereumWalletId
lightNodeInsertWallet (BREthereumLightNode node,
                       BREthereumWallet wallet);

extern BREthereumBlockId
lightNodeLookupBlockId (BREthereumLightNode node,
                        BREthereumBlock block);

extern BREthereumBlockId
lightNodeInsertBlock (BREthereumLightNode node,
                      BREthereumBlock block);

extern BREthereumTransactionId
lightNodeLookupTransactionId(BREthereumLightNode node,
                             BREthereumTransaction transaction);

extern BREthereumTransactionId
lightNodeInsertTransaction (BREthereumLightNode node,
                            BREthereumTransaction transaction);

//
// Handlers
//
extern const BREventType *handlerEventTypes[];
extern const unsigned int handlerEventTypesCount;

extern void
lightNodeHandleBalance (BREthereumLightNode node,
                        BREthereumAmount amount);

extern void
lightNodeHandleNonce (BREthereumLightNode node,
                      uint64_t nonce);

extern void
lightNodeHandleGasPrice (BREthereumLightNode node,
                         BREthereumWallet wallet,
                         BREthereumGasPrice gasPrice);

extern void
lightNodeHandleGasEstimate (BREthereumLightNode node,
                            BREthereumWallet wallet,
                            BREthereumTransaction transaction,
                            BREthereumGas gasEstimate);

extern void
lightNodeHandleTransactionStatus (BREthereumLightNode node,
                                  BREthereumHash transactionHash,
                                  BREthereumTransactionStatusLES status);

extern void
lightNodeHandleTransactionReceipt (BREthereumLightNode node,
                                   BREthereumHash blockHash,
                                   BREthereumTransactionReceipt receipt,
                                   unsigned int receiptIndex);

extern void
lightNodeHandleAnnounce (BREthereumLightNode node,
                         BREthereumHash headHash,
                         uint64_t headNumber,
                         uint64_t headTotalDifficulty);

extern void
lightNodeHandleBlockHeader (BREthereumLightNode node,
                            BREthereumBlockHeader header);

extern void
lightNodeHandleBlockBodies (BREthereumLightNode node,
                            BREthereumHash blockHash,
                            BREthereumTransaction transactions[],
                            BREthereumHash ommers[]);
    
#ifdef __cplusplus
}
#endif

#endif //BR_Ethereum_Light_Node_Private_H
