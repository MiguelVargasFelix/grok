// Image.h
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

#include <Basic/File.h>
#include <Basic/Memory.h>

#ifndef DEFAULT_IMAGE_ALIGN
	#define DEFAULT_IMAGE_ALIGN 4
#endif


namespace Grok
{
	struct Box;

	struct Color;

	struct Font;


	class Image
	{
		public:

			Color** __restrict pixel;

			int width;

			int height;

			int align;

			bool top_to_bottom;


			Image() throw(MemoryException);


			Image(const Image& image) throw(MemoryException);


			Image(int width, int height, bool top_to_bottom = true, int align = DEFAULT_IMAGE_ALIGN) throw(MemoryException);


			~Image() throw();


			inline unsigned char* Data() const throw()
			{
				return reinterpret_cast<unsigned char*>(pixel[top_to_bottom ? 0 : height - 1]);
			}


			void DrawLine(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha) throw();


			void DrawLine(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha, const Box& clip_box) throw();


			int DrawText(const char* text, int x, int y, const Font& font, const Color& color) throw();


			int DrawText(const char* text, int x, int y, const Font& font, const Color& color, const Box& clip_box) throw();


			void DrawCircle(int x, int y, int radius, const Color& color, unsigned char alpha) throw();


			void DrawRectangle(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha) throw();


			void DrawRectangle(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha, const Box& clip_box) throw();


			void LoadPPM(const char* file_name, bool top_to_bottom = true, int align = DEFAULT_IMAGE_ALIGN) throw(MemoryException, FileException);


			void Resize(int width, int height, bool top_to_bottom = true, int align = DEFAULT_IMAGE_ALIGN) throw(MemoryException);


			void ResizeToFit(int width, int height, bool top_to_bottom = true, int align = DEFAULT_IMAGE_ALIGN) throw(MemoryException);


			void SavePPM(const char* file_name) throw(FileException);
	};
}
