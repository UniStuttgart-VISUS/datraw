#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(Info) {

    public:

        TEST_METHOD(TestParseChar) {
            this->testParse<char>();
        }

        TEST_METHOD(TestParseWchar) {
            this->testParse<wchar_t>();
        }

    private:

        template<class C> void testParse(void) {
            typedef datraw::info<C> info;

            {
                auto input = DATRAW_TPL_LITERAL(C, "");
                auto fn = [&input] {
                    auto i = info::parse(input);
                };
                Assert::ExpectException<std::runtime_error>(fn, L"Empty string yields invalid info.");
            }

            {
                auto input = DATRAW_TPL_LITERAL(C, "");

            }
        }
    };
}
