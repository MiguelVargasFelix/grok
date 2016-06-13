// Integer.cpp
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
#include <Basic/Integer.h>


#if defined(CC_Microsoft)

	#if defined(_CHAR_UNSIGNED)
		#define CHAR_SIGNED 0
		#define CHAR_MAXIMUM static_cast<char>(255)
		#define CHAR_MINIMUM static_cast<char>(0)
	#else
		#define CHAR_SIGNED 1
		#define CHAR_MAXIMUM static_cast<char>(127)
		#define CHAR_MINIMUM static_cast<char>(-127 -1)
	#endif

	#define SCHAR_MAXIMUM static_cast<signed char>(127)
	#define SCHAR_MINIMUM static_cast<signed char>(-127 - 1)

	#define UCHAR_MAXIMUM static_cast<unsigned char>(255)
	#define UCHAR_MINIMUM static_cast<unsigned char>(0)

	#define SSHORT_MAXIMUM static_cast<signed short>(32767)
	#define SSHORT_MINIMUM static_cast<signed short>(-32767 - 1)

	#define USHORT_MAXIMUM static_cast<unsigned short>(65535)
	#define USHORT_MINIMUM static_cast<unsigned short>(0)

	#define SINT_MAXIMUM static_cast<signed int>(2147483647)
	#define SINT_MINIMUM static_cast<signed int>(-2147483647 - 1)

	#define UINT_MAXIMUM static_cast<unsigned int>(4294967295U)
	#define UINT_MINIMUM static_cast<unsigned int>(0U)

	#define SLONG_MAXIMUM static_cast<signed long>(2147483647L)
	#define SLONG_MINIMUM static_cast<signed long>(-2147483647L - 1)

	#define ULONG_MAXIMUM static_cast<unsigned long>(4294967295UL)
	#define ULONG_MINIMUM static_cast<unsigned long>(0U)

	#define SLLONG_MAXIMUM static_cast<signed long long>(9223372036854775807LL)
	#define SLLONG_MINIMUM static_cast<signed long long>(-9223372036854775807LL - 1)

	#define ULLONG_MAXIMUM static_cast<unsigned long long>(18446744073709551615ULL)
	#define ULLONG_MINIMUM static_cast<unsigned long long>(0U)

#elif defined(CC_GNU) || defined(CC_Intel) || defined(CC_Clang)

	#if defined(__CHAR_UNSIGNED__)
		#define CHAR_SIGNED 0
		#define CHAR_MAXIMUM static_cast<char>(255)
		#define CHAR_MINIMUM static_cast<char>(0)
	#else
		#define CHAR_SIGNED 1
		#define CHAR_MAXIMUM static_cast<char>(127)
		#define CHAR_MINIMUM static_cast<char>(-128)
	#endif

	#define SCHAR_MAXIMUM static_cast<signed char>(127)
	#define SCHAR_MINIMUM static_cast<signed char>(-128)

	#define UCHAR_MAXIMUM static_cast<unsigned char>(255)
	#define UCHAR_MINIMUM static_cast<unsigned char>(0)

	#define SSHORT_MAXIMUM static_cast<signed short>(__SHRT_MAX__)
	#define SSHORT_MINIMUM static_cast<signed short>(-__SHRT_MAX__ - 1)

	#define USHORT_MAXIMUM static_cast<unsigned short>((__SHRT_MAX__ << 1) + 1)
	#define USHORT_MINIMUM static_cast<unsigned short>(0)

	#define SINT_MAXIMUM static_cast<signed int>(__INT_MAX__)
	#define SINT_MINIMUM static_cast<signed int>(-__INT_MAX__ - 1)

	#define UINT_MAXIMUM static_cast<unsigned int>((static_cast<unsigned int>(__INT_MAX__) << 1) + 1)
	#define UINT_MINIMUM static_cast<unsigned int>(0U)

	#define SLONG_MAXIMUM static_cast<signed long>(__LONG_MAX__)
	#define SLONG_MINIMUM static_cast<signed long>(-__LONG_MAX__ - 1)

	#define ULONG_MAXIMUM static_cast<unsigned long>((static_cast<unsigned long>(__LONG_MAX__) << 1) + 1)
	#define ULONG_MINIMUM static_cast<unsigned long>(0UL)

	#define SLLONG_MAXIMUM static_cast<signed long long>(__LONG_LONG_MAX__)
	#define SLLONG_MINIMUM static_cast<signed long long>(-__LONG_LONG_MAX__ - 1)

	#define ULLONG_MAXIMUM static_cast<unsigned long long>((static_cast<unsigned long long>(__LONG_LONG_MAX__) << 1) + 1)
	#define ULLONG_MINIMUM static_cast<unsigned long long>(0ULL)

