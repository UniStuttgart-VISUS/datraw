#include "stdafx.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(Convert) {

    public:

        TEST_METHOD(TestFloatToDouble) {
            std::array<float, 256> in;
            std::array<double, 256> out;

            std::generate(in.begin(), in.end(),
                [n = 0]() mutable { return float(n++) / 255.0f; });

            datraw::convert(in.cbegin(), in.cend(), out.begin());

            for (size_t i = 0; i < out.size(); ++i) {
                Assert::AreEqual(double(in[i]), out[i], 0.0001, L"Value casted.", LINE_INFO());
            }
        }

        TEST_METHOD(TestFloatToUchar) {
            std::array<float, 256> in;
            std::array<unsigned char, 256> out;

            std::generate(in.begin(), in.end(),
                [n = 0]() mutable { return float(n++) / 255.0f; });

            datraw::convert(in.cbegin(), in.cend(), out.begin());

            for (size_t i = 0; i < out.size(); ++i) {
                Assert::AreEqual((unsigned char) i, out[i], L"Value converted correctly.", LINE_INFO());
            }
        }

        TEST_METHOD(TestMove) {
            std::array<unsigned char, 256> in;
            std::array<unsigned char, 256> out;

            std::iota(in.begin(), in.end(), 0);

            datraw::convert(in.cbegin(), in.cend(), out.begin());

            for (size_t i = 0; i < out.size(); ++i) {
                Assert::AreEqual(in[i], out[i], L"Value copied correctly.", LINE_INFO());
            }
        }

        TEST_METHOD(TestUcharToFloat) {
            std::array<unsigned char, 256> in;
            std::array<float, 256> out;

            std::iota(in.begin(), in.end(), 0);

            datraw::convert(in.cbegin(), in.cend(), out.begin());

            for (size_t i = 0; i < out.size(); ++i) {
                Assert::AreEqual(float(i) / 255.0f, out[i], 0.0001f, L"Value converted correctly.", LINE_INFO());
            }
        }

        TEST_METHOD(TestUcharToUint) {
            std::array<unsigned char, 256> in;
            std::array<unsigned int, 256> out;

            std::iota(in.begin(), in.end(), 0);

            datraw::convert(in.cbegin(), in.cend(), out.begin());

            for (size_t i = 0; i < out.size(); ++i) {
                Assert::AreEqual((unsigned int) (double(in[i]) / UCHAR_MAX * UINT_MAX), out[i], L"Widening conversion.", LINE_INFO());
            }
        }

        TEST_METHOD(TestUintToUchar) {
            std::array<unsigned int, 256> in;
            std::array<unsigned char, 256> out;

            std::iota(in.begin(), in.end(), 0);

            datraw::convert(in.cbegin(), in.cend(), out.begin());

            for (size_t i = 0; i < out.size(); ++i) {
                Assert::AreEqual((unsigned char) (double(in[i]) / UINT_MAX * UCHAR_MAX), out[i], L"Narrowing conversion.", LINE_INFO());
            }
        }


    };
}
