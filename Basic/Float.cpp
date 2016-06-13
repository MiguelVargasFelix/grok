// Float.cpp
// Copyright (C) 2016 Miguel Vargas-Felix (miguel.vargas@gmail.com)
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include <Basic/Assert.h>
#include <Basic/Float.h>
#include <Basic/Integer.h>
#include <Basic/System.h>


#if defined(CC_Microsoft)

	#define FLOAT_MAXIMUM 3.402823466e+38F
	#define FLOAT_MINIMUM -3.402823466e+38F
	#define FLOAT_EPSILON 1.192092896e-07F
	#define FLOAT_INFINITY FLOAT_MAXIMUM*FLOAT_MAXIMUM

	#define DOUBLE_MAXIMUM 1.7976931348623158e+308
	#define DOUBLE_MINIMUM -1.7976931348623158e+308
	#define DOUBLE_EPSILON 2.2204460492503131e-016
	#define DOUBLE_INFINITY DOUBLE_MAXIMUM*DOUBLE_MAXIMUM

#elif defined(CC_GNU) || defined(CC_Intel) || defined(CC_Clang)

	#define FLOAT_MAXIMUM __FLT_MAX__
	#define FLOAT_MINIMUM -__FLT_MAX__
	#define FLOAT_EPSILON __FLT_EPSILON__
	#define FLOAT_INFINITY __builtin_inff()

	#define DOUBLE_MAXIMUM __DBL_MAX__
	#define DOUBLE_MINIMUM -__DBL_MAX__
	#define DOUBLE_EPSILON __DBL_EPSILON__
	#define DOUBLE_INFINITY __builtin_inff()

#endif


template <typename A, typename B>
union ConvertBits
{
	A a;
	B b;

	inline ConvertBits(A a) : a(a) {}
};


namespace Grok
{
	#if defined(CC_Microsoft)
		#pragma warning(push)
		#pragma warning(disable: 4056) // overflow in floating-point constant arithmetic
	#endif

	template struct Float<float>;
	template <> const int Float<float>::size = sizeof(float);
	template <> const float Float<float>::maximum = FLOAT_MAXIMUM;
	template <> const float Float<float>::minimum = FLOAT_MINIMUM;
	template <> const float Float<float>::epsilon = FLOAT_EPSILON;
	template <> const float Float<float>::infinity = FLOAT_INFINITY;


	template <> bool Float<float>::IsNaN(float value) throw()
	{
		ConvertBits<float, uint32> m(value);
		return ((m.b & static_cast<uint32>(0x7F800000U)) == static_cast<uint32>(0x7F800000U)) && ((m.b & static_cast<uint32>(0x007FFFFFU)) != 0);
	}


	template struct Float<double>;
	template <> const int Float<double>::size = sizeof(double);
	template <> const double Float<double>::maximum = DOUBLE_MAXIMUM;
	template <> const double Float<double>::minimum = DOUBLE_MINIMUM;
	template <> const double Float<double>::epsilon = DOUBLE_EPSILON;
	template <> const double Float<double>::infinity = DOUBLE_INFINITY;


	template <> bool Float<double>::IsNaN(double value) throw()
	{
		ConvertBits<double, uint64> m(value);
		return ((m.b & static_cast<uint64>(0x7FE0000000000000ULL)) == static_cast<uint64>(0x7FE0000000000000ULL)) && ((m.b & static_cast<uint64>(0x000FFFFFFFFFFFFFULL)) != 0);
	}

	#if defined(CC_Microsoft)
		#pragma warning(pop)
	#endif


	FloatFormat::FloatFormat(bool space, bool sign, int width, int precision, FloatNotation::ID notation, bool is_long) throw()
	{
		Assert(width >= 0);
		Assert(precision >= 0);

		char digits[40];

		register char* __restrict def = definition;

		*(def++) = '%';
		if (space)
		{
			*(def++) = ' ';
		}
		if (sign)
		{
			*(def++) = '+';
		}
		{
			register char* __restrict d = digits;

			do
			{
				*(d++) = static_cast<char>((width % 10) + '0');
				width /= 10;
			} while (width);
			do
			{
				*(def++) = *(--d);
			} while (d > digits);
		}
		*(def++) = '.';
		{
			register char* __restrict d = digits;

			do
			{
				*(d++) = static_cast<char>((precision % 10) + '0');
				precision /= 10;
			} while (precision);
			do
			{
				*(def++) = *(--d);
			} while (d > digits);
		}
		if (is_long)
		{
			*(def++) = 'l';
		}
		*(def++) = (notation == FloatNotation::fixed) ? 'f' : (notation == FloatNotation::exponential) ? 'e' : 'g';
		*def = '\0';
	}
}
