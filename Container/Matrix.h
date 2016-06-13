// Matrix.h
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
	struct Matrix
	{
		TYPE** __restrict entry;

		int rows;

		int columns;


		inline Matrix() throw()
		:	entry(static_cast<TYPE**>(0)),
			rows(0),
			columns(0)
		{
		}


		Matrix(int rows, int columns, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
			try
			{
				entry = static_cast<TYPE**>(0);
				Resize(rows, columns, alignment);
			}
			catch (MemoryException&)
			{
				ReThrow();
			}
		}


		Matrix(const Matrix<TYPE>& matrix, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
			try
			{
				entry = static_cast<TYPE**>(0);
				Resize(rows, columns, alignment);
				for (int i = 0; i < rows; ++i)
				{
					TYPE* __restrict entry_i = entry[i];
					TYPE* __restrict matrix_entry_i = matrix.entry[i];
					for (register int j = 0; j < columns; ++j)
					{
						entry_i[j] = matrix_entry_i[j];
					}
				}
			}
			catch (MemoryException&)
			{
				ReThrow();
			}
		}


		~Matrix() throw()
		{
			if (entry)
			{
				delete [] entry[0];
			}
			delete [] entry;
		}


		Matrix& operator = (const Matrix& matrix) throw()
		{
			Assert(rows == matrix.rows);
			Assert(columns == matrix.columns);

			for (int i = 0; i < rows; ++i)
			{
				TYPE* __restrict entry_i = entry[i];
				TYPE* __restrict matrix_entry_i = matrix.entry[i];
				for (register int j = 0; j < columns; ++j)
				{
					entry_i[j] = matrix_entry_i[j];
				}
			}
			return *this;
		}


		void Fill(const TYPE& value) throw()
		{
			for (int i = 0; i < rows; ++i)
			{
				TYPE* __restrict entry_i = entry[i];
				for (register int j = 0; j < columns; ++j)
				{
					entry_i[j] = value;
				}
			}
		}


		void Resize(int rows, int columns, unsigned short alignment = (DEFAULT_ALIGNMENT)) throw(MemoryException)
		{
			Assert(rows >= 0);
			Assert(columns >= 0);

			if (entry)
			{
				delete [] entry[0];
				delete [] entry;
			}
			if ((rows > 0) && (columns > 0))
			{
				entry = new(alignment) TYPE*[rows];
				if (entry)
				{
					unsigned int row_size = SIZE_WITH_PAD(TYPE, columns, alignment);
					char* __restrict entry_i = new(alignment) char[static_cast<size_t>(rows)*row_size];
					if (entry_i)
					{
						for (register int i = 0; i < rows; ++i)
						{
							entry[i] = reinterpret_cast<TYPE*>(entry_i);
							entry_i += row_size;
						}
						this->rows = rows;
						this->columns = columns;
						return;
					}
					delete [] entry;
				}
				this->rows = 0;
				this->columns = 0;
				Throw(MemoryException());
			}
			else
			{
				entry = static_cast<TYPE**>(0);
				this->rows = 0;
				this->columns = 0;
			}
		}
	};
}
