/// <copyright file="variant.h" company="Visualisierungsinstitut der Universität Stuttgart">
/// Copyright © 2017 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
/// </copyright>
/// <author>Christoph Müller</author>

#pragma once

#include <cassert>
#if (!defined(__GNUC__) || (__GNUC__ >= 5))
#include <codecvt>
#endif /* (!defined(__GNUC__) || (__GNUC__ >= 5)) */
#include <cstdint>
#include <cstring>
#include <functional>
#include <iostream>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <wchar.h>

#include "datraw/endianness.h"
#include "datraw/grid_type.h"
#include "datraw/literal.h"
#include "datraw/scalar_type.h"
#include "datraw/types.h"


namespace datraw {

    /* Forward declarations */
    template<class C> class basic_variant;


    /// <summary>
    /// Possible data types <see cref="datraw::variant" /> can hold.
    /// </summary>
    enum class variant_type {
        /// <summary>
        /// The variant does not contain valid data.
        /// </summary>
        empty,

        /// <summary>
        /// The variant holds a Boolean value.
        /// </summary>
        boolean,

        /// <summary>
        /// The variant holds a signed 8-bit integer.
        /// </summary>
        int8,

        /// <summary>
        /// The variant holds a signed 16-bit integer.
        /// </summary>
        int16,

        /// <summary>
        /// The variant holds a signed 32-bit integer.
        /// </summary>
        int32,

        /// <summary>
        /// The variant holds a signed 64-bit integer.
        /// </summary>
        int64,

        /// <summary>
        /// The variant holds an unsigned 8-bit integer.
        /// </summary>
        uint8,

        /// <summary>
        /// The variant holds an unsigned 16-bit integer.
        /// </summary>
        uint16,

        /// <summary>
        /// The variant holds an unsigned 32-bit integer.
        /// </summary>
        uint32,

        /// <summary>
        /// The variant holds an unsigned 64-bit integer.
        /// </summary>
        uint64,

        /// <summary>
        /// The variant holds 32-bit floating point number.
        /// </summary>
        float32,

        /// <summary>
        /// The variant holds 64-bit floating point number.
        /// </summary>
        float64,

        /// <summary>
        /// The variant holds a string.
        /// </summary>
        string,

        /// <summary>
        /// The variant holds a wide string.
        /// </summary>
        wstring,

        /// <summary>
        /// The variant holds <c>void</c> pointer.
        /// </summary>
        pointer,

        /// <summary>
        // A vector of signed 8-bit integers.
        /// </summary>
        vec_int8,

        /// <summary>
        // A vector of signed 16-bit integers.
        /// </summary>
        vec_int16,

        /// <summary>
        // A vector of signed 32-bit integers.
        /// </summary>
        vec_int32,

        /// <summary>
        // A vector of signed 64-bit integers.
        /// </summary>
        vec_int64,

        /// <summary>
        // A vector of unsigned 8-bit integers.
        /// </summary>
        vec_uint8,

        /// <summary>
        // A vector of unsigned 16-bit integers.
        /// </summary>
        vec_uint16,

        /// <summary>
        // A vector of unsigned 32-bit integers.
        /// </summary>
        vec_uint32,

        /// <summary>
        // A vector of unsigned 64-bit integers.
        /// </summary>
        vec_uint64,

        /// <summary>
        // A vector of 32-bit floats.
        /// </summary>
        vec_float32,

        /// <summary>
        // A vector of unsigned 64-bit floats.
        /// </summary>
        vec_float64,

        scalar_type,

        grid_type,

        endianness

        // Add new members here.
    };


namespace detail {

