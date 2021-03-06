#ifndef _STDEX_TYPE_TRAITS_H
#define _STDEX_TYPE_TRAITS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Implemented all basic standart C++11 features. 
// What can not be implemented or implemented with some limits:
// is_class - ni
// is_enum - ni
// is_rvalue_reference - ni
// is_union - ni
// is_scalar - can't detect unions
// is_abstract - ni
// is_empty - ni
// is_literal_type - ni
// is_pod - ni
// is_polymorphic - ni
// is_standard_layout - ni
// is_trivial - ni
// is_trivially_copyable - ni
// All type features (like is_assignable) - ni

// stdex includes
/*none*/

// POSIX includes
/*none*/

// std includes
#include <cstddef>
#include <climits>

namespace stdex
{
	template<class _Tp, _Tp Val>
	struct integral_constant
	{	// convenient template for integral constant types
		static const _Tp value = Val;

		typedef const _Tp value_type;
		typedef integral_constant<_Tp, Val> type;

		operator value_type() const
		{	// return stored value
			return (value);
		}

		value_type operator()() const
		{	// return stored value
			return (value);
		}
	};

	typedef integral_constant<bool, true> true_type;
	typedef integral_constant<bool, false> false_type;

	template<bool Val>
	struct bool_constant :
		public integral_constant<bool, Val>
	{};

	// Primary template.
	// Define a member typedef @c type to one of two argument types.
	template<bool _Cond, class _Iftrue, class _Iffalse>
	struct conditional
	{
		typedef _Iftrue type;
	};

	// Partial specialization for false.
	template<class _Iftrue, class _Iffalse>
	struct conditional<false, _Iftrue, _Iffalse>
	{
		typedef _Iffalse type;
	};

	namespace detail
	{
		struct void_type {};

		template<class _B1 = void_type, class _B2 = void_type, class _B3 = void_type, class _B4 = void_type>
		struct _or_ :
			public conditional<_B1::value, _B1, _or_<_B2, _or_<_B3, _B4> > >::type
		{ };


		template<>
		struct _or_<void_type, void_type, void_type, void_type>;

		template<class _B1>
		struct _or_<_B1, void_type, void_type, void_type> :
			public _B1
		{ };

		template<class _B1, class _B2>
		struct _or_<_B1, _B2, void_type, void_type> :
			public conditional<_B1::value, _B1, _B2>::type
		{ };

		template<class _B1, class _B2, class _B3>
		struct _or_<_B1, _B2, _B3, void_type> :
			public conditional<_B1::value, _B1, _or_<_B2, _B3> >::type
		{ };

		template<class _B1 = void_type, class _B2 = void_type, class _B3 = void_type, class _B4 = void_type>
		struct _and_;


		template<>
		struct _and_<void_type, void_type, void_type, void_type>;

		template<class _B1>
		struct _and_<_B1, void_type, void_type, void_type> :
			public _B1
		{ };

		template<class _B1, class _B2>
		struct _and_<_B1, _B2, void_type, void_type> :
			public conditional<_B1::value, _B2, _B1>::type
		{ };

		template<class _B1, class _B2, class _B3>
		struct _and_<_B1, _B2, _B3, void_type> :
			public conditional<_B1::value, _and_<_B2, _B3>, _B1>::type
		{ };

		template<class _Pp>
		struct _not_
		{
			static const bool value = !bool(_Pp::value);

			typedef const bool value_type;
			typedef integral_constant<bool, _not_::value == bool(1)> type;

			operator value_type() const
			{	// return stored value
				return (value);
			}

			value_type operator()() const
			{	// return stored value
				return (value);
			}
		};
	}

	template<bool Val>
	struct _cat_base :
		integral_constant<bool, Val>
	{	// base class for type predicates
	};

	template<class _Tp>
	struct remove_const
	{	// remove top level const qualifier
		typedef _Tp type;
	};

	template<class _Tp>
	struct remove_const<const _Tp>
	{	// remove top level const qualifier
		typedef _Tp type;
	};

