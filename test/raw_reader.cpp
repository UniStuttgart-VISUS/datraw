// <copyright file="raw_reader.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(RawReader) {

    public:

        TEST_METHOD(TestReadSingleFrameChar) {
            this->testReadSingleFrame<char>();
        }

        TEST_METHOD(TestReadSingleFrameWchar) {
            this->testReadSingleFrame<wchar_t>();
        }

        TEST_METHOD(TestOffsetFixChar) {
            this->testOffsetFix<char>();
        }

        TEST_METHOD(TestOffsetFixWchar) {
            this->testOffsetFix<wchar_t>();
        }

        TEST_METHOD(TestOffsetArgCheckChar) {
            this->testOffsetArgCheck<char>();
        }

        TEST_METHOD(TestOffsetArgCheckWchar) {
            this->testOffsetArgCheck<wchar_t>();
        }

    private:

        template<class C> void testReadSingleFrame(void) {
            typedef datraw::info<C> info;
            typedef datraw::raw_reader<C> raw_reader;

            std::vector<std::uint8_t> expected(256 * 256 * 256);
            std::generate(expected.begin(), expected.end(), std::rand);

            {
                std::fstream stream("test.raw", std::ios::out
                    | std::ios::binary);
                stream.write(reinterpret_cast<const char *>(expected.data()),
                    expected.size());
                stream.close();
            }

            auto dat = DATRAW_TPL_LITERAL(C, "\
ObjectFileName: test.raw\n\
Resolution: 256 256 256\n\
Format: CHAR\n\
GridType: CARTESIAN\n\
");

            raw_reader reader(info::parse(dat));
            Assert::IsTrue(reader, L"The reader has a frame.", LINE_INFO());

            auto actual = reader.read_current();
            Assert::IsTrue(expected == actual, L"Reader retrieved expected data.", LINE_INFO());

            Assert::IsFalse(reader.move_next(), L"The reader reached the end.", LINE_INFO());
        }

        template<class C> void testOffsetFix(void) {
            typedef datraw::info<C> info;
            typedef datraw::raw_reader<C> raw_reader;

            std::vector<std::uint8_t> expected(8 + 256 * 256 * 256);
            std::generate(expected.begin(), expected.end(), std::rand);

            {
                std::fstream stream("test.raw", std::ios::out
                    | std::ios::binary);
                stream.write(reinterpret_cast<const char *>(expected.data()),
                    expected.size());
                stream.close();
            }

            expected.erase(expected.begin(), expected.begin() + 8);

            auto dat = DATRAW_TPL_LITERAL(C, "\
ObjectFileName: test.raw\n\
Resolution: 256 256 256\n\
Format: CHAR\n\
GridType: CARTESIAN\n\
DataOffset: 8\n\
");

            raw_reader reader(info::parse(dat));
            Assert::IsTrue(reader, L"The reader has a frame.", LINE_INFO());

            auto actual = reader.read_current();
            Assert::IsTrue(expected == actual, L"Reader retrieved expected data.", LINE_INFO());

            Assert::IsFalse(reader.move_next(), L"The reader reached the end.", LINE_INFO());
        }

        template<class C> void testOffsetArgCheck(void) {
            typedef datraw::info<C> info;
            typedef datraw::raw_reader<C> raw_reader;

            std::vector<std::uint8_t> expected(256 * 256 * 256);
            std::generate(expected.begin(), expected.end(), std::rand);

            {
                std::fstream stream("test.raw", std::ios::out
                    | std::ios::binary);
                stream.write(reinterpret_cast<const char *>(expected.data()),
                    expected.size());
                stream.close();
            }

            auto dat = DATRAW_TPL_LITERAL(C, "\
ObjectFileName: test.raw\n\
Resolution: 256 256 256\n\
Format: CHAR\n\
GridType: CARTESIAN\n\
DataOffset: 16777217\n\
");

            raw_reader reader(info::parse(dat));
            Assert::IsTrue(reader, L"The reader has a frame.", LINE_INFO());

            Assert::ExpectException<std::invalid_argument>([&]() {
                auto actual = reader.read_current();
                }, L"Illegal data offset is recognised.", LINE_INFO());

        }

    };
}
