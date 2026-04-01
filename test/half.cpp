// <copyright file="half.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "stdafx.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(Half) {

    public:

        TEST_METHOD(Construction) {
            {
                datraw::half h;
                Assert::AreEqual(0.0f, static_cast<float>(h), L"Default constructor should create zero.", LINE_INFO());
            }
            {
                datraw::half h(-1.0f);
                Assert::AreEqual(-1.0f, static_cast<float>(h), L"-1.0", LINE_INFO());
            }
            {
                datraw::half h(1.0f);
                Assert::AreEqual(1.0f, static_cast<float>(h), L"1.0", LINE_INFO());
            }
            {
                datraw::half h(0.5f);
                Assert::AreEqual(0.5f, static_cast<float>(h), L"0.5", LINE_INFO());
            }
            {
                datraw::half h(-0.5f);
                Assert::AreEqual(-0.5f, static_cast<float>(h), L"-0.5", LINE_INFO());
            }
            {
                datraw::half h(-2.0f);
                Assert::AreEqual(-2.0f, static_cast<float>(h), L"-2.0", LINE_INFO());
            }
            {
                datraw::half h(2.0f);
                Assert::AreEqual(2.0f, static_cast<float>(h), L"2.0", LINE_INFO());
            }
        }

        TEST_METHOD(IeeeHalf) {
            typedef datraw::detail::half_converter<false, false> converter;

            {
                constexpr auto h = std::uint16_t(0x0000);
                const auto f = converter::to_float(h);
                Assert::AreEqual(0.0f, f, L"+0", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"+0", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0x8000);
                const auto f = converter::to_float(h);
                Assert::AreEqual(-0.0f, f, L"-0", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"-0", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0x7C00);
                const auto f = converter::to_float(h);
                Assert::AreEqual(std::numeric_limits<float>::infinity(), f, L"+inf", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"+inf", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0xFC00);
                const auto f = converter::to_float(h);
                Assert::AreEqual(-std::numeric_limits<float>::infinity(), f, L"-inf", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0x7C01);
                const auto f = converter::to_float(h);
                Assert::IsTrue(std::isnan(f), L"nan", LINE_INFO());
                Assert::IsFalse(std::signbit(f), L"+nan", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0xFC01);
                const auto f = converter::to_float(h);
                Assert::IsTrue(std::isnan(f), L"nan", LINE_INFO());
                Assert::IsTrue(std::signbit(f), L"-nan", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0x7FFF);
                const auto f = converter::to_float(h);
                Assert::IsTrue(std::isnan(f), L"nan", LINE_INFO());
                Assert::IsFalse(std::signbit(f), L"+nan", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0xFFFF);
                const auto f = converter::to_float(h);
                Assert::IsTrue(std::isnan(f), L"nan", LINE_INFO());
                Assert::IsTrue(std::signbit(f), L"-nan", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0 + ((-14 + 15) << 10));
                constexpr auto e = datraw::detail::make_float_from_bits(0x3F800000u + (static_cast<std::uint32_t>(-14) << 23u));
                const auto f = converter::to_float(h);
                Assert::AreEqual(e.as_float, f, L"1024", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"1024", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(0x03FF + ((-14 + 15) << 10));
                constexpr auto e = datraw::detail::make_float_from_bits(0x3FFFE000 + (static_cast<std::uint32_t>(-14) << 23u));
                const auto f = converter::to_float(h);
                Assert::AreEqual(e.as_float, f, L"2047", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"2047", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t(1 + ((15 + 15) << 10));
                constexpr auto e = datraw::detail::make_float_from_bits(0x3F802000u + (static_cast<std::uint32_t>(15) << 23u));
                const auto f = converter::to_float(h);
                Assert::AreEqual(e.as_float, f, L"1024", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"1024", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t((0 + ((-14 + 15) << 10)) ^ UINT16_C(0x8000));
                constexpr auto e = datraw::detail::make_float_from_bits((0x3F800000u + (static_cast<std::uint32_t>(-14) << 23u)) ^ UINT16_C(0x80000000));
                const auto f = converter::to_float(h);
                Assert::AreEqual(e.as_float, f, L"33792", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"33792", LINE_INFO());
            }
            {
                constexpr auto h = std::uint16_t((1 + ((-14 + 15) << 10)) ^ UINT16_C(0x8000));
                constexpr auto e = datraw::detail::make_float_from_bits((0x3F802000u + (static_cast<std::uint32_t>(-14) << 23u)) ^ UINT16_C(0x80000000));
                const auto f = converter::to_float(h);
                Assert::AreEqual(e.as_float, f, L"33793", LINE_INFO());
                const auto g = converter::to_half(f);
                Assert::AreEqual(h, g, L"33793", LINE_INFO());
            }

        }

    };
}
