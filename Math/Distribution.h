// Distribution.h
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

#include <Basic/Random.h>
#include <math.h>


namespace Grok
{
	template <typename TYPE = double>
	class Uniform
	{
		public:

			TYPE minimum;

			TYPE maximum;


			Uniform(TYPE minimum = 0, TYPE maximum = 1, uint32 seed = 1, RandomGenerator::ID random_generator = RandomGenerator::linear_congruential) throw()
			:	minimum(minimum),
				maximum(maximum),
				random(seed, random_generator),
				factor((static_cast<double>(maximum) - static_cast<double>(minimum))/(static_cast<double>(random.maximum) + 1.0))
			{
			}


			TYPE Get() throw()
			{
				return minimum + static_cast<TYPE>(factor*random.Get());
			}


		protected:

			Random random;

			double factor;
	};


	// http://en.wikipedia.org/wiki/Box_Muller_transform
	template <typename TYPE = double>
	class Normal
	{
		public:

			TYPE mean;

			TYPE variance;


			Normal(TYPE mean = 0, TYPE variance = 1, uint32 seed = 1, RandomGenerator::ID random_generator = RandomGenerator::linear_congruential) throw()
			:	mean(mean),
				variance(variance),
				random1(seed, random_generator),
				random2(seed + 1, random_generator),
				factor1(1.0/(static_cast<double>(random1.maximum) + 1.0)),
				factor2(6.283185307179586476925286766559L/static_cast<double>(random2.maximum))
			{
			}


			TYPE Get() throw()
			{
				return static_cast<TYPE>(sqrt(-2.0*variance*log(factor1*(static_cast<double>(random1.Get()) + 1.0)))*cos(factor2*static_cast<double>(random2.Get())) + mean);
			}


		protected:

			Random random1;

			Random random2;
			
			double factor1;
			
			double factor2;
	};


	// http://en.wikipedia.org/wiki/Exponential_distribution
	template <typename TYPE = double>
	class Exponential
	{
		public:

			TYPE rate;


			Exponential(TYPE rate = 1, uint32 seed = 1, RandomGenerator::ID random_generator = RandomGenerator::linear_congruential)
			:	rate(rate),
				random(seed, random_generator),
				factor1(1.0/(static_cast<double>(random.maximum) + 2.0)),
				factor2(-1.0/static_cast<double>(rate))
			{
			}


			TYPE Get()
			{
				return static_cast<TYPE>(log(factor1*(static_cast<double>(random.Get()) + 1.0))*factor2);
			}


		protected:

			Random random;

			double factor1;
			
			double factor2;
	};
}
