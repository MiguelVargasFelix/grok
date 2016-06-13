// Vector.h
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
	template <typename TYPE>
	struct Vector
	{
		TYPE* __restrict entry;

		int size;


		inline Vector() throw()
		:	entry(static_cast<TYPE*>(0)),
			size(0)
		{
		}


		Vector(int size, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
			try
			{
				entry = static_cast<TYPE*>(0);
				Resize(size, alignment);
			}
			catch (MemoryException&)
			{
				ReThrow();
			}
		}


		Vector(const Vector<TYPE>& vector) throw(MemoryException)
		{
			try
			{
				entry = static_cast<TYPE*>(0);
				Resize(size);
				for (register int i = 0; i < size; ++i)
				{
					entry[i] = vector.entry[i];
				}
			}
			catch (MemoryException&)
			{
				ReThrow();
			}
		}


		~Vector() throw()
		{
			delete [] entry;
		}


		Vector& operator = (const Vector& vector) throw()
		{
			Assert(size == vector.size);

			for (register int i = 0; i < size; ++i)
			{
				entry[i] = vector.entry[i];
			}
			return *this;
		}


		void Fill(const TYPE& value) throw()
		{
			for (register int i = 0; i < size; ++i)
			{
				entry[i] = value;
			}
		}


		void Fill(const TYPE* __restrict data) throw()
		{
			Assert(data);

			for (register int i = 0; i < size; ++i)
			{
				entry[i] = data[i];
			}
		}


		void Resize(int size, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
			#if defined(CC_Intel)
				#pragma warning(push)
				#pragma warning(disable: 873) // entity-kind "entity" has no corresponding operator deletexxxx (to be called if an exception is thrown during initialization of an allocated object)
			#endif

			Assert(size >= 0);

			if (entry)
			{
				delete [] entry;
			}
			if (size > 0)
			{
				entry = new(alignment) TYPE[static_cast<size_t>(size)];
				if (!entry)
				{
					this->size = 0;
					Throw(MemoryException());
				}
				this->size = size;
			}
			else
			{
				entry = static_cast<TYPE*>(0);
				this->size = 0;
			}

			#if defined(CC_Intel)
				#pragma warning(pop)
			#endif
		}
	};
}
