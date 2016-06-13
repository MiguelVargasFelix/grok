// Font.h
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
	struct Glyph
	{
		unsigned char width;

		unsigned char height;
		
		short offset_x;
		
		short offset_y;
		
		short advance;
		
		unsigned char* data;
	};


	struct Font
	{
		Glyph glyphs[256];

		int ascent;

		int descent;

		int size;


		void TextSize(const char* text, int& width, int& above, int& below) const throw();


		int TextWidth(const char* text) const throw();
	};


	namespace Fonts
	{
		extern Font roboto_8;
		extern Font roboto_8_bold;
		extern Font roboto_8_italic;
		extern Font roboto_8_bold_italic;
		extern Font roboto_10;
		extern Font roboto_10_bold;
		extern Font roboto_10_italic;
		extern Font roboto_10_bold_italic;
		extern Font roboto_12;
		extern Font roboto_12_bold;
		extern Font roboto_12_italic;
		extern Font roboto_12_bold_italic;
		extern Font roboto_14;
		extern Font roboto_14_bold;
		extern Font roboto_14_italic;
		extern Font roboto_14_bold_italic;
		extern Font roboto_18;
		extern Font roboto_18_bold;
		extern Font roboto_18_italic;
		extern Font roboto_18_bold_italic;
		extern Font roboto_24;
		extern Font roboto_24_bold;
		extern Font roboto_24_italic;
		extern Font roboto_24_bold_italic;
	}
}