#endif


namespace Grok
{
	template struct Integer<char>;
	template <> const int Integer<char>::size = sizeof(char);
	template <> const char Integer<char>::maximum = CHAR_MAXIMUM;
	template <> const char Integer<char>::minimum = CHAR_MINIMUM;
	template <> const bool Integer<char>::is_signed = CHAR_SIGNED;

	template struct Integer<signed char>;
	template <> const int Integer<signed char>::size = sizeof(signed char);
	template <> const signed char Integer<signed char>::maximum = SCHAR_MAXIMUM;
	template <> const signed char Integer<signed char>::minimum = SCHAR_MINIMUM;
	template <> const bool Integer<signed char>::is_signed = true;


	template struct Integer<unsigned char>;
	template <> const int Integer<unsigned char>::size = sizeof(unsigned char);
	template <> const unsigned char Integer<unsigned char>::maximum = UCHAR_MAXIMUM;
	template <> const unsigned char Integer<unsigned char>::minimum = UCHAR_MINIMUM;
	template <> const bool Integer<unsigned char>::is_signed = false;


	template struct Integer<signed short>;
	template <> const int Integer<signed short>::size = sizeof(signed short);
	template <> const signed short Integer<signed short>::maximum = SSHORT_MAXIMUM;
	template <> const signed short Integer<signed short>::minimum = SSHORT_MINIMUM;
	template <> const bool Integer<signed short>::is_signed = true;


	template struct Integer<unsigned short>;
	template <> const int Integer<unsigned short>::size = sizeof(unsigned short);
	template <> const unsigned short Integer<unsigned short>::maximum = USHORT_MAXIMUM;
	template <> const unsigned short Integer<unsigned short>::minimum = USHORT_MINIMUM;
	template <> const bool Integer<unsigned short>::is_signed = false;


	template struct Integer<signed int>;
	template <> const int Integer<signed int>::size = sizeof(signed int);
	template <> const signed int Integer<signed int>::maximum = SINT_MAXIMUM;
	template <> const signed int Integer<signed int>::minimum = SINT_MINIMUM;
	template <> const bool Integer<signed int>::is_signed = true;


	template struct Integer<unsigned int>;
	template <> const int Integer<unsigned int>::size = sizeof(unsigned int);
	template <> const unsigned int Integer<unsigned int>::maximum = UINT_MAXIMUM;
	template <> const unsigned int Integer<unsigned int>::minimum = UINT_MINIMUM;
	template <> const bool Integer<unsigned int>::is_signed = false;


	template struct Integer<signed long>;
	template <> const int Integer<signed long>::size = sizeof(signed long);
	template <> const signed long Integer<signed long>::maximum = SLONG_MAXIMUM;
	template <> const signed long Integer<signed long>::minimum = SLONG_MINIMUM;
	template <> const bool Integer<signed long>::is_signed = true;


	template struct Integer<unsigned long>;
	template <> const int Integer<unsigned long>::size = sizeof(unsigned long);
	template <> const unsigned long Integer<unsigned long>::maximum = ULONG_MAXIMUM;
	template <> const unsigned long Integer<unsigned long>::minimum = ULONG_MINIMUM;
	template <> const bool Integer<unsigned long>::is_signed = false;


	template struct Integer<signed long long>;
	template <> const int Integer<signed long long>::size = sizeof(signed long long);
	template <> const signed long long Integer<signed long long>::maximum = SLLONG_MAXIMUM;
	template <> const signed long long Integer<signed long long>::minimum = SLLONG_MINIMUM;
	template <> const bool Integer<signed long long>::is_signed = true;


	template struct Integer<unsigned long long>;
	template <> const int Integer<unsigned long long>::size = sizeof(unsigned long long);
	template <> const unsigned long long Integer<unsigned long long>::maximum = ULLONG_MAXIMUM;
	template <> const unsigned long long Integer<unsigned long long>::minimum = ULLONG_MINIMUM;
	template <> const bool Integer<unsigned long long>::is_signed = false;


	IntegerFormat::IntegerFormat(bool space, bool sign, int width, IntegerNotation::ID notation, bool is_long) throw()
	{
		Assert(width > 0);

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
		if (is_long)
		{
			*(def++) = 'l';
		}
		*(def++) = (notation == IntegerNotation::decimal) ? 'd' : (notation == IntegerNotation::hexadecimal) ? 'x' : (notation == IntegerNotation::HEXADECIMAL) ? 'X' : 'o';
		*def = '\0';
	}
}
