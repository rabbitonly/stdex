#ifndef _STDEX_RATIO_H
#define _STDEX_RATIO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// stdex includes
/*none*/

// POSIX includes
/*none*/

// std includes
#include <stdint.h>
#include <climits>

namespace stdex
{
#ifdef LLONG_MAX
	#define __INTMAX_MAX LLONG_MAX//9223372036854775807i64
	typedef ::intmax_t intmax_t;
	typedef ::uintmax_t uintmax_t;
#else
	#define __INTMAX_MAX LONG_MAX//	2147483647
	typedef int32_t intmax_t;
	typedef uint32_t uintmax_t;
#endif

	template<intmax_t _Val>
	struct _sign_of
	{   // computes sign of _Val
		static const intmax_t value = _Val < 0 ? -1 : 1;
	};

	template<intmax_t _Val>
	struct _abs
	{   // computes absolute value of _Val
		static const intmax_t value = _Val * _sign_of<_Val>::value;
	};

	// [Greatest common divisor template]

	template<intmax_t _Ax, intmax_t _Bx>
	struct _gcdX
	{   // computes greatest common divisor of _Ax and _Bx
		static const intmax_t value = _gcdX<_Bx, _Ax % _Bx>::value;
	};

	template<intmax_t _Ax>
	struct _gcdX<_Ax, 0>
	{   // computes greatest common divisor of _Ax and 0
		static const intmax_t value = _Ax;
	};

	template<intmax_t _Ax, intmax_t _Bx>
		struct _gcd
	{   // computes greatest common divisor of abs(_Ax) and abs(_Bx)
		static const intmax_t value =
			_gcdX<_abs<_Ax>::value, _abs<_Bx>::value>::value;
	};

	template<>
	struct _gcd<0, 0>
	{   // avoids division by 0 in ratio_less
		static const intmax_t value = 1;	// contrary to mathematical convention
	};

	namespace intern
	{
		// since we have no static_assert in pre-C++11 we just compile-time assert this way:
		struct ratio_asserts
		{
			template<bool> 
			struct overflow_in_multiplication_assert; // if you are there means overflow in safe template multiplication occured

			template<> 
			struct overflow_in_multiplication_assert<true> 
			{
				typedef bool is_ok;
			};

			template<bool>
			struct internal_library_error_assert; // if you are there means internal library error occured

			template<>
			struct internal_library_error_assert<true>
			{
				typedef bool is_ok;
			};

			template<bool>
			struct denominator_cant_be_zero_assert; // if you are there means you put the denominator to zero

			template<>
			struct denominator_cant_be_zero_assert<true>
			{
				typedef bool is_ok;
			};

			template<bool>
			struct out_of_range; // if you are there means that value is out of range

			template<>
			struct out_of_range<true>
			{
				typedef bool is_ok;
			};

			template<bool>
			struct division_by_zero; // if you are there means that divider is zero

			template<>
			struct division_by_zero<true>
			{
				typedef bool is_ok;
			};

			template<bool>
			struct overflow_in_addition_assert; // if you are there means overflow in safe template addition occured

			template<>
			struct overflow_in_addition_assert<true>
			{
				typedef bool is_ok;
			};
		};
	}

	// [Safe add template]

	template<intmax_t _Pn, intmax_t _Qn, bool>
	struct _add_overflow_check_impl
	{ 
		static const intmax_t value = _Pn <= __INTMAX_MAX - _Qn;
	};

	template<intmax_t _Pn, intmax_t _Qn>
	struct _add_overflow_check_impl<_Pn, _Qn, false>
	{ 
		static const intmax_t value = _Pn >= -__INTMAX_MAX - _Qn;
	};

	template<intmax_t _Pn, intmax_t _Qn>
	struct _add_overflow_check
		: _add_overflow_check_impl<_Pn, _Qn, (_Qn >= 0)>
	{ };

	template<intmax_t _Pn, intmax_t _Qn>
	struct _safe_add
	{
		static const intmax_t value = _Pn + _Qn;

	private:
		typedef intern::ratio_asserts check;

		typedef typename check::overflow_in_addition_assert<_add_overflow_check<_Pn, _Qn>::value != 0>::is_ok
			check1; // if you are there means overflow in safe template addition occured
	};

