// Sort.h
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
#include <Basic/Integer.h>
#include <Basic/Memory.h>
#include <Container/Vector.h>

#if !defined(SORT_RADIX_COMB_LIMIT)
	#define SORT_RADIX_COMB_LIMIT 64
#endif


namespace Grok
{
	template <typename TYPE>
	void CombSort(TYPE* vector, int size) throw()
	{
		Assert(vector);
		Assert(size >= 0);

		// http://en.wikipedia.org/wiki/Combsort#Combsort11
		register bool swapped;
		register int gap = size;
		do
		{
			gap *= 93;
			gap /= 116;
			if ((gap == 9) || (gap == 10))
			{
				gap = 11;
			}
			else if (gap == 0)
			{
				gap = 1;
			}
			swapped = false;
			for (register int i = gap; i < size; ++i)
			{
				register int j = i - gap;
				if (vector[i] < vector[j])
				{
					register TYPE tmp = vector[i];
					vector[i] = vector[j];
					vector[j] = tmp;
					swapped = true;
				}
			}
		} while (swapped || (gap > 1));
	}


	template <typename TYPE>
	inline void CombSort(Vector<TYPE>& vector) throw()
	{
		CombSort(vector.entry, vector.size);
	}


	template <typename TYPE, typename KEY>
	void CombSort(TYPE* vector, int size, KEY TYPE::* key) throw()
	{
		Assert(vector);
		Assert(size >= 0);

		// http://en.wikipedia.org/wiki/Combsort#Combsort11
		register char* __restrict keys = reinterpret_cast<char*>(&(vector[0].*key));
		register bool swapped;
		register int gap = size;
		do
		{
			gap *= 93;
			gap /= 116;
			if ((gap == 9) || (gap == 10))
			{
				gap = 11;
			}
			else if (gap == 0)
			{
				gap = 1;
			}
			swapped = false;
			for (register int i = gap; i < size; ++i)
			{
				register int j = i - gap;
				if (*reinterpret_cast<KEY*>(keys + static_cast<unsigned int>(i)*sizeof(TYPE)) < *reinterpret_cast<KEY*>(keys + static_cast<unsigned int>(j)*sizeof(TYPE)))
				{
					TYPE tmp = vector[i];
					vector[i] = vector[j];
					vector[j] = tmp;
					swapped = true;
				}
			}
		} while (swapped || (gap > 1));
	}


	template <typename TYPE, typename KEY>
	inline void CombSort(Vector<TYPE>& vector, KEY TYPE::* key) throw()
	{
		CombSort(vector.entry, vector.size, key);
	}


	void RadixSort(uint8* vector, int size) throw(MemoryException);


	void RadixSort(sint8* vector, int size) throw(MemoryException);


	void RadixSort(uint16* vector, int size) throw(MemoryException);


	void RadixSort(sint16* vector, int size) throw(MemoryException);


	void RadixSort(uint32* vector, int size) throw(MemoryException);


	void RadixSort(sint32* vector, int size) throw(MemoryException);


	void RadixSort(uint64* vector, int size) throw(MemoryException);


	void RadixSort(sint64* vector, int size) throw(MemoryException);


	void RadixSort(float* vector, int size) throw(MemoryException);


	void RadixSort(double* vector, int size) throw(MemoryException);


	template <typename TYPE>
	inline void RadixSort(Vector<TYPE>& vector) throw(MemoryException)
	{
		RadixSort(vector.entry, vector.size);
	}


	template <typename TYPE>
	void RadixSort(TYPE* vector, int size, sint32 TYPE::* key) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		struct Data
		{
			uint32 value;
			int index;
		};

