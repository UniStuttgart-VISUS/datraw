#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace test {

    TEST_CLASS(Variant) {

    public:

        TEST_METHOD(TestAssignmentChar) {
            this->testAssignment<char>();
        }

        TEST_METHOD(TestAssignmentWchar) {
            this->testAssignment<wchar_t>();
        }

        TEST_METHOD(TestEqualityChar) {
            this->testEquality<char>();
        }

        TEST_METHOD(TestEqualityWchar) {
            this->testEquality<wchar_t>();
        }

        TEST_METHOD(TestSettersChar) {
            this->testSetters<char>();
        }

        TEST_METHOD(TestSettersWchar) {
            this->testSetters<wchar_t>();
        }

    private:

        template<class C> void testAssignment(void) {
            using datraw::variant_type;
            typedef datraw::basic_variant<C> variant;

            {
                variant v1;
                Assert::IsTrue(v1.type() == variant_type::empty, L"Default constructed variant is empty.");

                variant v2(static_cast<float>(42));
                Assert::IsTrue(v2.type() == variant_type::float32, L"Initialised source variant as float32.");

                v1 = v2;
                Assert::IsTrue(v2.type() == variant_type::float32, L"Assignement changed target type.");
                Assert::AreEqual(v1.get<float>(), static_cast<float>(42), L"Assignement changed target value.");
                Assert::IsTrue(v1 == v2, L"Assignement yields equality.");
            }

            {
                variant v1;
                Assert::IsTrue(v1.type() == variant_type::empty, L"Default constructed variant is empty.");

                variant v2(static_cast<float>(42));
                Assert::IsTrue(v2.type() == variant_type::float32, L"Initialised source variant as float32.");

                v1 = std::move(v2);
                Assert::IsTrue(v1.type() == variant_type::float32, L"Move assignement changed target type.");
                Assert::IsTrue(v2.type() == variant_type::empty, L"Move assignement cleared source type.");
                Assert::AreEqual(v1.get<float>(), static_cast<float>(42), L"Move assignement changed target value.");
                Assert::IsTrue(v1 != v2, L"Move assignment does not yield equality.");
            }
        }

        template<class C> void testEquality(void) {
            using datraw::variant_type;
            typedef datraw::basic_variant<C> variant;

            {
                datraw::uint32 value = 42;
                variant v1(value);
                variant v2 = v1;
                Assert::IsTrue(v1 == v2, L"Equality of cloned variant.");
                Assert::IsTrue(v2 == v1, L"Equality of variant is commutative.");
                Assert::IsFalse(v1 != v2, L"Inequality of cloned variant.");
                Assert::IsFalse(v2 != v1, L"Inequality of variant is commutative.");
            }
        }

        template<class C> void testSetters(void) {
            using datraw::variant_type;
            typedef datraw::basic_variant<C> variant;

            {
                variant v;
                Assert::IsTrue(v.type() == variant_type::empty, L"Default constructed variant is empty.");
            }

            {
                bool value = true;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::boolean, L"Construction from boolean yields correct type.");
                Assert::AreEqual(v.get<decltype(value)>(), value, L"Correct boolean has been saved.");
            }

            {
                datraw::int8 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::int8, L"Construction from int8 yields correct type.");
                Assert::AreEqual(v.get<decltype(value)>(), value, L"Correct int8 has been saved.");
            }

            {
                datraw::int16 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::int16, L"Construction from int16 yields correct type.");
                Assert::AreEqual(v.get<decltype(value)>(), value, L"Correct int16 has been saved.");
            }

            {
                datraw::int32 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::int32, L"Construction from int32 yields correct type.");
                Assert::AreEqual(v.get<decltype(value)>(), value, L"Correct int32 has been saved.");
            }

            {
                datraw::int64 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::int64, L"Construction from int64 yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct int64 has been saved.");
            }

            {
                datraw::uint8 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::uint8, L"Construction from uint8 yields correct type.");
                Assert::AreEqual(v.get<decltype(value)>(), value, L"Correct uint8 has been saved.");
            }

            {
                datraw::uint16 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::uint16, L"Construction from uint16 yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct uint16 has been saved.");
            }

            {
                datraw::uint32 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::uint32, L"Construction from uint32 yields correct type.");
                Assert::AreEqual(v.get<decltype(value)>(), value, L"Correct uint32 has been saved.");
            }

            {
                datraw::uint64 value = 42;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::uint64, L"Construction from uint64 yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct uint64 has been saved.");
            }

            {
                datraw::scalar_type value = datraw::scalar_type::float16;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::scalar_type, L"Construction from scalar_type yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct scalar_type has been saved.");
            }

            {
                datraw::scalar_type value = datraw::scalar_type::raw;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::scalar_type, L"Construction from scalar_type yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct scalar_type has been saved.");
            }

            {
                datraw::grid_type value = datraw::grid_type::rectilinear;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::grid_type, L"Construction from grid_type yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct grid_type has been saved.");
            }

            {
                datraw::endianness value = datraw::endianness::big;
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::endianness, L"Construction from endianness yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct endianness has been saved.");
            }

            {
                std::string value = "HORST";
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::string, L"Construction from string yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct endianness has been saved.");
            }

            {
                std::wstring value = L"Hugo";
                variant v(value);
                Assert::IsTrue(v.type() == variant_type::wstring, L"Construction from wstring yields correct type.");
                Assert::IsTrue(v.get<decltype(value)>() == value, L"Correct endianness has been saved.");
            }
        }
    };
}