    /// <summary>
    //// Actual storage structure for <see cref="datraw::basic_variant" />.
    /// </summary>
    /// <remarks>
    /// <para>This union should never be used except for in
    /// <see cref="datraw::basic_variant" />!</para>
    /// <para>The union defines an empty default constructor and an empty
    /// destructor, which results in its members being treated like
    /// uninitialised memory. The <see cref="datraw::basic_variant" /> must
    /// handle initialisation and finalisation on behalf of the union. You will
    /// most likely get that wrong if you use this type directly.</para>
    /// </remarks>
    union variant {
        bool val_boolean;
        std::int8_t val_int8;
        std::int16_t val_int16;
        std::int32_t val_int32;
        std::int64_t val_int64;
        std::uint8_t val_uint8;
        std::uint16_t val_uint16;
        std::uint32_t val_uint32;
        std::uint64_t val_uint64;
        float val_float32;
        double val_float64;
        std::string val_string;
        std::wstring val_wstring;
        void *val_pointer;
        std::vector<std::int8_t> val_vec_int8;
        std::vector<std::int16_t> val_vec_int16;
        std::vector<std::int32_t> val_vec_int32;
        std::vector<std::int64_t> val_vec_int64;
        std::vector<std::uint8_t> val_vec_uint8;
        std::vector<std::uint16_t> val_vec_uint16;
        std::vector<std::uint32_t> val_vec_uint32;
        std::vector<std::uint64_t> val_vec_uint64;
        std::vector<float> val_vec_float32;
        std::vector<double> val_vec_float64;
        datraw::scalar_type val_scalar_type;
        datraw::grid_type val_grid_type;
        datraw::endianness val_endianness;
        // Add new members here (must be val_[variant_type name]).

        inline variant(void) { }
        inline ~variant(void) { }
    };


    /// <summary>
    /// A list of variant types to expand.
    /// </summary>
    template<variant_type...> struct variant_type_list_t { };

    /// <summary>
    /// The actual type list we use in 
    /// <see cref="variant::conditional_invoke" />.
    /// </summary>
    /// <remarks>
    /// New types in <see cref="variant_type" /> must be added here.
    /// </remarks>
    typedef variant_type_list_t<variant_type::boolean, variant_type::int8,
        variant_type::int16, variant_type::int32, variant_type::int64,
        variant_type::uint8, variant_type::uint16, variant_type::uint32,
        variant_type::uint64, variant_type::float32, variant_type::float64,
        variant_type::string, variant_type::wstring, variant_type::pointer,
        variant_type::vec_int8, variant_type::vec_int16,
        variant_type::vec_int32, variant_type::vec_int64,
        variant_type::vec_uint8, variant_type::vec_uint16,
        variant_type::vec_uint32, variant_type::vec_uint64,
        variant_type::vec_float32, variant_type::vec_float64,
        variant_type::scalar_type, variant_type::grid_type,
        variant_type::endianness
        /* Add new members here. */>
        variant_type_list;

#if 0
    /// <summary>
    /// <see cref="variant_types" /> which of the values can be automatically
    /// parsed using <see cref="trrojan::parse" />.
    /// </summary>
    typedef variant_type_list_t<variant_type::int8, variant_type::int16,
        variant_type::int32, variant_type::int64, variant_type::uint8,
        variant_type::uint16, variant_type::uint32, variant_type::uint64,
        variant_type::float32, variant_type::float64>
        auto_parsable_variant_type_list;
#endif


    /// <summary>
    /// This structure allows for deriving the C++ type from a given
    /// <see cref="datraw::variant_type" />.
    /// </summary>
    /// <remarks>
    /// The default implementation does indicate an invalid type; the template
    /// specialisations implement the type deduction.
    /// </remarks>
    /// <tparam name="C">The character type used for strings.</tparam>
    /// <tparam name="T">The <see cref="datraw::variant_type" /> to derive the
    /// C++ type for.</tparam>
    template<class C, variant_type T> struct variant_fwd_traits { };

