// Memory.cpp
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

#include <Basic/Memory.h>

#if defined(OS_MacOSX)

	#include <malloc/malloc.h>

#elif defined(OS_Windows)

	#include <malloc.h>

#elif defined(OS_Cygwin) || defined(OS_Linux)

	#include <stdlib.h>

#elif defined(OS_FreeBSD)

	#include <malloc_np.h>

#endif


void* operator new (size_t size, unsigned short alignment) throw()
{
	#if defined(OS_Windows)

		return _aligned_malloc(size, alignment);

	#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_FreeBSD) || defined(OS_Linux)

		void* memory;
		return posix_memalign(&memory, alignment, size) ? static_cast<void*>(0) : memory;

	#endif
}


void* operator new [] (size_t size, unsigned short alignment) throw()
{
	#if defined(OS_Windows)

		return _aligned_malloc(size, alignment);

	#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_FreeBSD) || defined(OS_Linux)

		void* memory;
		return posix_memalign(&memory, alignment, size) ? static_cast<void*>(0) : memory;

	#endif
}


void operator delete (void* object) throw()
{
	#if defined(OS_Windows)

		_aligned_free(object);

	#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_FreeBSD) || defined(OS_Linux)

		free(object);

	#endif
}


void operator delete [] (void* objects) throw()
{
	#if defined(OS_Windows)

		_aligned_free(objects);

	#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_FreeBSD) || defined(OS_Linux)

		free(objects);

	#endif
}
