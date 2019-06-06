/*
 * Created by Michael Carrara <michael.carrara@breadwallet.com> on 5/31/18.
 * Copyright (c) 2018 Breadwinner AG.  All right reserved.
 *
 * See the LICENSE file at the project root for license information.
 * See the CONTRIBUTORS file at the project root for a list of contributors.
 */
package com.breadwallet.corenative.crypto;

import com.breadwallet.corenative.CryptoLibrary;

public interface CoreBRCryptoCurrency {

    static CoreBRCryptoCurrency create(String name, String code, String type) {
        return new OwnedBRCryptoCurrency(CryptoLibrary.INSTANCE.cryptoCurrencyCreate(name, code, type));
    }

    String getName();

    String getCode();

    String getType();

    BRCryptoCurrency asBRCryptoCurrency();
}
