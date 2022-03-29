#include <ethash/ethash.hpp>
#include <ethash/progpow.hpp>
#include <gtest/gtest.h>

#include "firopow_test_vectors.hpp"
#include "helpers.hpp"
#include "kawpow_test_vectors.hpp"
#include "test_cases.hpp"

ethash::epoch_context_ptr context{nullptr, nullptr};

TEST(update_local_context, ethash)
{
    auto& t = hash_test_cases[0];
    const auto epoch_number = ethash::get_epoch_number<ethash::ethash_traits>(t.block_number);
    if (!context || context->epoch_number != epoch_number)
        context = ethash::create_epoch_context<ethash::ethash_traits>(epoch_number);

    const auto header_hash = to_hash256(t.header_hash_hex);
    const auto nonce = std::stoull(t.nonce_hex, nullptr, 16);
    const auto result = ethash::hash(*context, header_hash, nonce);
    EXPECT_EQ(to_hex(result.mix_hash), t.mix_hash_hex);
    EXPECT_EQ(to_hex(result.final_hash), t.final_hash_hex);

    auto success = ethash::verify(*context, header_hash, result.mix_hash, nonce, result.final_hash);
    EXPECT_TRUE(success);

    auto lower_boundary = result.final_hash;
    --lower_boundary.bytes[31];
    auto final_failure =
        ethash::verify(*context, header_hash, result.mix_hash, nonce, lower_boundary);
    EXPECT_FALSE(final_failure);

    auto different_mix = result.mix_hash;
    ++different_mix.bytes[7];
    auto mix_failure =
        ethash::verify(*context, header_hash, different_mix, nonce, result.final_hash);
    EXPECT_FALSE(mix_failure);
}

TEST(update_local_context, kawpow)
{
    auto& t = kawpow_hash_test_cases[0];
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

TEST(update_local_context, firopow)
{
    auto& t = firopow_hash_test_cases[0];
    const auto epoch_number = ethash::get_epoch_number<progpow::firopow_traits>(t.block_number);
    if (!context || context->epoch_number != epoch_number)
        context = ethash::create_epoch_context<progpow::firopow_traits>(epoch_number);

    const auto header_hash = to_hash256(t.header_hash_hex);
    const auto nonce = std::stoull(t.nonce_hex, nullptr, 16);
    const auto result =
        progpow::hash<progpow::firopow_traits>(*context, t.block_number, header_hash, nonce);
    EXPECT_EQ(to_hex(result.mix_hash), t.mix_hash_hex);
    EXPECT_EQ(to_hex(result.final_hash), t.final_hash_hex);

    auto success = progpow::verify<progpow::firopow_traits>(
        *context, t.block_number, header_hash, result.mix_hash, nonce, result.final_hash);
    EXPECT_TRUE(success);

    auto lower_boundary = result.final_hash;
    --lower_boundary.bytes[31];
    auto final_failure = progpow::verify<progpow::firopow_traits>(
        *context, t.block_number, header_hash, result.mix_hash, nonce, lower_boundary);
    EXPECT_FALSE(final_failure);

    auto different_mix = result.mix_hash;
    ++different_mix.bytes[7];
    auto mix_failure = progpow::verify<progpow::firopow_traits>(
        *context, t.block_number, header_hash, different_mix, nonce, result.final_hash);
    EXPECT_FALSE(mix_failure);
}
