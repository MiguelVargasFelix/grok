// Memory.h
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

#include <Basic/Exception.h>
#include <Basic/System.h>


#define SIZE_WITH_PAD(type, count, align) (((sizeof(type)*static_cast<unsigned int>(count) + static_cast<unsigned int>(align) - 1)/static_cast<unsigned int>(align))*static_cast<unsigned int>(align))

#define USE_ALLOCA_SIZE 1000


namespace Grok
{
	struct MemoryException : public Exception
	{
	};
}


#if defined(CC_Microsoft) || (defined(CC_GNU) && defined(OS_Windows))

	#include <malloc.h>


	#define Alloca(TYPE, size) reinterpret_cast<TYPE*>(_alloca(sizeof(TYPE)*static_cast<unsigned int>(size)))

#else

	#include <alloca.h>


	#define Alloca(TYPE, size) reinterpret_cast<TYPE*>(alloca(sizeof(TYPE)*static_cast<unsigned int>(size)))

#endif


void* operator new (size_t size, unsigned short alignment) throw();


void* operator new [] (size_t size, unsigned short alignment) throw();


void operator delete (void* object) throw();


void operator delete [] (void* vector) throw();
