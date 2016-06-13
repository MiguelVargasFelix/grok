// Font.cpp
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

#include <Basic/Assert.h>
#include <Image/Font.h>


namespace Grok
{
	void Font::TextSize(const char* text, int& width, int& above, int& below) const throw()
	{
		Assert(text);

		width = 0;
		above = 0;
		below = 0;
		for (register const char* __restrict character = text; *character; ++character)
		{
			unsigned char code = static_cast<unsigned char>(*character);
			const Glyph& glyph = glyphs[code];
			width += *(character + 1) ? glyph.advance : (glyph.width != 0) ? glyph.width : glyph.advance;
			if (above < glyph.height + glyph.offset_y)
			{
				above = glyph.height + glyph.offset_y;
			}
			if (below > glyph.offset_y)
			{
				below = glyph.offset_y;
			}
		}
		below = -below;
	}


	int Font::TextWidth(const char* text) const throw()
	{
		Assert(text);

		int width = 0;
		for (register const char* __restrict character = text; *character; ++character)
		{
			unsigned char code = static_cast<unsigned char>(*character);
			const Glyph& glyph = glyphs[code];
			width += *(character + 1) ? glyph.advance : (glyph.width != 0) ? glyph.width : glyph.advance;
		}
		return width;
	}
}
