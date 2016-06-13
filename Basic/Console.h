// Console.h
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

namespace Grok
{
	namespace ConsoleColor
	{
		enum ID
		{
			white    = 0,
			red      = 1,
			green    = 2,
			blue     = 3,
			cyan     = 4,
			magenta  = 5,
			yellow   = 6,
			black    = 7,
			standard = 8
		};
	}


	struct Console
	{
		static void SetForeground(ConsoleColor::ID color, bool bold = false) throw();


		static void SetBackground(ConsoleColor::ID color) throw();
	};
}