	template<class _Tp>
	struct remove_const<const volatile _Tp>
	{	// remove top level const qualifier
		typedef volatile _Tp type;
	};

	// remove_volatile
	template<class _Tp>
	struct remove_volatile
	{	// remove top level volatile qualifier
		typedef _Tp type;
	};

	template<class _Tp>
	struct remove_volatile<volatile _Tp>
	{	// remove top level volatile qualifier
		typedef _Tp type;
	};

	// remove_cv
	template<class _Tp>
	struct remove_cv
	{	// remove top level const and volatile qualifiers
		typedef typename remove_const<typename remove_volatile<_Tp>::type>::type
			type;
	};

	// add_const
	template<typename _Tp>
	struct add_const
	{
		typedef _Tp const     type;
	};

	// add_volatile
	template<typename _Tp>
	struct add_volatile
	{
		typedef _Tp volatile     type;
	};

	// add_cv
	template<typename _Tp>
	struct add_cv
	{
		typedef typename
			add_const<typename add_volatile<_Tp>::type>::type     type;
	};

	namespace detail
	{
		template <class> struct _is_floating_point : public false_type {};

		template<> struct _is_floating_point<float> : public true_type {};
		template<> struct _is_floating_point<double> : public true_type {};
		template<> struct _is_floating_point<long double> : public true_type {};

		template <class> struct _is_integral : public false_type {};

		template<> struct _is_integral<bool> : public true_type {};
		template<> struct _is_integral<char> : public true_type {};
		template<> struct _is_integral<wchar_t> : public true_type {};

#ifdef STDEX_FORCE_CPP11_TYPES_SUPPORT
#include "types_ex.h"
		template<> struct _is_integral<char16_t> : public true_type {};
		template<> struct _is_integral<char32_t> : public true_type {};
		template<> struct _is_integral<int64_t> : public true_type {};
		template<> struct _is_integral<uint64_t> : public true_type {};
#endif

		template<> struct _is_integral<unsigned char> : public true_type {};
		template<> struct _is_integral<unsigned short int> : public true_type {};
		template<> struct _is_integral<unsigned int> : public true_type {};
		template<> struct _is_integral<unsigned long int> : public true_type {};

#ifdef LLONG_MAX
		template<> struct _is_integral<unsigned long long int> : public true_type {};
#endif

		template<> struct _is_integral<signed char> : public true_type {};
		template<> struct _is_integral<short int> : public true_type {};
		template<> struct _is_integral<int> : public true_type {};
		template<> struct _is_integral<long int> : public true_type {};

#ifdef LLONG_MAX
		template<> struct _is_integral<long long int> : public true_type {};
#endif
	}


	template <class _Tp> 
	struct is_floating_point : 
		public detail::_is_floating_point<typename remove_cv<_Tp>::type> 
	{ };

	template <class _Tp>
	struct is_integral :
		public detail::_is_integral<typename remove_cv<_Tp>::type>
	{ };

	namespace detail
	{
		template<bool>
		struct _sign_unsign_chooser;

		template<class _Tp>
		struct _signed_comparer
		{
			static const bool value = _Tp(-1) < _Tp(0);
		};

		template<class _Tp>
		struct UnsignedComparer
		{
			static const bool value = _Tp(0) < _Tp(-1);
		};

		template<>
		struct _sign_unsign_chooser<true>//integral
		{
			template<class _Tp>
			struct _signed :
				public _cat_base<_signed_comparer<typename remove_cv<_Tp>::type>::value>
			{
			};

			template<class _Tp>
			struct _unsigned :
				public _cat_base<UnsignedComparer<typename remove_cv<_Tp>::type>::value>
			{
			};
		};

		template<>
		struct _sign_unsign_chooser<false>//floating point
		{
			template<class _Tp>
			struct _signed :
				public is_floating_point<_Tp>
			{
			};

