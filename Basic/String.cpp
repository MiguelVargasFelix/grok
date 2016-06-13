// String.cpp
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

#include <Basic/String.h>

#include <stdarg.h>
#include <stdio.h>


namespace Grok
{
	String::String(int size) throw(MemoryException)
	{
		try
		{
			entry = static_cast<char*>(0);
			Resize(size);
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	String::String(const String& string) throw(MemoryException)
	{
		try
		{
			entry = static_cast<char*>(0);
			Set(string);
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	String::String(const char* __restrict string) throw(MemoryException)
	{
		try
		{
			entry = static_cast<char*>(0);
			Set(string);
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	String::String(const char* __restrict string, int length) throw(MemoryException)
	{
		try
		{
			entry = static_cast<char*>(0);
			Set(string, length);
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	String& String::operator = (const String& string) throw(MemoryException)
	{
		try
		{
			Set(string);
			return *this;
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	String& String::operator = (const char* string) throw(MemoryException)
	{
		try
		{
			Set(string);
			return *this;
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	void String::Clear() throw()
	{
		delete [] entry;
		entry = static_cast<char*>(0);
		this->size = 0;
	}


	int String::Length() const throw()
	{
		if (entry)
		{
			register const char* __restrict c = entry;
			while (*c)
			{
				++c;
			}
			return static_cast<int>(c - entry);
		}
		return 0;
	}


	void String::Resize(int size) throw(MemoryException)
	{
		Assert(size >= 0);

		if (entry)
		{
			delete [] entry;
		}
		if (size > 0)
		{
			entry = new(DEFAULT_ALIGNMENT) char[static_cast<size_t>(size)];
			if (!entry)
			{
				this->size = 0;
				Throw(MemoryException());
			}
			this->size = size;
		}
		else
		{
			entry = static_cast<char*>(0);
			this->size = 0;
		}
	}


	void String::Set(const String& string) throw(MemoryException)
	{
		try
		{
			Resize(string.size);
			for (register int i = 0; i < size; ++i)
			{
				entry[i] = string.entry[i];
			}
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	void String::Set(const char* __restrict string) throw(MemoryException)
	{
		try
		{
			Resize(Grok::Length(string) + 1);
			for (register int i = 0; i < size; ++i)
			{
				entry[i] = string[i];
			}
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	void String::Set(const char* __restrict string, int length) throw(MemoryException)
	{
		Assert(length >= 0);

		try
		{
			Resize(length + 1);
			for (register int i = 0; i < length; ++i)
			{
				entry[i] = string[i];
			}
			entry[length] = '\0';
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
	}


	void String::Set(int max_size, const char* format, ...) throw(MemoryException)
	{
		#if defined(CC_Clang)
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wformat-nonliteral" // format string is not a string literal
		#endif

		Assert(max_size > 0);

		try
		{
			char* string = max_size <= USE_ALLOCA_SIZE ? Alloca(char, max_size) : new(DEFAULT_ALIGNMENT) char[static_cast<size_t>(max_size)];
			va_list arguments;
			va_start(arguments, format);
			vsnprintf(string, static_cast<size_t>(max_size), format, arguments);
			va_end(arguments);
			Resize(Grok::Length(string) + 1);
			for (register int i = 0; i < size; ++i)
			{
				entry[i] = string[i];
			}
			if (max_size > USE_ALLOCA_SIZE)
			{
				delete [] string;
			}
		}
		catch (MemoryException&)
		{
			ReThrow();
		}

		#if defined(CC_Clang)
			#pragma clang diagnostic pop
		#endif
	}


	int Length(const char* __restrict string) throw()
	{
		Assert(string);

		register const char* __restrict c = string;
		while (*c)
		{
			++c;
		}
		return static_cast<int>(c - string);
	}


	bool IsEqual(const char* __restrict a, const char* __restrict b) throw()
	{
		Assert(a);
		Assert(b);

		register const char* __restrict ptr_a = a;
		register const char* __restrict ptr_b = b;
		while (*ptr_a)
		{
			if (*ptr_a != *ptr_b)
			{
				return false;
			}
			++ptr_a;
			++ptr_b;
		}
		if (*ptr_b != '\0')
		{
			return false;
		}
		return true;
	}


	bool IsEqual(const char* __restrict a, const char* __restrict b, int length) throw()
	{
		Assert(a);
		Assert(b);
		Assert(length >= 0);

		register const char* __restrict ptr_a = a;
		register const char* __restrict ptr_b = b;
		while (*ptr_a && length)
		{
			if (*ptr_a != *ptr_b)
			{
				return false;
			}
			++ptr_a;
			++ptr_b;
			--length;
		}
		if (*ptr_b != '\0')
		{
			return false;
		}
		return true;
	}
}

