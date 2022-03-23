#include "helpers.hpp"
#include "kawpow_test_vectors.hpp"

#include <ethash/endianness.hpp>
#include <ethash/progpow.hpp>
#include <gtest/gtest.h>

#include <array>

TEST(kawpow, hash_and_verify)
{
    ethash::epoch_context_ptr context{nullptr, nullptr};

    for (auto& t : kawpow_hash_test_cases)
    {
        const auto epoch_number = ethash::get_epoch_number<progpow::kawpow_traits>(t.block_number);
        if (!context || context->epoch_number != epoch_number)
            context = ethash::create_epoch_context<progpow::kawpow_traits>(epoch_number);

        const auto header_hash = to_hash256(t.header_hash_hex);
        const auto nonce = std::stoull(t.nonce_hex, nullptr, 16);
        const auto result =
            progpow::hash<progpow::kawpow_traits>(*context, t.block_number, header_hash, nonce);
        EXPECT_EQ(to_hex(result.mix_hash), t.mix_hash_hex);
        EXPECT_EQ(to_hex(result.final_hash), t.final_hash_hex);

        auto success = progpow::verify<progpow::kawpow_traits>(
            *context, t.block_number, header_hash, result.mix_hash, nonce, result.final_hash);
        EXPECT_TRUE(success);

        auto lower_boundary = result.final_hash;
        --lower_boundary.bytes[31];
        auto final_failure = progpow::verify<progpow::kawpow_traits>(
            *context, t.block_number, header_hash, result.mix_hash, nonce, lower_boundary);
        EXPECT_FALSE(final_failure);

        auto different_mix = result.mix_hash;
        ++different_mix.bytes[7];
        auto mix_failure = progpow::verify<progpow::kawpow_traits>(
            *context, t.block_number, header_hash, different_mix, nonce, result.final_hash);
        EXPECT_FALSE(mix_failure);
    }
}