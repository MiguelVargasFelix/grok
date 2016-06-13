// Array3.h
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
	struct Array3
	{
		TYPE*** __restrict entry;

		int pages;

		int rows;

		int columns;

		inline Array3() throw()
		:	entry(static_cast<TYPE***>(0)),
			pages(0),
			rows(0),
			columns(0),
			manage(true)
		{
		}


		Array3(int pages, int rows, int columns, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
			try
			{
				entry = static_cast<TYPE***>(0);
				Resize(pages, rows, columns, alignment);
			}
			catch (MemoryException&)
			{
				ReThrow();
			}
		}


		Array3(const Array3& array3, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
		try
		{
			entry = static_cast<TYPE***>(0);
			Resize(array3.pages, array3.rows, array3.columns, alignment);
			for (int h = 0; h < pages; ++h)
			{
				for (int i = 0; i < rows; ++i)
				{
					register TYPE* __restrict array3__entry_h_i = array3.entry[h][i];
					register TYPE* __restrict entry_h_i = entry[h][i];
					for (register int j = 0; j < columns; ++j)
					{
						entry_h_i[j] = array3__entry_h_i[j];
					}
				}
			}
		}
		catch (MemoryException&)
		{
			ReThrow();
		}
		}


		inline ~Array3() throw()
		{
			if (entry)
			{
				delete [] entry[0][0];
				delete [] entry[0];
				delete [] entry;
			}
		}


		Array3& operator = (const Array3& array3) throw()
		{
			Assert(pages == array3.pages);
			Assert(rows == array3.rows);
			Assert(columns == array3.columns);

			for (int h = 0; h < pages; ++h)
			{
				for (int i = 0; i < rows; ++i)
				{
					register TYPE* __restrict array3_entry_h_i = array3.entry[h][i];
					register TYPE* __restrict entry_i = entry[h][i];
					for (register int j = 0; j < columns; ++j)
					{
						entry_i[j] = array3_entry_h_i[j];
					}
				}
			}
			return *this;
		}


		void Fill(const TYPE& value) throw()
		{
			for (int h = 0; h < pages; ++h)
			{
				for (register int i = 0; i < rows; ++i)
				{
					register TYPE* __restrict entry_h_i = entry[h][i];
					for (register int j = 0; j < columns; ++j)
					{
						entry_h_i[j] = value;
					}
				}
			}
		}


		void Resize(int pages, int rows, int columns, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
			Assert(pages > 0);
			Assert(rows > 0);
			Assert(columns > 0);

			if (entry)
			{
				delete [] entry[0][0];
				delete [] entry[0];
				delete [] entry;
			}
			if ((pages > 0) && (rows > 0) && (columns > 0))
			{
				entry = new(alignment) TYPE**[pages];
				if (entry)
				{
					register TYPE** entry_h = new(alignment) TYPE*[(size_t)pages*rows];
					if (entry_h)
					{
						unsigned int row_size = SIZE_WITH_PAD(TYPE, columns, align);
						register char* __restrict entry_h_i = new(alignment) char[(size_t)pages*(unsigned int)rows*(unsigned int)row_size];
						if (entry_h_i)
						{
							for (register int h = 0; h < pages; ++h)
							{
								for (register int i = 0; i < rows; ++i)
								{
									entry_h[i] = reinterpret_cast<TYPE*>(entry_h_i);
									entry_h_i += row_size;
								}
								entry[h] = entry_h;
								entry_h += rows;
							}
							this->pages = pages;
							this->rows = rows;
							this->columns = columns;
							return;
						}
						delete [] entry_h;
					}
					delete [] entry;
					entry = static_cast<TYPE***>(0);
				}
				this->pages = 0;
				this->rows = 0;
				this->columns = 0;
				Throw(MemoryException());
			}
			else
			{
				entry = static_cast<TYPE***>(0);
				this->pages = 0;
				this->rows = 0;
				this->columns = 0;
			}
		}
	};
}
