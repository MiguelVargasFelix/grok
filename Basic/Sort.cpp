// Sort.cpp
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

#include <Basic/Sort.h>


namespace Grok
{
	void RadixSort(sint8* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint8* __restrict b = new(DEFAULT_ALIGNMENT) uint8[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint8* __restrict a = reinterpret_cast<uint8*>(vector);
		int histogram0[16] = {0};
		int histogram1[16] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint8 value = a[i] ^ 0x80;
			++histogram0[value & 0xf];
			++histogram1[value >> 4];
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			for (register sint8 h = 0; h < 16; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint8 value = a[i] ^ 0x80;
			b[histogram0[value & 0xf]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint8 value = b[i];
			a[histogram1[value >> 4]++] = value ^ 0x80;
		}

		delete [] b;
	}


	void RadixSort(uint8* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint8* __restrict b = new(DEFAULT_ALIGNMENT) uint8[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint8* __restrict a = vector;
		int histogram0[16] = {0};
		int histogram1[16] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint8 value = a[i];
			++histogram0[value & 0xf];
			++histogram1[value >> 4];
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			for (register sint8 h = 0; h < 16; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint8 value = a[i];
			b[histogram0[value & 0xf]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint8 value = b[i];
			a[histogram1[value >> 4]++] = value;
		}

		delete [] b;
	}


	void RadixSort(uint16* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint16* __restrict b = new(DEFAULT_ALIGNMENT) uint16[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint16* __restrict a = vector;
		int histogram0[256] = {0};
		int histogram1[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint16 value = a[i];
			++histogram0[value & 0xff];
			++histogram1[value >> 8];
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			for (register sint16 h = 0; h < 256; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint16 value = a[i];
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint16 value = b[i];
			a[histogram1[value >> 8]++] = value;
		}

		delete [] b;
	}


	void RadixSort(sint16* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint16* __restrict b = new(DEFAULT_ALIGNMENT) uint16[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint16* __restrict a = reinterpret_cast<uint16*>(vector);
		int histogram0[256] = {0};
		int histogram1[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint16 value = a[i] ^ 0x8000;
			++histogram0[value & 0xff];
			++histogram1[value >> 8];
		}

		{
			register int count0 = 0;
			register int count1 = 0;
			for (register sint16 h = 0; h < 256; ++h)
			{
				register int tmp;
				
				tmp = histogram0[h];
				histogram0[h] = count0;
				count0 += tmp;

				tmp = histogram1[h];
				histogram1[h] = count1;
				count1 += tmp;
			}
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint16 value = a[i] ^ 0x8000;
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint16 value = b[i];
			a[histogram1[value >> 8]++] = value ^ 0x8000;
		}

		delete [] b;
	}


	void RadixSort(uint32* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint32* __restrict b = new(DEFAULT_ALIGNMENT) uint32[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint32* __restrict a = vector;
		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = a[i];
			++histogram0[value & 0xff];
			++histogram1[value >> 8 & 0xff];
			++histogram2[value >> 16 & 0xff];
			++histogram3[value >> 24];
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
			register uint32 value = a[i];
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = b[i];
			a[histogram1[value >> 8 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = a[i];
			b[histogram2[value >> 16 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = b[i];
			a[histogram3[value >> 24]++] = value;
		}

		delete [] b;
	}


	void RadixSort(sint32* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint32* __restrict b = new(DEFAULT_ALIGNMENT) uint32[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint32* __restrict a = reinterpret_cast<uint32*>(vector);
		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = a[i] ^ 0x80000000;
			++histogram0[value & 0xff];
			++histogram1[value >> 8 & 0xff];
			++histogram2[value >> 16 & 0xff];
			++histogram3[value >> 24];
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
			register uint32 value = a[i] ^ 0x80000000;
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = b[i];
			a[histogram1[value >> 8 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = a[i];
			b[histogram2[value >> 16 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = b[i];
			a[histogram3[value >> 24]++] = value ^ 0x80000000;
		}

		delete [] b;
	}


	void RadixSort(uint64* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint64* __restrict b = new(DEFAULT_ALIGNMENT) uint64[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint64* __restrict a = vector;
		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};
		int histogram4[256] = {0};
		int histogram5[256] = {0};
		int histogram6[256] = {0};
		int histogram7[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			++histogram0[value & 0xff];
			++histogram1[value >> 8 & 0xff];
			++histogram2[value >> 16 & 0xff];
			++histogram3[value >> 24 & 0xff];
			++histogram4[value >> 32 & 0xff];
			++histogram5[value >> 40 & 0xff];
			++histogram6[value >> 48 & 0xff];
			++histogram7[value >> 56];
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
			register uint64 value = a[i];
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram1[value >> 8 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram2[value >> 16 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram3[value >> 24 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram4[value >> 32 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram5[value >> 40 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram6[value >> 48 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram7[value >> 56]++] = value;
		}

		delete [] b;
	}


	void RadixSort(sint64* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint64* __restrict b = new(DEFAULT_ALIGNMENT) uint64[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint64* __restrict a = reinterpret_cast<uint64*>(vector);
		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};
		int histogram4[256] = {0};
		int histogram5[256] = {0};
		int histogram6[256] = {0};
		int histogram7[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i] ^ 0x8000000000000000;
			++histogram0[value & 0xff];
			++histogram1[value >> 8 & 0xff];
			++histogram2[value >> 16 & 0xff];
			++histogram3[value >> 24 & 0xff];
			++histogram4[value >> 32 & 0xff];
			++histogram5[value >> 40 & 0xff];
			++histogram6[value >> 48 & 0xff];
			++histogram7[value >> 56];
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
			register uint64 value = a[i] ^ 0x8000000000000000;
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram1[value >> 8 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram2[value >> 16 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram3[value >> 24 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram4[value >> 32 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram5[value >> 40 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram6[value >> 48 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram7[value >> 56]++] = value ^ 0x8000000000000000;
		}

		delete [] b;
	}


	void RadixSort(float* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint32* __restrict b = new(DEFAULT_ALIGNMENT) uint32[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint32* __restrict a = reinterpret_cast<uint32*>(vector);
		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = a[i];
			value ^= static_cast<uint32>(-sint32(value >> 31)) | 0x80000000; // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
			++histogram0[value & 0xff];
			++histogram1[value >> 8 & 0xff];
			++histogram2[value >> 16 & 0xff];
			++histogram3[value >> 24];
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
			register uint32 value = a[i];
			value ^= static_cast<uint32>(-sint32(value >> 31)) | 0x80000000; // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = b[i];
			a[histogram1[value >> 8 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = a[i];
			b[histogram2[value >> 16 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint32 value = b[i];
			a[histogram3[value >> 24]++] = value ^ (((value >> 31) - 1) | 0x80000000); // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
		}

		delete [] b;
	}


	void RadixSort(double* vector, int size) throw(MemoryException)
	{
		Assert(vector);
		Assert(size >= 0);

		register uint64* __restrict b = new(DEFAULT_ALIGNMENT) uint64[size];
		if (!b)
		{
			Throw(MemoryException());
		}

		register uint64* __restrict a = reinterpret_cast<uint64*>(vector);
		int histogram0[256] = {0};
		int histogram1[256] = {0};
		int histogram2[256] = {0};
		int histogram3[256] = {0};
		int histogram4[256] = {0};
		int histogram5[256] = {0};
		int histogram6[256] = {0};
		int histogram7[256] = {0};

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			value ^= static_cast<uint64>(-sint64(value >> 63)) | 0x8000000000000000; // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
			++histogram0[value & 0xff];
			++histogram1[value >> 8 & 0xff];
			++histogram2[value >> 16 & 0xff];
			++histogram3[value >> 24 & 0xff];
			++histogram4[value >> 32 & 0xff];
			++histogram5[value >> 40 & 0xff];
			++histogram6[value >> 48 & 0xff];
			++histogram7[value >> 56];
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
			register uint64 value = a[i];
			value ^= static_cast<uint64>(-sint64(value >> 63)) | 0x8000000000000000; // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
			b[histogram0[value & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram1[value >> 8 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram2[value >> 16 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram3[value >> 24 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram4[value >> 32 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram5[value >> 40 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = a[i];
			b[histogram6[value >> 48 & 0xff]++] = value;
		}

		for (register int i = 0; i < size; ++i)
		{
			register uint64 value = b[i];
			a[histogram7[value >> 56]++] = value ^ (((value >> 63) - 1) | 0x8000000000000000); // Michael Herf. Radix Tricks. 2001. http://stereopsis.com/radix.html
		}

		delete [] b;
	}
}
