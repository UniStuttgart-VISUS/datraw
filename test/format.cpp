// <copyright file="format.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2017 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(Format) {

    public:

        TEST_METHOD(TestFormatChar) {
            this->testFormat<char>();
        }

        TEST_METHOD(TestFormatWchar) {
            this->testFormat<wchar_t>();
        }

    private:

        template<class C> void testFormat(void) {
            {
                std::basic_string<C> fmt(DATRAW_TPL_LITERAL(C, "fmt"));
                auto expected = fmt;
                auto actual = datraw::detail::format(fmt);
                Assert::AreEqual(expected, actual, L"Format without variables", LINE_INFO());
            }

            {
                std::basic_string<C> fmt(DATRAW_TPL_LITERAL(C, "%d"));
                std::basic_string<C> expected(DATRAW_TPL_LITERAL(C, "1"));
                auto actual = datraw::detail::format(fmt, 1);
                Assert::AreEqual(expected, actual, L"Format int", LINE_INFO());
            }

            {
                std::basic_string<C> fmt(DATRAW_TPL_LITERAL(C, "test %hs"));
                std::basic_string<C> expected(DATRAW_TPL_LITERAL(C, "test test"));
                auto actual = datraw::detail::format(fmt, "test");
                Assert::AreEqual(expected, actual, L"Format string", LINE_INFO());
            }

        }

    };
}
