// Random.cpp
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

#include <Basic/Random.h>
#include <Basic/Time.h>


namespace Grok
{
	uint32 Random::maximum = static_cast<uint32>(4294967295U);


	Random::Random(uint32 seed, RandomGenerator::ID random_generator) throw()
	:	random_generator(random_generator)
	{
		if (seed == 0)
		{
			Time time;
			time.UseCurrentTime();
			seed = static_cast<uint32>(time.milliseconds);
		}

		switch (random_generator)
		{
			case RandomGenerator::linear_congruential:
			{
				get_function = &Random::LinearCongruentialGet;
				data.linear_congruential_state.last = seed;
				break;
			}
			case RandomGenerator::mersenne_twister:
			{
				get_function = &Random::MersenneTwisterGet;
				data.mersenne_twister_state.index = 624;
				data.mersenne_twister_state.x[0] = seed;
				register uint32* element = data.mersenne_twister_state.x;
				for (register int i = 1; i < 624; ++i)
				{
					uint32 e = static_cast<uint32>(1812433253UL)*(*element ^ (*element >> 30)) + static_cast<uint32>(i);
					*(++element) = e;
				}
				break;
			}
			case RandomGenerator::mother_of_all:
			{
				get_function = &Random::MotherOfAllGet;
				data.mother_of_all_state.x0 = MotherOfAllInit(seed);
				data.mother_of_all_state.x1 = MotherOfAllInit(data.mother_of_all_state.x0);
				data.mother_of_all_state.x2 = MotherOfAllInit(data.mother_of_all_state.x1);
				data.mother_of_all_state.x3 = MotherOfAllInit(data.mother_of_all_state.x2);
				data.mother_of_all_state.x4 = MotherOfAllInit(data.mother_of_all_state.x3);
				for (int i = 19; i; --i)
				{
					MotherOfAllGet();
				}
				break;
			}
			case RandomGenerator::lecuyer:
			{
				get_function = &Random::LecuyerGet;
				data.lecuyer_state.s1 = LecuyerInit(seed);
				data.lecuyer_state.s2 = LecuyerInit(data.lecuyer_state.s1);
				data.lecuyer_state.s3 = LecuyerInit(data.lecuyer_state.s2);
				for (int i = 19; i; --i)
				{
					LecuyerGet();
				}
				break;
			}
		}
	}


	uint32 Random::LinearCongruentialGet() throw()
	{
		return (data.linear_congruential_state.last = static_cast<uint32>(1103515245UL)*data.linear_congruential_state.last + static_cast<uint32>(12345UL));
	}


	uint32 Random::MersenneTwisterGet() throw()
	{
		static const uint32 a[2] = {0UL, static_cast<uint32>(2567483615UL)};

		register uint32 y;
		if (data.mersenne_twister_state.index == 624)
		{
			for (register int k = 0; k < 227; ++k)
			{
				y = (data.mersenne_twister_state.x[k] & static_cast<uint32>(0x80000000UL)) | (data.mersenne_twister_state.x[k + 1] & static_cast<uint32>(0x7fffffffUL));
				data.mersenne_twister_state.x[k] = data.mersenne_twister_state.x[k + 397] ^ (y >> 1) ^ a[y & 1UL];
			}
			for (register int k = 227; k < 623; ++k)
			{
				y = (data.mersenne_twister_state.x[k] & static_cast<uint32>(0x80000000UL)) | (data.mersenne_twister_state.x[k + 1] & static_cast<uint32>(0x7fffffffUL));
				data.mersenne_twister_state.x[k] = data.mersenne_twister_state.x[k - 227] ^ (y >> 1) ^ a[y & 1UL];
			}
			y = (data.mersenne_twister_state.x[623] & static_cast<uint32>(0x80000000UL)) | (data.mersenne_twister_state.x[0] & static_cast<uint32>(0x7fffffffUL));
			data.mersenne_twister_state.x[623] = data.mersenne_twister_state.x[396] ^ (y >> 1) ^ a[y & 1UL];
			data.mersenne_twister_state.index = 0;
		}
		y = data.mersenne_twister_state.x[data.mersenne_twister_state.index++];
		y ^= (y >> 11);
		y ^= (y << 7) & static_cast<uint32>(2636928640UL);
		y ^= (y << 15) & static_cast<uint32>(4022730752UL);
		y ^= (y >> 18);
		return y;
	}


	uint32 Random::MotherOfAllGet() throw()
	{
		register uint64 sum =
			static_cast<uint64>(2111111111UL)*static_cast<uint64>(data.mother_of_all_state.x3) +
			static_cast<uint64>(1492UL)*static_cast<uint64>(data.mother_of_all_state.x2) +
			static_cast<uint64>(1776UL)*static_cast<uint64>(data.mother_of_all_state.x1) +
			static_cast<uint64>(5115UL)*static_cast<uint64>(data.mother_of_all_state.x0) +
			static_cast<uint64>(data.mother_of_all_state.x4);
		data.mother_of_all_state.x3 = data.mother_of_all_state.x2;
		data.mother_of_all_state.x2 = data.mother_of_all_state.x1;
		data.mother_of_all_state.x1 = data.mother_of_all_state.x0;
		data.mother_of_all_state.x4 = static_cast<uint32>(sum >> 32);
		data.mother_of_all_state.x0 = static_cast<uint32>(sum);
		return data.mother_of_all_state.x0;
	}


	uint32 Random::MotherOfAllInit(uint32 seed) throw()
	{
		return seed*static_cast<uint32>(29943829UL) - static_cast<uint32>(1UL);
	}


	uint32 Random::LecuyerGet() throw()
	{
		register uint32 b;
		b = ((data.lecuyer_state.s1 << 13) ^ data.lecuyer_state.s1) >> 19;
		data.lecuyer_state.s1 = ((data.lecuyer_state.s1 & static_cast<uint32>(4294967294UL)) << 12) ^ b;
		b = ((data.lecuyer_state.s2 << 2) ^ data.lecuyer_state.s2) >> 25;
		data.lecuyer_state.s2 = ((data.lecuyer_state.s2 & static_cast<uint32>(4294967288UL)) << 4) ^ b;
		b = ((data.lecuyer_state.s3 << 3) ^ data.lecuyer_state.s3) >> 11;
		data.lecuyer_state.s3 = ((data.lecuyer_state.s3 & static_cast<uint32>(4294967280UL)) << 17) ^ b;
		return data.lecuyer_state.s1 ^ data.lecuyer_state.s2 ^ data.lecuyer_state.s3;
	}

			
	uint32 Random::LecuyerInit(uint32 seed) throw()
	{
		return seed*static_cast<uint32>(29943829UL) - static_cast<uint32>(1UL);
	}
}