    /// <summary>
    /// This structure allows for deriving the
    /// <see cref="datraw::variant_type" /> from a C++ type.
    /// </summary>
    /// <remarks>
    /// <para>This structure implements the inverse operation of
    /// <see cref="variant_fwd_traits" />.</para>
    /// <para>The default implementation does indicate an invalid type; the
    /// template specialisations implement the type deduction.</para>
    /// </remarks>
    /// <tparam name="C">The character type used for strings.</tparam>
    /// <tparam name="T">The C++ type to derive the
    /// <see cref="datraw::variant_type" /> for.</tparam>
    template<class C, class T> struct variant_rev_traits { };

#define __DATRAW_DECL_VARIANT_TRAITS(t)                                        \
    template<class C> struct variant_fwd_traits<C, variant_type::t> {          \
        typedef std::basic_string<C> string_type;                              \
        typedef decltype(detail::variant::val_##t) value_type;                 \
        static const variant_type type = variant_type::t;                      \
        inline static value_type *get(detail::variant& v) {                    \
            return &v.val_##t;                                                 \
        }                                                                      \
        inline static const value_type *get(const detail::variant& v) {        \
            return &v.val_##t;                                                 \
        }                                                                      \
        static inline const string_type& name(void) {                          \
            static const string_type retval(DATRAW_TPL_LITERAL(C, #t));        \
            return retval;                                                     \
        }                                                                      \
    };                                                                         \
    template<class C> struct variant_rev_traits<C,                             \
            decltype(detail::variant::val_##t)> {                              \
        typedef std::basic_string<C> string_type;                              \
        typedef decltype(detail::variant::val_##t) value_type;                 \
        static const variant_type type = variant_type::t;                      \
        inline static value_type *get(detail::variant& v) {                    \
            return &v.val_##t;                                                 \
        }                                                                      \
        inline static const value_type *get(const detail::variant& v) {        \
            return &v.val_##t;                                                 \
        }                                                                      \
        static inline const string_type& name(void) {                          \
            static const string_type retval(DATRAW_TPL_LITERAL(C, #t));        \
            return retval;                                                     \
        }                                                                      \
    }

    __DATRAW_DECL_VARIANT_TRAITS(boolean);
    __DATRAW_DECL_VARIANT_TRAITS(int8);
    __DATRAW_DECL_VARIANT_TRAITS(int16);
    __DATRAW_DECL_VARIANT_TRAITS(int32);
    __DATRAW_DECL_VARIANT_TRAITS(int64);
    __DATRAW_DECL_VARIANT_TRAITS(uint8);
    __DATRAW_DECL_VARIANT_TRAITS(uint16);
    __DATRAW_DECL_VARIANT_TRAITS(uint32);
    __DATRAW_DECL_VARIANT_TRAITS(uint64);
    __DATRAW_DECL_VARIANT_TRAITS(float32);
    __DATRAW_DECL_VARIANT_TRAITS(float64);
    __DATRAW_DECL_VARIANT_TRAITS(string);
    __DATRAW_DECL_VARIANT_TRAITS(wstring);
    __DATRAW_DECL_VARIANT_TRAITS(pointer);
    __DATRAW_DECL_VARIANT_TRAITS(vec_int8);
    __DATRAW_DECL_VARIANT_TRAITS(vec_int16);
    __DATRAW_DECL_VARIANT_TRAITS(vec_int32);
    __DATRAW_DECL_VARIANT_TRAITS(vec_int64);
    __DATRAW_DECL_VARIANT_TRAITS(vec_uint8);
    __DATRAW_DECL_VARIANT_TRAITS(vec_uint16);
    __DATRAW_DECL_VARIANT_TRAITS(vec_uint32);
    __DATRAW_DECL_VARIANT_TRAITS(vec_uint64);
    __DATRAW_DECL_VARIANT_TRAITS(vec_float32);
    __DATRAW_DECL_VARIANT_TRAITS(vec_float64);
    __DATRAW_DECL_VARIANT_TRAITS(scalar_type);
    __DATRAW_DECL_VARIANT_TRAITS(grid_type);
    __DATRAW_DECL_VARIANT_TRAITS(endianness);
    // Add new specialisations here here.

#undef __DATRAW_DECL_VARIANT_TRAITS

    /// <summary>
    /// Functor which tries casting the value of the variant to a specific
    /// type and returning it.
    /// </summary>
    template<class C, variant_type T> struct cast_to {
        typedef typename variant_fwd_traits<C, T>::value_type type;
        typedef datraw::basic_variant<C> variant_type;

        template<class U>
        static inline typename std::enable_if<std::is_convertible<type, U>::value>::type
        invoke(type& v, U& target) {
            target = static_cast<U>(v);
        }

        template<class U>
        static inline typename std::enable_if<!std::is_convertible<type, U>::value>::type
        invoke(type& v, U& target) {
            throw std::bad_cast();
        }
    };

    /// <summary>
    /// Functor which copies the data of the calling variant to another one.
    /// </summary>
    template<class C, variant_type T> struct copy_to {
        typedef typename detail::variant_fwd_traits<C, T>::value_type type;
        typedef datraw::basic_variant<C> variant_type;
        static inline void invoke(const type& v, variant_type& target);
    };

    /// <summary>
    /// Functor which destructs the currently active variant data.
    /// </summary>
    template<class C, variant_type T> struct destruct {
        typedef typename detail::variant_fwd_traits<C, T>::value_type type;
        typedef datraw::basic_variant<C> variant_type;
        static inline void invoke(type& v) { v.~type(); }
    };

    /// <summary>
    /// Functor which compares the values of two variants with the same
    /// type.
    /// </summary>
    template<class C, variant_type T> struct is_same {
        typedef typename detail::variant_fwd_traits<C, T>::value_type type;
        typedef datraw::basic_variant<C> variant_type;
        static inline void invoke(type& v, const variant_type& rhs,
            bool& retval);
    };

    /// <summary>
    /// Functor which moves the data of the calling variant to another
    /// one.
    /// </summary>
    /// <remarks>
    /// Please not that the source variant will be unmodified, only the
    /// data are moved, ie changes to the state of the variant itself must
    /// be done manually after the call.
    /// </remarks>
    template<class C, variant_type T> struct move_to {
        typedef typename detail::variant_fwd_traits<C, T>::value_type type;
        typedef datraw::basic_variant<C> variant_type;
        static inline void invoke(type& v, variant_type& target);
    };

    /// <summary>
    /// Functor for printing the currently active variant data to an
    /// <see cref="std::ostream" />.
    /// </summary>
    template<class C, variant_type T> struct print {
        typedef typename detail::variant_fwd_traits<C, T>::value_type type;
        typedef datraw::basic_variant<C> variant_type;
        static inline void invoke(type& v, std::basic_ostream<C>& stream) {
            stream << v;
        }
    };

} /* end namespace detail */


    /// <summary>
    /// A type that can opaquely store different kind of data.
    /// </summary>
    /// <tparam name="C">The character type used for strings.</tparam>
    template<class C> class basic_variant {

    public:

        /// <summary>
        /// The character type used for strings.
        /// </summary>
        typedef C char_type;

        /// <summary>
        /// The string type used by the variant.
        /// </summary>
        typedef std::basic_string<char_type> string_type;

        /// <summary>
        /// Initialises a new instance of the class.
        /// </summary>
        inline basic_variant(void) : cur_type(variant_type::empty) { }

        /// <summary>
        /// Clone <paramref name="rhs" />.
        /// </summary>
        /// <param name="rhs">The object to be cloned.</param>
        inline basic_variant(const basic_variant& rhs)
                : cur_type(variant_type::empty) {
            *this = rhs;
        }

        /// <summary>
        /// Move <paramref name="rhs" />.
        /// </summary>
        /// <param name="rhs">The object to be moved.</param>
        inline basic_variant(basic_variant&& rhs)
                : cur_type(variant_type::empty) {
            *this = std::move(rhs);
        }

        /// <summary>
        /// Initialise from <paramref name="value" />.
        /// </summary>
        /// <remarks>
        /// This constructor allows for implicit conversions from
        /// <tparamref name="T" />.
        /// </remarks>
        /// <param name="value">The initial value of the variant.</param>
        template<class T>
        inline basic_variant(const T value) : cur_type(variant_type::empty) {
            this->set<T>(value);
        }

        /// <summary>
        /// Finalises the instance.
        /// </summary>
        inline ~basic_variant(void) {
            this->clear();
        }

        /// <summary>
        /// Converts the value of the variant to <tparamref name="T" /> if
        /// possible.
        /// </summary>
        /// <tparam name="T">The target type.</tparam>
        /// <returns>The value of the variant casted to <tparamref name="T" />.
        /// </returns>
        /// <exception cref="std::bad_cast">If the currently stored value is not
        /// convertible to <tparam name="T" />.</exception>
        template<class T> inline T as(void) const {
            auto retval = T();
            this->conditional_invoke<detail::cast_to>(retval);
            return retval;
        }

        /// <summary>
        /// Clears the variant, effectively resetting it to empty state.
        /// </summary>
        inline void clear(void) {
            this->conditional_invoke<detail::destruct>();
            this->cur_type = variant_type::empty;
        }

        /// <summary>
        /// Answer whether the variant is empty.
        /// </summary>
        /// <returns><c>true</c> if the variant is empty,
        /// <c>false</c> otherwise.
        inline bool empty(void) const {
            return (this->cur_type == variant_type::empty);
        }

        /// <summary>
        /// Gets the value of the variant interpreted as type
        /// <tparamref name="T" />.
        /// </summary>
        /// <remarks>
        /// The method does not check (except for an assertion) whether it is
        /// legal to retrieve the value as the specified type. It is up to the
        /// user to check this beforehand.
        /// </remarks>
        /// <tparam name="T">The member of <see cref="variant_type" />
        /// matching the value currently returned by
        /// <see cref="datraw::variant::type" />.</tparam>
        /// <returns>The current value of the variant.</returns>
        template<datraw::variant_type T>
        const typename detail::variant_fwd_traits<C, T>::type& get(void) const {
            typedef detail::variant_fwd_traits<C, T> traits;
            assert(this->cur_type == T);
            return *traits::get(this->data);
        }

        /// <summary>
        /// Gets the value of the variant interpreted as type
        /// <tparamref name="T" />.
        /// </summary>
        /// <remarks>
        /// The method does not check (except for an assertion) whether it is
        /// legal to retrieve the value as the specified type. It is up to the
        /// user to check this beforehand.
        /// </remarks>
        /// <tparam name="T">The C++ type currently stored in the variant.
        /// </tparam>
        /// <returns>The current value of the variant.</returns>
        template<class T> const T& get(void) const {
            typedef detail::variant_rev_traits<C, T> traits;
            assert(this->cur_type == traits::type);
            return *traits::get(this->data);
        }

        /// <summary>
        /// Answer whether the variant holds the given type.
        /// </summary>
        /// <param name="type">The type to be tested</param>
        /// <returns><c>true</c> if the variants holds a value of
        /// <paramref name="type" />, <c>false</c> otherwise.</returns>
        inline bool is(const variant_type type) const {
            return (this->cur_type == type);
        }

        /// <summary>
        /// Sets a new value.
        /// </summary>
        /// <param name="value">The new value of the variant.</param>
        /// <tparam name="T">The new type of the variant.</tparam>
        template<variant_type T>
        void set(const typename detail::variant_fwd_traits<C, T>::value_type&
                value) {
            typedef detail::variant_fwd_traits<char_type, T> traits;
            this->reconstruct<T>();
            *traits::get(this->data) = value;
        }

        /// <summary>
        /// Sets a new value.
        /// </summary>
        /// <param name="value">The new value of the variant.</param>
        /// <tparam name="T">The type of the new value.</tparam>
        template<class T> void set(const T& value) {
            typedef typename std::decay<T>::type type;
            typedef detail::variant_rev_traits<char_type, type> traits;
            this->reconstruct<traits::type>();
            *traits::get(this->data) = value;
        }

        /// <summary>
        /// Sets a new value by moving the data.
        /// </summary>
        /// <param name="value">The new value of the variant.</param>
        /// <tparam name="T">The new type of the variant.</tparam>
        template<variant_type T>
        void set(typename detail::variant_fwd_traits<C, T>::type&& value) {
            typedef detail::variant_fwd_traits<char_type, T> traits;
            this->reconstruct<T>();
            *traits::get(this->data) = std::move(value);
        }

        /// <summary>
        /// Sets a new value by moving the data.
        /// </summary>
        /// <param name="value">The new value of the variant.</param>
        /// <tparam name="T">The type of the new value.</tparam>
        template<class T> void set(T&& value) {
            typedef typename std::decay<T>::type type;
            typedef detail::variant_rev_traits<char_type, type> traits;
            this->reconstruct<traits::type>();
            *traits::get(this->data) = std::move(value);
        }

        /// <summary>
        /// Answer the current type of the variant.
        /// </summary>
        /// <returns>The current type of the variant</returns>
        inline variant_type type(void) const {
            return this->cur_type;
        }

        /// <summary>
        /// Assignment.
        /// </summary>
        /// <remarks>
        /// This operator is an alias for <see cref="variant::set" />.
        /// </remarks>
        /// <param name="rhs">The right hand side operand.</param>
        /// <returns><c>*this</c></returns>
        template<variant_type T> inline basic_variant& operator =(
                const typename detail::variant_fwd_traits<C, T>::value_type&
                rhs) const {
            this->set(rhs);
            return *this;
        }

        /// <summary>
        /// Assignment.
        /// </summary>
        /// <param name="rhs">The right hand side operand.</param>
        /// <returns><c>*this</c></returns>
        basic_variant& operator =(const basic_variant& rhs);

        /// <summary>
        /// Move assignment.
        /// </summary>
        /// <param name="rhs">The right hand side operand.</param>
        /// <returns><c>*this</c></returns>
        basic_variant& operator =(basic_variant&& rhs);

        /// <summary>
        /// Test for equality.
        /// </summary>
        /// <param name="rhs">The right hand side operand.</param>
        /// <returns><c>true</c> if this variant and <paramref name="rhs" />
        /// are equal.</returns>
        bool operator ==(const basic_variant& rhs) const;

        /// <summary>
        /// Test for inequality.
        /// </summary>
        /// <param name="rhs">The right hand side operand.</param>
        /// <returns><c>true</c> if this variant and <paramref name="rhs" />
        /// are not equal.</returns>
        inline bool operator !=(const basic_variant& rhs) const {
            return !(*this == rhs);
        }

        /// <summary>
        /// Cast to <tparamref name="T" />.
        /// </summary>
        /// <remarks>
        /// This operator is an alias for <see cref="variant::as" />.
        /// </remarks>
        /// <tparam name="T">The target type.</tparam>
        /// <returns>The value of the variant casted to <tparamref name="T" />.
        /// </returns>
        /// <exception cref="std::bad_cast">If the currently stored value is not
        /// convertible to <tparam name="T" />.</exception>
        template<class T> operator T(void) const {
            return this->as<T>();
        }

        /// <summary>
        /// Write the <see cref="datraw::variant" /> to a stream.
        /// </summary>
        /// <param name="lhs">The left-hand side operand (the stream to
        /// write to).</param>
        /// <param name="rhs">The right-hand side operand (the object to
        /// be written).</param>
        /// <returns><paramref name="lhs" />.</returns>
        friend inline std::basic_ostream<C>& operator <<(
                std::basic_ostream<C>& lhs, const basic_variant<C>& rhs) {
            rhs.conditional_invoke<detail::print>(lhs);
            return lhs;
        }

    private:

        /// <summary>
        /// Invokes the functor <tparamref name="F" /> on the currently active
        /// variant data.
        /// </summary>
        /// <param name="params">An optional list of runtime-parameters
        /// which are forwarded to the functor.</param>
        /// <tparam name="F">The functor to be called for the matching
        /// <see cref="variant_type" />.</tparam>
        /// <tparam name="P">The parameter list for the functor.</tparam>
        template<template<class, variant_type> class F, class... P>
        inline void conditional_invoke(P&&... params) {
            this->conditional_invoke0<F>(detail::variant_type_list(),
                std::forward<P>(params)...);
        }

        /// <summary>
        /// Invokes the functor <tparamref name="F" /> on the currently active
        /// variant data.
        /// </summary>
        /// <param name="params">An optional list of runtime-parameters
        /// which are forwarded to the functor.</param>
        /// <tparam name="F">The functor to be called for the matching
        /// <see cref="variant_type" />.</tparam>
        /// <tparam name="P">The parameter list for the functor.</tparam>
        template<template<class, variant_type> class F, class... P>
        inline void conditional_invoke(P&&... params) const {
            // Rationale: We do not want to implement the whole stack twice, so
            // we use the const_cast here as we know that our own functors do
            // not do nasty things.
            const_cast<basic_variant *>(this)->conditional_invoke0<F>(
                detail::variant_type_list(), std::forward<P>(params)...);
        }

        /// <summary>
        /// Invokes the functor <tparamref name="F" /> on the data of the 
        /// variant if the the variant's type is <tparamref name="T" />.
        /// </summary>
        /// <param name="params">An optional list of runtime-parameters
        /// which are forwarded to the functor.</param>
        /// <tparam name="F">The functor to be called for the matching
        /// <see cref="variant_type" />.</tparam>
        /// <tparam name="T">The type to be currently evaluated. It this
        /// is the active type in the variant, the functor is invoked on
        /// the data.</tparam>
        /// <tparam name="U">The rest of types to be evaluated.</tparam>
        /// <tparam name="P">The parameter list for the functor.</tparam>
        template<template<class, variant_type> class F, variant_type T,
            variant_type... U, class... P>
        void conditional_invoke0(detail::variant_type_list_t<T, U...>,
            P&&... params);

        /// <summary>
        /// End of recursion for
        /// <see cref="variant::conditional_invoke0" />.
        /// </summary>
        /// <param name="params">An optional list of runtime-parameters
        /// which are forwarded to the functor.</param>
        /// <tparam name="F">The functor to be called for the matching
        /// <see cref="variant_type" />.</tparam>
        /// <tparam name="P">The parameter list for the functor.</tparam>
        template<template<class, variant_type> class F, class... P>
        inline void conditional_invoke0(detail::variant_type_list_t<>,
            P&&... params) { }

        /// <summary>
        /// Clears the variant and (re-) constructs it as type
        /// <tparamref name="T" />.
        /// </summary>
        /// <remarks>
        /// The effects of this method are that (i) any previous value will be
        /// destructed, (ii) the new type designated by <tparamref name="T" />
        /// will be constructed and (iii) the current type will be set to
        /// <tparamref name="T" />.
        /// <remarks>
        /// <tparam name="T">The new type of the variant, which should be
        /// initialised by the method.</tparam>
        template<variant_type T> inline void reconstruct(void) {
            typedef detail::variant_fwd_traits<C, T> traits;
            typedef typename traits::value_type type;
            this->conditional_invoke<detail::destruct>();
            ::new (traits::get(this->data)) type();
            this->cur_type = T;
        }

        /// <summary>
        /// Stores which of the members of <see cref="data" /> is currently
        /// valid.
        /// </summary>
        variant_type cur_type;

        /// <summary>
        /// Stores the actual value of the variant.
        /// </summary>
        detail::variant data;
    };


    /// <summary>
    /// Variant using 8-bit characters.
    /// </summary>
    typedef basic_variant<char> variant;


    /// <summary>
    /// Variant using wide characters.
    /// </summary>
    typedef basic_variant<wchar_t> wvariant;
}

#include "datraw/variant.inl"
