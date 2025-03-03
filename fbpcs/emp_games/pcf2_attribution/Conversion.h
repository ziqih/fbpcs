/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "fbpcs/emp_games/common/Constants.h"
#include "fbpcs/emp_games/pcf2_attribution/Constants.h"

namespace pcf2_attribution {

template <bool usingBatch>
struct Conversion {
  ConditionalVector<uint64_t, usingBatch> ts;
};

template <bool usingBatch>
using ConversionT = ConditionalVector<Conversion<usingBatch>, !usingBatch>;

template <
    int schedulerId,
    bool usingBatch,
    common::InputEncryption inputEncryption>
struct PrivateConversion {
  SecTimestamp<schedulerId, usingBatch> ts;

  explicit PrivateConversion(const Conversion<usingBatch>& conversion) {
    if constexpr (inputEncryption == common::InputEncryption::Plaintext) {
      ts =
          SecTimestamp<schedulerId, usingBatch>(conversion.ts, common::PARTNER);
    } else {
      typename SecTimestamp<schedulerId, usingBatch>::ExtractedInt extractedTs(
          conversion.ts);
      ts = SecTimestamp<schedulerId, usingBatch>(std::move(extractedTs));
    }
  }
};

// Used for parsing conversions from input CSV files
struct ParsedConversion {
  uint64_t ts;

  bool operator<(const ParsedConversion& conv) const {
    return (ts < conv.ts);
  }
};

} // namespace pcf2_attribution
