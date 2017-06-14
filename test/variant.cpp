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

            {
                datraw::uint32 value = 42;
                variant v1(value);
                variant v2 = v1;
                Assert::IsTrue(v1 == v2);
                Assert::IsTrue(v2 == v1);
                Assert::IsFalse(v1 != v2);
                Assert::IsFalse(v2 != v1);
            }
        }

        TEST_METHOD(TestSettersWchar) {
            using datraw::variant_type;
            typedef datraw::wvariant variant;

            {
                variant v;
                Assert::AreEqual((int)v.type(), (int)variant_type::empty);
            }

            {
                bool value = true;
                variant v(value);
                Assert::AreEqual((int)v.type(), (int)variant_type::boolean);
                Assert::AreEqual(v.get<decltype(value)>(), value);
            }

            {
                datraw::int8 value = 42;
                variant v(value);
                Assert::AreEqual((int)v.type(), (int)variant_type::int8);
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

            {
                datraw::uint32 value = 42;
                variant v1(value);
                variant v2 = v1;
                Assert::IsTrue(v1 == v2);
                Assert::IsTrue(v2 == v1);
                Assert::IsFalse(v1 != v2);
                Assert::IsFalse(v2 != v1);
            }
        }
    };
}
