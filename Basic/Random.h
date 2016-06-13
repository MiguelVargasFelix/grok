// Random.h
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

#include <Basic/Integer.h>


namespace Grok
{
	namespace RandomGenerator
	{
		enum ID
		{
			linear_congruential, // http://en.wikipedia.org/wiki/Linear_congruential_generator
			mersenne_twister,    // Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura, freely usable http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
			mother_of_all,       // Copyright (C) 1997 - 2007, Agner Fog, GNU General Public License http://www.agner.org/random
			lecuyer              // P. L'Ecuyer. Maximally Equidistributed Combined Tausworthe Generators. Mathematics of Computation, Vol. 65, pp. 203–213. 1996.
		};
	}


	class Random
	{
		public:

			static uint32 maximum;

			RandomGenerator::ID random_generator;


			Random(uint32 seed = 1, RandomGenerator::ID random_generator = RandomGenerator::linear_congruential) throw();


			inline uint32 Get() throw()
			{
				return (this->*get_function)();
			}


		protected:

			uint32 (Random::* get_function)() throw();


			uint32 LinearCongruentialGet() throw();


			uint32 MersenneTwisterGet() throw();


			uint32 MotherOfAllGet() throw();


			uint32 MotherOfAllInit(uint32 seed) throw();


			uint32 LecuyerGet() throw();

			
			uint32 LecuyerInit(uint32 seed) throw();


			union
			{
				struct
				{
					uint32 last;
				} linear_congruential_state;


				struct
				{
					int index;
					uint32 x[624];
				} mersenne_twister_state;


				struct
				{
					uint32 x0;
					uint32 x1;
					uint32 x2;
					uint32 x3;
					uint32 x4;
				} mother_of_all_state;

				
				struct
				{
					uint32 s1;
					uint32 s2;
					uint32 s3;
				} lecuyer_state;
			} data;
	};
}