			template<class _Tp>
			struct _unsigned :
				public false_type
			{
			};
		};
	}

	template <bool, class _Tp = detail::void_type>
	struct enable_if
	{ };

	template <class _Tp>
	struct enable_if<true, _Tp>
	{
		typedef _Tp type;
	};

	template<class, class>
	struct is_same :
		public false_type
	{ };

	template<class _Tp>
	struct is_same<_Tp, _Tp> :
		public true_type//specialization
	{ };

	template <class _Tp>
	struct is_const :
		public false_type
	{ };

	template <class _Tp>
	struct is_const<const _Tp> :
		public true_type
	{ };

	template <class _Tp>
	struct is_const<const volatile _Tp> :
		public true_type
	{ };

	/// is_volatile
	template<class>
	struct is_volatile
		: public false_type
	{ };

	template<class _Tp>
	struct is_volatile<volatile _Tp>
		: public true_type
	{ };

	template<class _Tp>
	struct is_reference;

	template<class _Tp>
	struct is_object;

	namespace detail
	{
		template<class _Tp>
		struct _is_referenceable :
			public _or_<is_object<_Tp>, is_reference<_Tp> >::type
		{ };

		/*template<typename _Res, typename... _Args>
		struct _is_referenceable<_Res(_Args...)>
		: public true_type
		{ };*/

		/*template<typename _Res, typename... _Args>
		struct _is_referenceable<_Res(_Args......)>
		: public true_type
		{ };*/
	}

	template< class _Tp >
	struct remove_reference
	{
		typedef _Tp type;
	};

	template< class _Tp >
	struct remove_reference<_Tp&>
	{
		typedef _Tp type;
	};

	namespace detail
	{
		template<class _Tp, bool = _is_referenceable<_Tp>::value>
		struct _add_lvalue_reference_helper
		{
			typedef _Tp   type;
		};

		template<class _Tp>
		struct _add_lvalue_reference_helper<_Tp, true>
		{
			typedef _Tp&   type;
		};
	}

	// add_lvalue_reference
	template<class _Tp>
	struct add_lvalue_reference :
		public detail::_add_lvalue_reference_helper<_Tp>
	{ };

	template<class T>
	struct is_signed
	{	// determine whether T is a signed type

		static const bool value = detail::_sign_unsign_chooser<is_integral<T>::value>::template _signed<T>::value;

		typedef const bool value_type;
		typedef integral_constant<bool, is_signed::value == bool(1)> type;

		operator value_type() const
		{	// return stored value
			return (value);
		}

		value_type operator()() const
		{	// return stored value
			return (value);
		}
	};

	template<class T>
	struct is_unsigned
	{	// determine whether T is an unsigned type

		static const bool value = detail::_sign_unsign_chooser<is_integral<T>::value>::template _unsigned<T>::value;

		typedef const bool value_type;
		typedef integral_constant<bool, is_unsigned::value == bool(1)> type;

		operator value_type() const
		{	// return stored value
			return (value);
		}

		value_type operator()() const
		{	// return stored value
			return (value);
		}
	};

	namespace detail
	{
		template<class>
		struct _is_void_helper
			: public false_type { };

		template<>
		struct _is_void_helper<void>
			: public true_type { };

	}

	// is_void
	template<class _Tp>
	struct is_void :
		public detail::_is_void_helper<typename remove_cv<_Tp>::type>::type
	{ };

	// is_array
	template<class>
	struct is_array :
		public false_type { };

	template<class _Tp, std::size_t _Size>
	struct is_array<_Tp[_Size]> :
		public true_type { };

	/*template<class _Tp>
	struct is_array<_Tp[]>:
		public true_type { }; */

	namespace detail
	{
		template<class>
		struct _is_pointer_helper :
			public false_type { };

		template<class _Tp>
		struct _is_pointer_helper<_Tp*> :
			public true_type { };
	}

