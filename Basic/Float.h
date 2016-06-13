// Float.h
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

#include <Basic/Format.h>


namespace Grok
{
	template <typename TYPE>
	struct Float
	{
		static const int size;

		static const TYPE maximum;

		static const TYPE minimum;

		static const TYPE epsilon;

		static const TYPE infinity;

		static bool IsNaN(TYPE value) throw();
	};


	namespace FloatNotation
	{
		enum ID
		{
			fixed,
			exponential,
			automatic
		};
	}


	struct FloatFormat : public Format
	{
		FloatFormat(bool space, bool sign, int width, int precision, FloatNotation::ID notation, bool is_long = false) throw();
	};
}
