// String.h
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

#include <Basic/Assert.h>
#include <Basic/Memory.h>


namespace Grok
{
	class String
	{
		public:

			char* entry;

			int size;
		

			inline String() throw()
			:	entry(static_cast<char*>(0)),
				size(0)
			{
			}


			String(int size) throw(MemoryException);


			String(const String& string) throw(MemoryException);


			String(const char* string) throw(MemoryException);


			String(const char* string, int length) throw(MemoryException);


			inline ~String() throw()
			{
				delete [] entry;
			}


			String& operator = (const String& string) throw(MemoryException);


			String& operator = (const char* string) throw(MemoryException);


			void Clear() throw();


			int Length() const throw();


			void Resize(int size) throw(MemoryException);


			void Set(const String& string) throw(MemoryException);


			void Set(const char* string) throw(MemoryException);


			void Set(const char* string, int length) throw(MemoryException);


			void Set(int max_size, const char* format, ...) throw(MemoryException);
	};


	int Length(const char* string) throw();


	bool IsEqual(const char* a, const char* b) throw();


	bool IsEqual(const char* a, const char* b, int length) throw();


	inline bool operator == (const String& string_a, const String& string_b) throw()
	{
		return IsEqual(string_a.entry, string_b.entry);
	}


	inline bool operator == (const String& string_a, const char* string_b) throw()
	{
		return IsEqual(string_a.entry, string_b);
	}


	inline bool operator == (const char* string_a, const String& string_b) throw()
	{
		return IsEqual(string_a, string_b.entry);
	}
}