	// [Safe multiply template]

	// Let c = 2^(half # of bits in an intmax_t)
	// then we find a1, a0, b1, b0 s.t. N = a1*c + a0, M = b1*c + b0
	// The multiplication of N and M becomes,
	// N * M = (a1 * b1)c^2 + (a0 * b1 + b0 * a1)c + a0 * b0
	// Multiplication is safe if each term and the sum of the terms
	// is representable by intmax_t.
	template<intmax_t _Pn, intmax_t _Qn>
	struct _safe_multiply
	{
	private:
		static const uintmax_t _c = uintmax_t(1) << (sizeof(intmax_t) * 4);

		static const uintmax_t _a0 = _abs<_Pn>::value % _c;
		static const uintmax_t _a1 = _abs<_Pn>::value / _c;
		static const uintmax_t _b0 = _abs<_Qn>::value % _c;
		static const uintmax_t _b1 = _abs<_Qn>::value / _c;

		typedef intern::ratio_asserts check;

		typedef typename check::overflow_in_multiplication_assert< (_safe_multiply::_a1 == 0 || _safe_multiply::_b1 == 0) >::is_ok
			check1; // if you are there means overflow in safe template multiplication occured
		typedef typename check::overflow_in_multiplication_assert< (_safe_multiply::_a0 * _safe_multiply::_b1 + _safe_multiply::_b0 * _safe_multiply::_a1 < (_safe_multiply::_c >> 1)) >::is_ok
			check2; // if you are there means overflow in safe template multiplication occured
		typedef typename check::overflow_in_multiplication_assert< (_safe_multiply::_b0 * _safe_multiply::_a0 <= __INTMAX_MAX) >::is_ok
			check3; // if you are there means overflow in safe template multiplication occured
		typedef typename check::overflow_in_multiplication_assert< ((_safe_multiply::_a0 * _safe_multiply::_b1 + _safe_multiply::_b0 * _safe_multiply::_a1) * _safe_multiply::_c <= __INTMAX_MAX - _safe_multiply::_b0 * _safe_multiply::_a0) >::is_ok
			check4; // if you are there means overflow in safe template multiplication occured

	public:
		static const intmax_t value = _Pn * _Qn;
	};

	// [Less template]

	// Some double-precision utilities, where numbers are represented as
	// _hi*2^(8*sizeof(uintmax_t)) + _lo.
	template<uintmax_t _hi1, uintmax_t _lo1, uintmax_t _hi2, uintmax_t _lo2>
	struct _big_less
	{ 
		static const bool value = (_hi1 < _hi2 || (_hi1 == _hi2 && _lo1 < _lo2));
	};

	template<uintmax_t _hi1, uintmax_t _lo1, uintmax_t _hi2, uintmax_t _lo2>
	struct _big_add
	{
		static const uintmax_t _lo = _lo1 + _lo2;
		static const uintmax_t _hi = (_hi1 + _hi2 +
			(_lo1 + _lo2 < _lo1)); // carry
	};

	// [Subtract template]

	// Subtract a number from a bigger one.
	template<uintmax_t _hi1, uintmax_t _lo1, uintmax_t _hi2, uintmax_t _lo2>
	struct _big_sub
	{
		static const uintmax_t _lo = _lo1 - _lo2;
		static const uintmax_t _hi = (_hi1 - _hi2 -
			(_lo1 < _lo2)); // carry

	private:
		typedef intern::ratio_asserts check;
		typedef typename check::internal_library_error_assert< (!(_big_less<_hi1, _lo1, _hi2, _lo2>::value != 0)) >::is_ok
			check1; // if you are there means internal library error occured
	};

	// [Safe multiply for bigger numbers template]

