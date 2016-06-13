// System.h
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


#if defined(__APPLE__) && defined(__MACH__)

	#define OS_MacOSX
	#define OS_NAME "OS X"

#elif defined(__CYGWIN__)

	#define OS_Cygwin
	#define OS_NAME "Cygwin"

#elif defined(__FreeBSD__)

	#define OS_FreeBSD
	#define OS_NAME "FreeBSD"

#elif defined(__linux__)

	#define OS_Linux
	#define OS_NAME "GNU/Linux"

#elif defined(_WIN32)

	#define OS_Windows
	#define OS_NAME "Windows"

#else

	#error Unsupported operating system

#endif


#if defined(_MSC_VER)

	#define CC_Microsoft
	#define CC_NAME "Microsoft Visual C++"

#elif defined(__clang__)

	#define CC_Clang
	#define CC_NAME "LLVM C++"

#elif defined(__INTEL_COMPILER)

	#define CC_Intel
	#define CC_NAME "Intel C++"

#elif defined(__GNUC__)

	#define CC_GNU
	#define CC_NAME "GNU G++"

#else

	#error Unsupported compiler

#endif


#if defined(CC_Microsoft)

	#if defined(_M_X64)
		#define BITNESS_64
	#else
		#define BITNESS_32
	#endif

	#pragma warning(disable: 4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
	#pragma warning(disable: 4458) // declaration of 'xxxx' hides class member

#elif defined(CC_Clang)

	typedef __SIZE_TYPE__ size_t;

	#if defined(__x86_64)
		#define BITNESS_64
	#else
		#define BITNESS_32
	#endif

#elif defined(CC_Intel)

	typedef __SIZE_TYPE__ size_t;

	#if defined(__x86_64)
		#define BITNESS_64
	#else
		#define BITNESS_32
	#endif

#elif defined(CC_GNU)

	typedef __SIZE_TYPE__ size_t;

	#if defined(__x86_64)
		#define BITNESS_64
	#else
		#define BITNESS_32
	#endif

	#if defined(OS_Windows) // MinGW
		#define __MSVCRT_VERSION__ 0x800
	#endif

#endif


#define BUILD_DATE __DATE__

#define BUILD_TIME __TIME__

#define DEFAULT_ALIGNMENT 16