		register Data* __restrict a = new(DEFAULT_ALIGNMENT) Data[size];
		register Data* __restrict b = new(DEFAULT_ALIGNMENT) Data[size];
		register TYPE* __restrict old = new(DEFAULT_ALIGNMENT) TYPE[size];
		if ((!a) || (!b) || (!old))
		{
			delete [] old;
			delete [] b;
			delete [] a;
			Throw(MemoryException());
		}

		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};

		{
			register char* k = reinterpret_cast<char*>(&(vector[0].*key));
			for (register int i = 0; i < size; ++i, k += sizeof(TYPE))
			{
				old[i] = vector[i];

				register uint32 value = *reinterpret_cast<uint32*>(k) ^ 0x80000000;
				++histogram0[value & 0xff];
				++histogram1[value >> 8 & 0xff];
				++histogram2[value >> 16 & 0xff];
				++histogram3[value >> 24];

				a[i].value = value;
				a[i].index = i;
			}
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			register int count2 = 0;
			register int count3 = 0;
			for (register sint16 h = 0; h < 256; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;

				tmp = histogram2[h];
				histogram2[h] = count2;
				count2 += tmp;

				tmp = histogram3[h];
				histogram3[h] = count3;
				count3 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram0[a[i].value & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram1[b[i].value >> 8 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram2[a[i].value >> 16 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			vector[histogram3[b[i].value >> 24]++] = old[b[i].index];
		}

		delete [] old;
		delete [] b;
		delete [] a;
	}


	template <typename TYPE>
	void RadixSort(TYPE* vector, int size, sint64 TYPE::* key) throw(MemoryException)
	{
		Assert(size >= 0);

		struct Data
		{
			uint64 value;
			int index;
		};

		register Data* __restrict a = new(DEFAULT_ALIGNMENT) Data[size];
		register Data* __restrict b = new(DEFAULT_ALIGNMENT) Data[size];
		register TYPE* __restrict old = new(DEFAULT_ALIGNMENT) TYPE[size];
		if ((!a) || (!b) || (!old))
		{
			delete [] old;
			delete [] b;
			delete [] a;
			Throw(MemoryException());
		}

		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};
		int histogram4[256] = {0};
		int histogram5[256] = {0};
		int histogram6[256] = {0};
		int histogram7[256] = {0};

		{
			register char* k = reinterpret_cast<char*>(&(vector[0].*key));
			for (register int i = 0; i < size; ++i, k += sizeof(TYPE))
			{
				old[i] = vector[i];

				register uint64 value = *reinterpret_cast<uint64*>(k) ^ 0x8000000000000000;
				++histogram0[value & 0xff];
				++histogram1[value >> 8 & 0xff];
				++histogram2[value >> 16 & 0xff];
				++histogram3[value >> 24 & 0xff];
				++histogram4[value >> 32 & 0xff];
				++histogram5[value >> 40 & 0xff];
				++histogram6[value >> 48 & 0xff];
				++histogram7[value >> 56];

				a[i].value = value;
				a[i].index = i;
			}
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			register int count2 = 0;
			register int count3 = 0;
			register int count4 = 0;
			register int count5 = 0;
			register int count6 = 0;
			register int count7 = 0;
			for (register sint16 h = 0; h < 256; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;

				tmp = histogram2[h];
				histogram2[h] = count2;
				count2 += tmp;

				tmp = histogram3[h];
				histogram3[h] = count3;
				count3 += tmp;

				tmp = histogram4[h];
				histogram4[h] = count4;
				count4 += tmp;

				tmp = histogram5[h];
				histogram5[h] = count5;
				count5 += tmp;

				tmp = histogram6[h];
				histogram6[h] = count6;
				count6 += tmp;

				tmp = histogram7[h];
				histogram7[h] = count7;
				count7 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram0[a[i].value & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram1[b[i].value >> 8 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram2[a[i].value >> 16 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram3[b[i].value >> 24 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram4[a[i].value >> 32 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram5[b[i].value >> 40 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram6[a[i].value >> 48 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			vector[histogram7[b[i].value >> 56]++] = old[b[i].index];
		}

		delete [] old;
		delete [] b;
		delete [] a;
	}


	template <typename TYPE>
	void RadixSort(TYPE* vector, int size, float TYPE::* key) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		struct Data
		{
			uint32 value;
			int index;
		};

		register Data* __restrict a = new(DEFAULT_ALIGNMENT) Data[size];
		register Data* __restrict b = new(DEFAULT_ALIGNMENT) Data[size];
		register TYPE* __restrict old = new(DEFAULT_ALIGNMENT) TYPE[size];
		if ((!a) || (!b) || (!old))
		{
			delete [] old;
			delete [] b;
			delete [] a;
			Throw(MemoryException());
		}

		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};

		{
			register char* k = reinterpret_cast<char*>(&(vector[0].*key));
			for (register int i = 0; i < size; ++i, k += sizeof(TYPE))
			{
				old[i] = vector[i];

				register uint32 value = *reinterpret_cast<uint32*>(k);
				value ^= static_cast<uint32>(-sint32(value >> 31)) | 0x80000000; // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
				++histogram0[value & 0xff];
				++histogram1[value >> 8 & 0xff];
				++histogram2[value >> 16 & 0xff];
				++histogram3[value >> 24];

				a[i].value = value;
				a[i].index = i;
			}
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			register int count2 = 0;
			register int count3 = 0;
			for (register sint16 h = 0; h < 256; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;

				tmp = histogram2[h];
				histogram2[h] = count2;
				count2 += tmp;

				tmp = histogram3[h];
				histogram3[h] = count3;
				count3 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram0[a[i].value & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram1[b[i].value >> 8 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram2[a[i].value >> 16 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			vector[histogram3[b[i].value >> 24]++] = old[b[i].index];
		}

		delete [] old;
		delete [] b;
		delete [] a;
	}


	template <typename TYPE>
	void RadixSort(TYPE* vector, int size, double TYPE::* key) throw(MemoryException)
	{
		Assert(size >= 0);

		struct Data
		{
			uint64 value;
			int index;
		};

		register Data* __restrict a = new(DEFAULT_ALIGNMENT) Data[size];
		register Data* __restrict b = new(DEFAULT_ALIGNMENT) Data[size];
		register TYPE* __restrict old = new(DEFAULT_ALIGNMENT) TYPE[size];
		if ((!a) || (!b) || (!old))
		{
			delete [] old;
			delete [] b;
			delete [] a;
			Throw(MemoryException());
		}

		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};
		int histogram4[256] = {0};
		int histogram5[256] = {0};
		int histogram6[256] = {0};
		int histogram7[256] = {0};

		{
			register char* k = reinterpret_cast<char*>(&(vector[0].*key));
			for (register int i = 0; i < size; ++i, k += sizeof(TYPE))
			{
				old[i] = vector[i];

				register uint64 value = *reinterpret_cast<uint64*>(k);
				value ^= static_cast<uint64>(-static_cast<sint64>(value >> 63)) | 0x8000000000000000; // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
				++histogram0[value & 0xff];
				++histogram1[value >> 8 & 0xff];
				++histogram2[value >> 16 & 0xff];
				++histogram3[value >> 24 & 0xff];
				++histogram4[value >> 32 & 0xff];
				++histogram5[value >> 40 & 0xff];
				++histogram6[value >> 48 & 0xff];
				++histogram7[value >> 56];

				a[i].value = value;
				a[i].index = i;
			}
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			register int count2 = 0;
			register int count3 = 0;
			register int count4 = 0;
			register int count5 = 0;
			register int count6 = 0;
			register int count7 = 0;
			for (register sint16 h = 0; h < 256; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;

				tmp = histogram2[h];
				histogram2[h] = count2;
				count2 += tmp;

				tmp = histogram3[h];
				histogram3[h] = count3;
				count3 += tmp;

				tmp = histogram4[h];
				histogram4[h] = count4;
				count4 += tmp;

				tmp = histogram5[h];
				histogram5[h] = count5;
				count5 += tmp;

				tmp = histogram6[h];
				histogram6[h] = count6;
				count6 += tmp;

				tmp = histogram7[h];
				histogram7[h] = count7;
				count7 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram0[a[i].value & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram1[b[i].value >> 8 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram2[a[i].value >> 16 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram3[b[i].value >> 24 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram4[a[i].value >> 32 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			a[histogram5[b[i].value >> 40 & 0xff]++] = b[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			b[histogram6[a[i].value >> 48 & 0xff]++] = a[i];
		}

		for (register int i = 0; i < size; ++i)
		{
			vector[histogram7[b[i].value >> 56]++] = old[b[i].index];
		}

		delete [] old;
		delete [] b;
		delete [] a;
	}


	template <typename TYPE, typename KEY>
	inline void RadixSort(Vector<TYPE>& vector, KEY TYPE::* key) throw(MemoryException)
	{
		RadixSort(vector.entry, vector.size, key);
	}


	template <typename TYPE>
	void Sort(TYPE* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		if (size >= SORT_RADIX_COMB_LIMIT)
		{
			RadixSort(vector, size);
		}
		else if (size > 4)
		{
			CombSort(vector, size);
		}
		else if (size == 4)
		{
			register TYPE tmp;
			register TYPE a = vector[0];
			register TYPE b = vector[1];
			register TYPE c = vector[2];
			register TYPE d = vector[3];

			if (b < a)
			{
				tmp = b;
				b = a;
				a = tmp;
			}
			if (c < b)
			{
				tmp = c;
				c = b;
				b = tmp;
			}
			if (d < c)
			{
				tmp = d;
				d = c;
				c = tmp;
			}
			if (b < a)
			{
				tmp = b;
				b = a;
				a = tmp;
			}
			if (c < b)
			{
				tmp = c;
				c = b;
				b = tmp;
			}
			if (b < a)
			{
				tmp = b;
				b = a;
				a = tmp;
			}
			vector[0] = a;
			vector[1] = b;
			vector[2] = c;
			vector[3] = d;
		}
		else if (size == 3)
		{
			register TYPE a = vector[0];
			register TYPE b = vector[1];
			register TYPE c = vector[2];

			if (a < b)
			{
				if (c < a)
				{
					vector[0] = c;
					vector[1] = a;
					vector[2] = b;
				}
				else if (c < b)
				{
					vector[1] = c;
					vector[2] = b;
				}
			}
			else
			{
				if (c < b)
				{
					vector[0] = c;
					vector[2] = a;
				}
				else if (c < a)
				{
					vector[0] = b;
					vector[1] = c;
					vector[2] = a;
				}
				else
				{
					vector[0] = b;
					vector[1] = a;
				}
			}
		}
		else if (size == 2)
		{
			register TYPE a = vector[0];
			register TYPE b = vector[1];

			if (a > b)
			{
				vector[0] = b;
				vector[1] = a;
			}
		}
	}


	template <typename TYPE>
	inline void Sort(Vector<TYPE>& vector) throw(MemoryException)
	{
		Sort(vector.entry, vector.size);
	}


	template <typename TYPE, typename KEY>
	void Sort(TYPE* vector, int size, KEY TYPE::* key) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		if (size >= SORT_RADIX_COMB_LIMIT)
		{
			RadixSort(vector, size, key);
		}
		else if (size > 4)
		{
			CombSort(vector, size, key);
		}
		else if (size == 4)
		{
			TYPE tmp;
			TYPE a = vector[0];
			TYPE b = vector[1];
			TYPE c = vector[2];
			TYPE d = vector[3];

			if (b.*key < a.*key)
			{
				tmp = b;
				b = a;
				a = tmp;
			}
			if (c.*key < b.*key)
			{
				tmp = c;
				c = b;
				b = tmp;
			}
			if (d.*key < c.*key)
			{
				tmp = d;
				d = c;
				c = tmp;
			}
			if (b.*key < a.*key)
			{
				tmp = b;
				b = a;
				a = tmp;
			}
			if (c.*key < b.*key)
			{
				tmp = c;
				c = b;
				b = tmp;
			}
			if (b.*key < a.*key)
			{
				tmp = b;
				b = a;
				a = tmp;
			}
			vector[0] = a;
			vector[1] = b;
			vector[2] = c;
			vector[3] = d;
		}
		else if (size == 3)
		{
			TYPE a = vector[0];
			TYPE b = vector[1];
			TYPE c = vector[2];

			if (a.*key < b.*key)
			{
				if (c.*key < a.*key)
				{
					vector[0] = c;
					vector[1] = a;
					vector[2] = b;
				}
				else if (c.*key < b.*key)
				{
					vector[1] = c;
					vector[2] = b;
				}
			}
			else
			{
				if (c.*key < b.*key)
				{
					vector[0] = c;
					vector[2] = a;
				}
				else if (c.*key < a.*key)
				{
					vector[0] = b;
					vector[1] = c;
					vector[2] = a;
				}
				else
				{
					vector[0] = b;
					vector[1] = a;
				}
			}
		}
		else if (size == 2)
		{
			TYPE a = vector[0];
			TYPE b = vector[1];

			if (a.*key > b.*key)
			{
				vector[0] = b;
				vector[1] = a;
			}
		}
	}


	template <typename TYPE, typename KEY>
	inline void Sort(Vector<TYPE>& vector, KEY TYPE::* key) throw(MemoryException)
	{
		Sort(vector.entry, vector.size, key);
	}
}