	// Same principle as _safe_multiply.
	template<uintmax_t _x, uintmax_t _y>
	struct _big_multiply
	{
	private:
		static const uintmax_t _c = uintmax_t(1) << (sizeof(intmax_t) * 4);
		static const uintmax_t _x0 = _x % _c;
		static const uintmax_t _x1 = _x / _c;
		static const uintmax_t _y0 = _y % _c;
		static const uintmax_t _y1 = _y / _c;
		static const uintmax_t _x0y0 = _x0 * _y0;
		static const uintmax_t _x0y1 = _x0 * _y1;
		static const uintmax_t _x1y0 = _x1 * _y0;
		static const uintmax_t _x1y1 = _x1 * _y1;
		static const uintmax_t _mix = _x0y1 + _x1y0; // possible carry...
		static const uintmax_t _mix_lo = _mix * _c;
		static const uintmax_t _mix_hi
			= _mix / _c + ((_big_multiply::_mix < _x0y1) ? _c : 0); // ... added here
		typedef _big_add<_big_multiply::_mix_hi, _big_multiply::_mix_lo, _big_multiply::_x1y1, _big_multiply::_x0y0> _Res;
	public:
		static const uintmax_t _hi = _Res::_hi;
		static const uintmax_t _lo = _Res::_lo;
	};

	/**
	*  @brief Provides compile-time rational arithmetic.
	*
	*  This class template represents any finite rational number with a
	*  numerator and denominator representable by compile-time constants of
	*  type intmax_t. The ratio is simplified when instantiated.
	*
	*  For example:
	*  @code
	*    stdex::ratio<7,-21>::num == -1;
	*    stdex::ratio<7,-21>::den == 3;
	*  @endcode
	*
	*/
	template<intmax_t _Num, intmax_t _Den = 1>
	struct ratio
	{
		// Note: sign(N) * abs(N) == N
		static const intmax_t num =
			_Num * _sign_of<_Den>::value / _gcd<_Num, _Den>::value;

		static const intmax_t den =
			_abs<_Den>::value / _gcd<_Num, _Den>::value;

		typedef ratio<ratio::num, ratio::den> type;

	private:
		typedef intern::ratio_asserts check;

		typedef typename check::denominator_cant_be_zero_assert< (_Den != 0) >::is_ok 
			check1; // if you are there means you put the denominator to zero
		typedef typename check::out_of_range< (_Num >= -__INTMAX_MAX && _Den >= -__INTMAX_MAX) >::is_ok 
			check2; // if you are there means that value is out of range
	};

	template<class _R1, class _R2>
	struct _ratio_multiply
	{
	private:
		static const intmax_t _gcd1 =
			_gcd<_R1::num, _R2::den>::value;
		static const intmax_t _gcd2 =
			_gcd<_R2::num, _R1::den>::value;

	public:
		typedef ratio<
			_safe_multiply<(_R1::num / _ratio_multiply::_gcd1),
			(_R2::num / _ratio_multiply::_gcd2)>::value,
			_safe_multiply<(_R1::den / _ratio_multiply::_gcd2),
			(_R2::den / _ratio_multiply::_gcd1)>::value> type;

		static const intmax_t num = type::num;
		static const intmax_t den = type::den;
	};

	template<class _R1, class _R2>
	struct ratio_multiply:
		_ratio_multiply<_R1, _R2>::type
	{};

	template<class _R1, class _R2>
	struct _ratio_divide
	{
		typedef typename ratio_multiply<
			_R1,
			ratio<_R2::den, _R2::num> >::type type;

		static const intmax_t num = type::num;
		static const intmax_t den = type::den;

	private:
		typedef intern::ratio_asserts check;

		typedef typename check::out_of_range< (_R2::num != 0) >::is_ok
			check1;// if you are there means that divider is zero
	};

	template<class _R1, class _R2>
	struct ratio_divide :
		_ratio_divide<_R1, _R2>::type
	{};

	// ratio_equal
	template<class _R1, class _R2>
	struct ratio_equal
	{ 
		static const bool value = _R1::num == _R2::num && _R1::den == _R2::den;
	};

	// ratio_not_equal
	template<class _R1, class _R2>
	struct ratio_not_equal
	{ 
		static const bool value = !ratio_equal<_R1, _R2>::value;
	};


	// Both numbers are positive.
	template<class _R1, class _R2,
		class _Left = _big_multiply<_R1::num, _R2::den>,
		class _Right = _big_multiply<_R2::num, _R1::den> >
	struct _ratio_less_impl_1
	{ 
		static const bool value = _big_less<_Left::_hi, _Left::_lo, _Right::_hi, _Right::_lo>::value;
	};

