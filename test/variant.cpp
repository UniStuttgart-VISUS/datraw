#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(Variant) {

    public:

        TEST_METHOD(TestSettersChar) {
            using datraw::variant_type;
            typedef datraw::variant variant;

            {
                variant v;
                Assert::AreEqual((int) v.type(), (int) variant_type::empty);
            }

            {
                bool value = true;
                variant v(value);
                Assert::AreEqual((int) v.type(), (int) variant_type::boolean);
                Assert::AreEqual(v.get<decltype(value)>(), value);
            }

            {
                datraw::int8 value = 42;
                variant v(value);
                Assert::AreEqual((int) v.type(), (int) variant_type::int8);
                Assert::AreEqual(v.get<decltype(value)>(), value);
            }

            {
                datraw::int16 value = 42;
                variant v(value);
                Assert::AreEqual((int)v.type(), (int)variant_type::int16);
                Assert::AreEqual(v.get<decltype(value)>(), value);
            }

            {
                datraw::int32 value = 42;
                variant v(value);
                Assert::AreEqual((int)v.type(), (int)variant_type::int32);
                Assert::AreEqual(v.get<decltype(value)>(), value);
            }

            //{
            //    datraw::int64 value = 42;
            //    variant v(value);
            //    Assert::AreEqual((int)v.type(), (int)variant_type::int64);
            //    Assert::AreEqual(v.get<decltype(value)>(), value);
            //}

            {
                datraw::uint8 value = 42;
                variant v(value);
                Assert::AreEqual((int)v.type(), (int)variant_type::uint8);
                Assert::AreEqual(v.get<decltype(value)>(), value);
            }

            //{
            //    datraw::uint16 value = 42;
            //    variant v(value);
            //    Assert::AreEqual((int)v.type(), (int)variant_type::uint16);
            //    Assert::AreEqual(v.get<decltype(value)>(), value);
            //}

            {
                datraw::uint32 value = 42;
                variant v(value);
                Assert::AreEqual((int)v.type(), (int)variant_type::uint32);
                Assert::AreEqual(v.get<decltype(value)>(), value);
            }

            //{
            //    datraw::uint64 value = 42;
            //    variant v(value);
            //    Assert::AreEqual((int)v.type(), (int)variant_type::uint64);
            //    Assert::AreEqual(v.get<decltype(value)>(), value);
            //}


            /*
            Assert::AreEqual(v.)

            bool val_boolean;
            datraw::int8 val_int8;
            datraw::int16 val_int16;
            datraw::int32 val_int32;
            datraw::int64 val_int64;
            datraw::uint8 val_uint8;
            datraw::uint16 val_uint16;
            datraw::uint32 val_uint32;
            datraw::uint64 val_uint64;
            datraw::float32 val_float32;
            datraw::float64 val_float64;
            std::basic_string<C> val_string;
            void *val_pointer;
            */
        }

        TEST_METHOD(TestSettersWchar) {
            using datraw::variant_type;
            typedef datraw::wvariant variant;
            variant v;
        }

    };
}
