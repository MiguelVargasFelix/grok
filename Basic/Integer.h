// Integer.h
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

#pragma once

#include <Basic/Format.h>
#include <Basic/System.h>


#if defined(CC_Microsoft)

	namespace Grok
	{
		typedef signed __int8 sint8;
		typedef unsigned __int8 uint8;

		typedef signed __int16 sint16;
		typedef unsigned __int16 uint16;

		typedef signed __int32 sint32;
		typedef unsigned __int32 uint32;

		typedef signed __int64 sint64;
		typedef unsigned __int64 uint64;
	}

#elif defined(CC_GNU)

	namespace Grok
	{
		#if (__SCHAR_MAX__ == 127)
			typedef signed char sint8;
			typedef unsigned char uint8;
		#else
			#error Unknown type for 'sint8' and 'uint8'
		#endif

		#if (__SHRT_MAX__ == 32767)
			typedef signed short sint16;
			typedef unsigned short uint16;
		#else
			#error Unknown type for 'sint16' and 'uint16'
		#endif

		#if (__INT_MAX__ == 2147483647)
			typedef signed int sint32;
			typedef unsigned int uint32;
		#else
			#error Unknown type for 'sint32' and 'uint32'
		#endif

		#if (__LONG_MAX__ == 9223372036854775807L)
			typedef signed long sint64;
			typedef unsigned long uint64;
		#elif (__LONG_LONG_MAX__ == 9223372036854775807LL)
			typedef signed long long sint64;
			typedef unsigned long long uint64;
		#else
			#error Unknown type for 'sint64' and 'uint64'
		#endif
	}

#elif defined(CC_Intel)

	namespace Grok
	{
		typedef signed char sint8;
		typedef unsigned char uint8;

		typedef signed short sint16;
		typedef unsigned short uint16;

		typedef signed int sint32;
		typedef unsigned int uint32;

		#if defined(__i386__)
			typedef signed long long sint64;
			typedef unsigned long long uint64;
		#elif defined(__x86_64__)
			typedef signed long sint64;
			typedef unsigned long uint64;
		#endif
	}

#elif defined(CC_Clang)

	namespace Grok
	{
		typedef signed char sint8;
		typedef unsigned char uint8;

		#if (__SIZEOF_SHORT__ == 2)
			typedef signed short sint16;
			typedef unsigned short uint16;
		#else
			#error Unknown type for 'sint32' and 'uint32'
		#endif

		#if (__SIZEOF_INT__ == 4)
			typedef signed int sint32;
			typedef unsigned int uint32;
		#else
			#error Unknown type for 'sint32' and 'uint32'
		#endif

		#if (__SIZEOF_LONG__ == 8)
			typedef signed long sint64;
			typedef unsigned long uint64;
		#elif (__SIZEOF_LONG_LONG__ == 8)
			typedef signed long long sint64;
			typedef unsigned long long uint64;
		#else
			#error Unknown type for 'sint32' and 'uint32'
		#endif
	}

#endif


namespace Grok
{
	template <typename TYPE>
	struct Integer
	{
		static const int size;

		static const TYPE maximum;

		static const TYPE minimum;

		static const bool is_signed;
	};


	inline sint16 RoundToInt16(register float value) throw()
	{
		union
		{
			float f;
			sint16 i;
		} v;
		v.f = value + 12582912.0f;
		return v.i;
	}


	inline sint32 RoundToInt32(register float value) throw()
	{
		union
		{
			float f;
			sint32 i;
		} v;
		v.f = value + 12582912.0f;
		return v.i - 0x4B400000;
	}


	inline sint32 RoundToInt32(register double value) throw()
	{
		union
		{
			double f;
			sint32 i;
		} v;
		v.f = value + 6755399441055744.0;
		return v.i;
	}


	inline sint64 RoundToInt64(register double value) throw()
	{
		union
		{
			double f;
			sint64 i;
		} v;
		v.f = value + 6755399441055744.0;
		return v.i - 0x4338000000000000LL;
	}


	namespace IntegerNotation
	{
		enum ID
		{
			decimal,
			hexadecimal,
			HEXADECIMAL,
			octal
		};
	}


	struct IntegerFormat : public Format
	{
		IntegerFormat(bool space, bool sign, int width, IntegerNotation::ID notation, bool is_long = false) throw();
	};
}