	template<class _R1, class _R2,
		bool = (_R1::num == 0 || _R2::num == 0
			|| (_sign_of<_R1::num>::value
				!= _sign_of<_R2::num>::value)),
		bool = (_sign_of<_R1::num>::value == -1
			&& _sign_of<_R2::num>::value == -1)>
	struct _ratio_less_impl: 
		_ratio_less_impl_1<_R1, _R2>::type
	{ };

	template<class _R1, class _R2>
	struct _ratio_less_impl<_R1, _R2, true, false>
	{ 
		static const bool value = _R1::num < _R2::num;
	};

	template<class _R1, class _R2>
	struct _ratio_less_impl<_R1, _R2, false, true>: 
		_ratio_less_impl_1< ratio<-_R2::num, _R2::den>, ratio<-_R1::num, _R1::den> >::type
	{ };

	// ratio_less
	template<class _R1, class _R2>
	struct ratio_less
		: _ratio_less_impl<_R1, _R2>::type
	{ };

	// ratio_less_equal
	template<class _R1, class _R2>
	struct ratio_less_equal
	{ 
		static const bool value = !ratio_less<_R2, _R1>::value;
	};

	// ratio_greater
	template<class _R1, class _R2>
	struct ratio_greater
	{ 
		static const bool value = ratio_less<_R2, _R1>::value;
	};

	// ratio_greater_equal
	template<class _R1, class _R2>
	struct ratio_greater_equal
	{ 
		static const bool value = !ratio_less<_R1, _R2>::value;
	};

	// ratio_add
	template<class _R1, class _R2>
	struct _ratio_add
	{	// add two ratios

		static const intmax_t _n1 = _R1::num;
		static const intmax_t _d1 = _R1::den;
		static const intmax_t _n2 = _R2::num;
		static const intmax_t _d2 = _R2::den;

		static const intmax_t _gx = _gcd<_d1, _d2>::value;

		// typename ratio<>::type is necessary here
		typedef typename ratio<
			_safe_add<
			_safe_multiply<_ratio_add<_R1, _R2>::_n1, _ratio_add<_R1, _R2>::_d2 / _ratio_add<_R1, _R2>::_gx>::value,
			_safe_multiply<_ratio_add<_R1, _R2>::_n2, _ratio_add<_R1, _R2>::_d1 / _ratio_add<_R1, _R2>::_gx>::value
			>::value,
			_safe_multiply<_ratio_add<_R1, _R2>::_d1, _ratio_add<_R1, _R2>::_d2 / _ratio_add<_R1, _R2>::_gx>::value
		>::type type;
	};

	template<class _R1, class _R2>
	struct ratio_add :
		_ratio_add<_R1, _R2>::type
	{};

	template<class _R1, class _R2>
	struct _ratio_subtract
	{
		typedef typename _ratio_add<
			_R1,
			ratio<-_R2::num, _R2::den> >::type type;

		static const intmax_t num = type::num;
		static const intmax_t den = type::den;
	};

	template<class _R1, class _R2>
	struct ratio_subtract :
		_ratio_subtract<_R1, _R2>::type
	{};

	typedef ratio<1, 1000000000000000000> atto;
	typedef ratio<1,	1000000000000000> femto;
	typedef ratio<1,	   1000000000000> pico;
	typedef ratio<1,		  1000000000> nano;
	typedef ratio<1,			 1000000> micro;
	typedef ratio<1,				1000> milli;
	typedef ratio<1,				 100> centi;
	typedef ratio<1,				  10> deci;
	typedef ratio<				   10, 1> deca;
	typedef ratio<				  100, 1> hecto;
	typedef ratio<				 1000, 1> kilo;
	typedef ratio<			  1000000, 1> mega;
	typedef ratio<		   1000000000, 1> giga;
	typedef ratio<		1000000000000, 1> tera;
	typedef ratio<   1000000000000000, 1> peta;
	typedef ratio<1000000000000000000, 1> exa;

} // namespace stdex

#endif // _STDEX_THREAD_H