	// is_pointer
	template<class _Tp>
	struct is_pointer :
		public detail::_is_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };

	// is_lvalue_reference
	template<class>
	struct is_lvalue_reference :
		public false_type { };

	template<class _Tp>
	struct is_lvalue_reference<_Tp&> :
		public true_type { };

	namespace detail
	{
		template <class R>
		struct _is_function_ptr_helper : false_type {};
		template <class R >
		struct _is_function_ptr_helper<R(*)()> : true_type {};
		template <class R >
		struct _is_function_ptr_helper<R(*)(...)> : true_type {};
		template <class R, class T0>
		struct _is_function_ptr_helper<R(*)(T0)> : true_type {};
		template <class R, class T0>
		struct _is_function_ptr_helper<R(*)(T0 ...)> : true_type {};
		template <class R, class T0, class T1>
		struct _is_function_ptr_helper<R(*)(T0, T1)> : true_type {};
		template <class R, class T0, class T1>
		struct _is_function_ptr_helper<R(*)(T0, T1 ...)> : true_type {};
		template <class R, class T0, class T1, class T2>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2)> : true_type {};
		template <class R, class T0, class T1, class T2>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23 ...)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24)> : true_type {};
		template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24>
		struct _is_function_ptr_helper<R(*)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24 ...)> : true_type {};
	}

	// is_function
	template<class _Tp>
	struct is_function
	{
		static const bool value = detail::_is_function_ptr_helper<_Tp>::value;

		typedef const bool value_type;
		typedef integral_constant<bool, is_function::value == bool(1)> type;

		operator value_type() const
		{	// return stored value
			return (value);
		}

		value_type operator()() const
		{	// return stored value
			return (value);
		}
	};

	namespace detail
	{
		template<class>
		struct _is_null_pointer_helper
			: public false_type { };

#ifdef _STDEX_IMPLEMENTS_NULLPTR_SUPPORT
		template<>
		struct _is_null_pointer_helper<stdex::nullptr_t>
			: public true_type { };
#elif defined(_STDEX_NATIVE_NULLPTR_SUPPORT)
		template<>
		struct _is_null_pointer_helper<std::nullptr_t>
			: public true_type { };
#endif

	}
	// is_null_pointer (LWG 2247).
	template<class _Tp>
	struct is_null_pointer :
		public detail::_is_null_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };

	namespace detail
	{
		// __is_nullptr_t (extension).
		template<class _Tp>
		struct _is_nullptr_t :
			public is_null_pointer<_Tp>
		{ };
	}

	namespace detail
	{
		template<class>
		struct _is_member_object_pointer_helper :
			public false_type { };

		template<class _Tp, class _Cp>
		struct _is_member_object_pointer_helper<_Tp _Cp::*>:
			public _not_<is_function<_Tp> >::type
		{ };

		template<class>
		struct _is_member_function_pointer_helper :
			public false_type { };

		template<class _Tp, class _Cp>
		struct _is_member_function_pointer_helper<_Tp _Cp::*>
		{
			static const bool value = is_function<_Tp>::value;

			typedef const bool value_type;
			typedef integral_constant<bool, _is_member_function_pointer_helper::value == bool(1)> type;

			operator value_type() const
			{	// return stored value
				return (value);
			}

			value_type operator()() const
			{	// return stored value
				return (value);
			}
		};
	}

	// is_member_object_pointer
	template<class _Tp>
	struct is_member_object_pointer :
		public detail::_is_member_object_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };

	// is_member_function_pointer
	template<class _Tp>
	struct is_member_function_pointer :
		public detail::_is_member_function_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };

	// is_reference
	template<class _Tp>
	struct is_reference :
		public detail::_or_<is_lvalue_reference<_Tp>/*, is_rvalue_reference<_Tp> */>::type
	{};

	// is_arithmetic
	template<class _Tp>
	struct is_arithmetic :
		public detail::_or_<is_integral<_Tp>, is_floating_point<_Tp> >::type
	{ };

	// is_fundamental
	template<typename _Tp>
	struct is_fundamental :
		public detail::_or_<is_arithmetic<_Tp>, is_void<_Tp>, is_null_pointer<_Tp> >::type
	{};

	// is_object
	template<class _Tp>
	struct is_object :
		public detail::_not_< detail::_or_< is_function<_Tp>, is_reference<_Tp>, is_void<_Tp> > >::type
	{};

	template<class>
	struct is_member_pointer;

	// is_scalar
	template<class _Tp>
	struct is_scalar :
		public detail::_or_<is_arithmetic<_Tp>, is_pointer<_Tp>, is_member_pointer<_Tp>, is_null_pointer<_Tp>/*, is_enum<_Tp>*/ >::type
	{};

	// is_compound
	template<class _Tp>
	struct is_compound:
		public detail::_not_<is_fundamental<_Tp> >::type
	{ };

	namespace detail
	{
		template<class _Tp>
		struct _is_member_pointer_helper :
			public false_type { };

		template<class _Tp, class _Cp>
		struct _is_member_pointer_helper<_Tp _Cp::*> :
			public true_type { };
	}

	// is_member_pointer
	template<class _Tp>
	struct is_member_pointer :
		public detail::_is_member_pointer_helper<typename remove_cv<_Tp>::type>::type
	{ };


	template<class, unsigned = 0>
	struct extent;

	template<class>
	struct remove_all_extents;

	/*namespace detail
	{
		template<class _Tp>
		struct _is_array_known_bounds:
			public integral_constant<bool, (extent<_Tp>::value > 0)>
		{ };

		template<class _Tp>
		struct _is_array_unknown_bounds:
			public _and_<is_array<_Tp>, _not_<extent<_Tp> > >
		{ };
	}*/


	// rank
	template<class>
	struct rank :
		public integral_constant<std::size_t, 0> { };

	template<class _Tp, std::size_t _Size>
	struct rank<_Tp[_Size]>
	{
		static const std::size_t value = 1 + rank<_Tp>::value;

		typedef const bool value_type;
		typedef integral_constant<std::size_t, std::size_t(rank::value)> type;

		operator value_type() const
		{	// return stored value
			return (value);
		}

		value_type operator()() const
		{	// return stored value
			return (value);
		}
	};

	/*template<class _Tp>
	struct rank<_Tp []>:
		public integral_constant<std::size_t, 1 + rank<_Tp>::value> { };*/

		// extent
	template<class, unsigned _Uint>
	struct extent :
		public integral_constant<std::size_t, 0> { };

	template<class _Tp, unsigned _Uint, std::size_t _Size>
	struct extent<_Tp[_Size], _Uint> :
		public integral_constant<std::size_t, _Uint == 0 ? _Size : extent<_Tp, _Uint - 1>::value>
	{ };

	/*template<class _Tp, unsigned _Uint>
	struct extent<_Tp [], _Uint>:
		public integral_constant<std::size_t, _Uint == 0 ? 0 : extent<_Tp, _Uint - 1>::value>
	{ };*/


	namespace detail
	{
		// Utility for constructing identically cv-qualified types.
		template<class _Unqualified, bool _IsConst, bool _IsVol>
		struct _cv_selector;

		template<class _Unqualified>
		struct _cv_selector<_Unqualified, false, false>
		{
			typedef _Unqualified _type;
		};

		template<class _Unqualified>
		struct _cv_selector<_Unqualified, false, true>
		{
			typedef volatile _Unqualified _type;
		};

		template<class _Unqualified>
		struct _cv_selector<_Unqualified, true, false>
		{
			typedef const _Unqualified _type;
		};

		template<class _Unqualified>
		struct _cv_selector<_Unqualified, true, true>
		{
			typedef const volatile _Unqualified _type;
		};

		template<class _Qualified, class _Unqualified, bool _IsConst = is_const<_Qualified>::value, bool _IsVol = is_volatile<_Qualified>::value>
		class _match_cv_qualifiers
		{
			typedef _cv_selector<_Unqualified, _IsConst, _IsVol> _match;

		public:
			typedef typename _match::_type _type;
		};

		// Utility for finding the unsigned versions of signed integral types.
		template<class _Tp>
		struct _make_unsigned
		{
			typedef _Tp _type;
		};

		template<>
		struct _make_unsigned<char>
		{
			typedef unsigned char _type;
		};

		template<>
		struct _make_unsigned<signed char>
		{
			typedef unsigned char _type;
		};

		template<>
		struct _make_unsigned<short>
		{
			typedef unsigned short _type;
		};

		template<>
		struct _make_unsigned<int>
		{
			typedef unsigned int _type;
		};

		template<>
		struct _make_unsigned<long>
		{
			typedef unsigned long _type;
		};

#ifdef LLONG_MAX
		template<>
		struct _make_unsigned<long long>
		{
			typedef unsigned long long _type;
		};
#endif


		template<class _Tp>
		class _make_unsigned_selector
		{
			typedef unsigned char _smallest;
			static const bool _b0 = sizeof(_Tp) <= sizeof(_smallest);
			static const bool _b1 = sizeof(_Tp) <= sizeof(unsigned short);
			static const bool _b2 = sizeof(_Tp) <= sizeof(unsigned int);
			static const bool _b3 = sizeof(_Tp) <= sizeof(unsigned long);
#ifdef LLONG_MAX
			typedef conditional<_make_unsigned_selector<_Tp>::_b3, unsigned long, unsigned long long> _cond3;
#else
			typedef conditional<_make_unsigned_selector<_Tp>::_b3, unsigned long, unsigned long> _cond3;
#endif
			typedef typename _cond3::type _cond3_type;
			typedef conditional<_make_unsigned_selector<_Tp>::_b2, unsigned int, _cond3_type> _cond2;
			typedef typename _cond2::type _cond2_type;
			typedef conditional<_make_unsigned_selector<_Tp>::_b1, unsigned short, _cond2_type> _cond1;
			typedef typename _cond1::type _cond1_type;

			typedef typename conditional<_make_unsigned_selector<_Tp>::_b0, _smallest, _cond1_type>::type
				_unsigned_type;
			typedef _match_cv_qualifiers<_Tp, _unsigned_type> _cv_unsigned;

		public:
			typedef typename _cv_unsigned::_type _type;
		};
	}

	// make_unsigned
	template<class _Tp>
	struct make_unsigned
	{
		typedef typename detail::_make_unsigned_selector<_Tp>::_type type;
	};

	// Integral, but don't define.
	template<>
	struct make_unsigned<bool>;

	namespace detail
	{
		// Utility for finding the signed versions of unsigned integral types.
		template<class _Tp>
		struct _make_signed
		{
			typedef _Tp _type;
		};

		template<>
		struct _make_signed<char>
		{
			typedef signed char _type;
		};

		template<>
		struct _make_signed<unsigned char>
		{
			typedef signed char _type;
		};

		template<>
		struct _make_signed<unsigned short>
		{
			typedef signed short _type;
		};

		template<>
		struct _make_signed<unsigned int>
		{
			typedef signed int _type;
		};

		template<>
		struct _make_signed<unsigned long>
		{
			typedef signed long _type;
		};

#ifdef LLONG_MAX
		template<>
		struct _make_signed<unsigned long long>
		{
			typedef signed long long _type;
		};
#endif

		template<class _Tp>
		class _make_signed_selector
		{
			typedef typename _make_unsigned_selector<_Tp>::_type _unsigned_type;

			typedef _make_signed<typename remove_cv<_unsigned_type>::type> _signedt;
			typedef typename _signedt::_type _signed_type;
			typedef _match_cv_qualifiers<_unsigned_type, _signed_type> _cv_signed;

		public:
			typedef typename _cv_signed::_type _type;
		};
	}

	// make_signed
	template<class _Tp>
	struct make_signed
	{
		typedef typename detail::_make_signed_selector<_Tp>::_type type;
	};

	// Integral, but don't define.
	template<>
	struct make_signed<bool>;


	// remove_extent
	template<class _Tp>
	struct remove_extent
	{
		typedef _Tp     type;
	};

	template<class _Tp, std::size_t _Size>
	struct remove_extent<_Tp[_Size]>
	{
		typedef _Tp     type;
	};

	/*template<class _Tp>
	struct remove_extent<_Tp []>
	{
		typedef _Tp type;
	};*/

	// remove_all_extents
	template<class _Tp>
	struct remove_all_extents
	{
		typedef _Tp     type;
	};

	template<class _Tp, std::size_t _Size>
	struct remove_all_extents<_Tp[_Size]>
	{
		typedef typename remove_all_extents<_Tp>::type     type;
	};

	/*template<class _Tp>
	struct remove_all_extents<_Tp []>
	{
		typedef typename remove_all_extents<_Tp>::type type;
	};*/

	namespace detail
	{
		template<class _Tp, class>
		struct _remove_pointer_helper
		{
			typedef _Tp     type;
		};

		template<class _Tp, class _Up>
		struct _remove_pointer_helper<_Tp, _Up*>
		{
			typedef _Up     type;
		};
	}

	// remove_pointer
	template<class _Tp>
	struct remove_pointer
		: public detail::_remove_pointer_helper<_Tp, typename remove_cv<_Tp>::type>
	{ };


	namespace detail
	{
		template<class _Tp, bool>
		struct _add_pointer_helper
		{
			typedef _Tp     type;
		};

		template<class _Tp>
		struct _add_pointer_helper<_Tp, true>
		{
			typedef typename remove_reference<_Tp>::type*     type;
		};
	}

	// add_pointer
	template<class _Tp>
	struct add_pointer
		: public detail::_add_pointer_helper<_Tp, detail::_or_<detail::_is_referenceable<_Tp>, is_void<_Tp> >::value>
	{ };

	namespace detail
	{
		// Decay trait for arrays and functions, used for perfect forwarding
		// in make_pair, make_tuple, etc.
		template<class _Up,
			bool _IsArray,
			bool _IsFunction>
		struct _decay_selector;

		template<class _Up>
		struct _decay_selector<_Up, false, false>
		{
			typedef typename remove_cv<_Up>::type _type;
		};

		template<class _Up>
		struct _decay_selector<_Up, true, false>
		{
			typedef typename remove_extent<_Up>::type* _type;
		};

		template<class _Up>
		struct _decay_selector<_Up, false, true>
		{
			typedef typename add_pointer<_Up>::type _type;
		};
	}

	// decay
	template<class _Tp>
	class decay
	{
		typedef typename remove_reference<_Tp>::type _remove_type;

	public:
		typedef typename detail::_decay_selector<_remove_type, is_array<_remove_type>::value, is_function<_remove_type>::value>::_type type;
	};

	template<class _Tp, class T0 = detail::void_type, class T1 = detail::void_type, class T2 = detail::void_type, class T3 = detail::void_type, class T4 = detail::void_type, class T5 = detail::void_type, class T6 = detail::void_type, class T7 = detail::void_type, class T8 = detail::void_type, class T9 = detail::void_type, class T10 = detail::void_type, class T11 = detail::void_type, class T12 = detail::void_type, class T13 = detail::void_type, class T14 = detail::void_type, class T15 = detail::void_type, class T16 = detail::void_type, class T17 = detail::void_type, class T18 = detail::void_type, class T19 = detail::void_type, class T20 = detail::void_type, class T21 = detail::void_type, class T22 = detail::void_type, class T23 = detail::void_type, class T24 = detail::void_type>
	struct common_type;

	template<class _Tp>
	struct common_type<_Tp, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type, detail::void_type>
	{
		typedef typename decay<_Tp>::type type;
	};

} // namespace stdex

#endif // _STDEX_TYPE_TRAITS_H
