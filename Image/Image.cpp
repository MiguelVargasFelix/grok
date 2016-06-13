// Image.cpp
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
#include <Basic/Integer.h>
#include <Basic/System.h>
#include <Image/Box.h>
#include <Image/Color.h>
#include <Image/Font.h>
#include <Image/Image.h>

#include <math.h>


namespace Grok
{
	Image::Image() throw(MemoryException)
	:	pixel(static_cast<Color**>(0)),
		width(0),
		height(0),
		align(0),
		top_to_bottom(false)
	{
	}


	Image::Image(const Image& image) throw(MemoryException)
	:	pixel(static_cast<Color**>(0))
	{
		Resize(image.width, image.height, image.top_to_bottom, image.align);

		for (int y = 0; y < height; ++y)
		{
			register Color* __restrict src_pixel_y = pixel[y];
			register Color* __restrict dst_pixel_y = image.pixel[y];
			for (register int x = 0; x < width; ++x)
			{
				dst_pixel_y[x] = src_pixel_y[x];
			}
		}
	}


	Image::Image(int width, int height, bool top_to_bottom, int align) throw(MemoryException)
	:	pixel(static_cast<Color**>(0))
	{
		Resize(width, height, top_to_bottom, align);
	}


	Image::~Image() throw()
	{
		if (pixel)
		{
			delete [] pixel[top_to_bottom ? 0 : height - 1];
			delete [] pixel;
		}
	}


	void Image::DrawLine(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha) throw()
	{
		int x_max = width - 1;
		int y_max = height - 1;

		int dx = x2 - x1;
		int dy = y2 - y1;
		int d;
		{
			int sx = x2 > x1 ? dx : -dx;
			int sy = y2 > y1 ? dy : -dy;
			d = sx > sy ? sx : sy;
		}

		if (d)
		{
			int a;
			int b;

			if (dx == 0) // Vertical line
			{
				if ((x1 > x_max) || (x1 < 0))
				{
					return;
				}

				if (y2 > y1)
				{
					if ((y1 > y_max) || (y2 < 0))
					{
						return;
					}

					a = (y1 < 0) ? -y1 : 0;
					b = (y2 > y_max) ? y_max - y1 : d;
				}
				else
				{
					if ((y1 < 0) || (y2 > y_max))
					{
						return;
					}

					a = (y2 < 0) ? -y1 : -d;
					b = (y1 > y_max) ? y_max - y1 : 0;
				}
				if (alpha == 255)
				{
					for (register int i = a; i <= b; ++i)
					{
						register int y = y1 + i;

						pixel[y][x1].blue = color.blue;
						pixel[y][x1].green = color.green;
						pixel[y][x1].red = color.red;
					}
				}
				else
				{
					uint16 alpha_blue = alpha*color.blue;
					uint16 alpha_green = alpha*color.green;
					uint16 alpha_red = alpha*color.red;
					uint16 beta = 255 - alpha;
					for (register int i = a; i <= b; ++i)
					{
						register int y = y1 + i;

						pixel[y][x1].blue = static_cast<unsigned char>((alpha_blue + beta*pixel[y][x1].blue)/255);
						pixel[y][x1].green = static_cast<unsigned char>((alpha_green + beta*pixel[y][x1].green)/255);
						pixel[y][x1].red = static_cast<unsigned char>((alpha_red + beta*pixel[y][x1].red)/255);
					}
				}

				return;
			}
			
			if (dy == 0) // Horizontal line
			{
				if ((y1 > y_max) || (y1 < 0))
				{
					return;
				}

				if (x2 > x1)
				{
					if ((x1 > x_max) || (x2 < 0))
					{
						return;
					}

					a = (x1 < 0) ? -x1 : 0;
					b = (x2 > x_max) ? x_max - x1 : d;
				}
				else
				{
					if ((x1 < 0) || (x2 > x_max))
					{
						return;
					}

					a = (x2 < 0) ? -x1 : -d;
					b = (x1 > x_max) ? x_max - x1 : 0;
				}
				if (alpha == 255)
				{
					for (register int i = a; i <= b; ++i)
					{
						register int x = x1 + i;

						pixel[y1][x].blue = color.blue;
						pixel[y1][x].green = color.green;
						pixel[y1][x].red = color.red;
					}
				}
				else
				{
					uint16 alpha_blue = alpha*color.blue;
					uint16 alpha_green = alpha*color.green;
					uint16 alpha_red = alpha*color.red;
					uint16 beta = 255 - alpha;
					for (register int i = a; i <= b; ++i)
					{
						register int x = x1 + i;

						pixel[y1][x].blue = static_cast<unsigned char>((alpha_blue + beta*pixel[y1][x].blue)/255);
						pixel[y1][x].green = static_cast<unsigned char>((alpha_green + beta*pixel[y1][x].green)/255);
						pixel[y1][x].red = static_cast<unsigned char>((alpha_red + beta*pixel[y1][x].red)/255);
					}
				}

				return;
			}

			{
				int ax = 0;
				int bx = d;
				if (x2 > x1)
				{
					if ((x1 > x_max) || (x2 < 0))
					{
						return;
					}
					if (x1 < 0)
					{
						ax = -(x1*d)/dx;
						if (x1 + (ax*dx)/d < 0)
						{
							++ax;
						}
					}
					if (x2 > x_max)
					{
						bx = ((x_max - x1)*d)/dx;
						if (x1 + (bx*dx)/d < x_max)
						{
							++bx;
						}
					}
				}
				else
				{
					if ((x1 < 0) || (x2 > x_max))
					{
						return;
					}
					if (x1 > x_max)
					{
						ax = ((x_max - x1)*d)/dx;
						if (x1 + (ax*dx)/d > x_max)
						{
							++ax;
						}
					}
					if (x2 < 0)
					{
						bx = -(x1*d)/dx;
						if (x1 + (bx*dx)/d > 0)
						{
							++bx;
						}
					}
				}
				
				int ay = 0;
				int by = d;
				if (y2 > y1)
				{
					if ((y1 > y_max) || (y2 < 0))
					{
						return;
					}
					if (y1 < 0)
					{
						ay = -(y1*d)/dy;
						if (y1 + (ay*dy)/d < 0)
						{
							++ay;
						}
					}
					if (y2 > y_max)
					{
						by = ((y_max - y1)*d)/dy;
						if (y1 + (by*dy)/d < y_max)
						{
							++by;
						}
					}
				}
				else
				{
					if ((y1 < 0) || (y2 > y_max))
					{
						return;
					}
					if (y1 > y_max)
					{
						ay = ((y_max - y1)*d)/dy;
						if (y1 + (ay*dy)/d > y_max)
						{
							++ay;
						}
					}
					if (y2 < 0)
					{
						by = -(y1*d)/dy;
						if (y1 + (by*dy)/d > 0)
						{
							++by;
						}
					}
				}

				a = ax > ay ? ax : ay;
				b = bx < by ? bx : by;
			}

			if (alpha == 255)
			{
				for (register int i = a; i <= b; ++i)
				{
					register int x = x1 + (i*dx)/d;
					register int y = y1 + (i*dy)/d;

					pixel[y][x].blue = color.blue;
					pixel[y][x].green = color.green;
					pixel[y][x].red = color.red;
				}
			}
			else
			{
				uint16 alpha_blue = alpha*color.blue;
				uint16 alpha_green = alpha*color.green;
				uint16 alpha_red = alpha*color.red;
				uint16 beta = 255 - alpha;
				for (register int i = a; i <= b; ++i)
				{
					register int x = x1 + (i*dx)/d;
					register int y = y1 + (i*dy)/d;

					pixel[y][x].blue = static_cast<unsigned char>((alpha_blue + beta*pixel[y][x].blue)/255);
					pixel[y][x].green = static_cast<unsigned char>((alpha_green + beta*pixel[y][x].green)/255);
					pixel[y][x].red = static_cast<unsigned char>((alpha_red + beta*pixel[y][x].red)/255);
				}
			}
		}
		else // Single point
		{
			if ((x1 > x_max) || (x1 < 0))
			{
				return;
			}
			if ((y1 > y_max) || (y1 < 0))
			{
				return;
			}

			if (alpha == 255)
			{
				pixel[y1][x1].blue = color.blue;
				pixel[y1][x1].green = color.green;
				pixel[y1][x1].red = color.red;
			}
			else
			{
				uint16 beta = 255 - alpha;
				pixel[y1][x1].blue = static_cast<unsigned char>((alpha*color.blue + beta*pixel[y1][x1].blue)/255);
				pixel[y1][x1].green = static_cast<unsigned char>((alpha*color.green + beta*pixel[y1][x1].green)/255);
				pixel[y1][x1].red = static_cast<unsigned char>((alpha*color.red + beta*pixel[y1][x1].red)/255);
			}
		}
	}


	void Image::DrawLine(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha, const Box& clip_box) throw()
	{
		int x_max;
		int x_min;
		if (clip_box.x2 > clip_box.x1)
		{
			x_max = clip_box.x2;
			x_min = clip_box.x1;
		}
		else
		{
			x_max = clip_box.x1;
			x_min = clip_box.x2;
		}
		if ((x_min >= width) || (x_max < 0))
		{
			return;
		}
		if (x_max >= width)
		{
			x_max = width - 1;
		}
		if (x_min < 0)
		{
			x_min = 0;
		}

		int y_max;
		int y_min;
		if (clip_box.y2 > clip_box.y1)
		{
			y_max = clip_box.y2;
			y_min = clip_box.y1;
		}
		else
		{
			y_max = clip_box.y1;
			y_min = clip_box.y2;
		}
		if ((y_min >= height) || (y_max < 0))
		{
			return;
		}
		if (y_max >= height)
		{
			y_max = height - 1;
		}
		if (y_min < 0)
		{
			y_min = 0;
		}

		int dx = x2 - x1;
		int dy = y2 - y1;
		int d;
		{
			int sx = x2 > x1 ? dx : -dx;
			int sy = y2 > y1 ? dy : -dy;
			d = sx > sy ? sx : sy;
		}

		if (d)
		{
			int a;
			int b;

			if (dx == 0) // Vertical line
			{
				if ((x1 > x_max) || (x2 < x_min))
				{
					return;
				}

				if (y2 > y1)
				{
					if ((y1 > y_max) || (y2 < y_min))
					{
						return;
					}

					a = (y1 < y_min) ? y_min - y1 : 0;
					b = (y2 > y_max) ? y_max - y1 : d;
				}
				else
				{
					if ((y1 < y_min) || (y2 > y_max))
					{
						return;
					}

					a = (y2 < y_min) ? y_min - y1 : -d;
					b = (y1 > y_max) ? y_max - y1 : 0;
				}

				if (alpha == 255)
				{
					for (register int i = a; i <= b; ++i)
					{
						register int y = y1 + i;

						pixel[y][x1].blue = color.blue;
						pixel[y][x1].green = color.green;
						pixel[y][x1].red = color.red;
					}
				}
				else
				{
					uint16 alpha_blue = alpha*color.blue;
					uint16 alpha_green = alpha*color.green;
					uint16 alpha_red = alpha*color.red;
					uint16 beta = 255 - alpha;
					for (register int i = a; i <= b; ++i)
					{
						register int y = y1 + i;

						pixel[y][x1].blue = static_cast<unsigned char>((alpha_blue + beta*pixel[y][x1].blue)/255);
						pixel[y][x1].green = static_cast<unsigned char>((alpha_green + beta*pixel[y][x1].green)/255);
						pixel[y][x1].red = static_cast<unsigned char>((alpha_red + beta*pixel[y][x1].red)/255);
					}
				}

				return;
			}

			if (dy == 0) // Horizontal line
			{
				if ((y1 > y_max) || (y2 < y_min))
				{
					return;
				}

				if (x2 > x1)
				{
					if ((x1 > x_max) || (x2 < x_min))
					{
						return;
					}
					
					a = (x1 < x_min) ? x_min - x1 : 0;
					b = (x2 > x_max) ? x_max - x1 : d;
				}
				else
				{
					if ((x1 < x_min) || (x2 > x_max))
					{
						return;
					}
					
					a = (x2 < x_min) ? x_min - x1 : -d;
					b = (x1 > x_max) ? x_max - x1 : 0;
				}
				
				if (alpha == 255)
				{
					for (register int i = a; i <= b; ++i)
					{
						register int x = x1 + i;

						pixel[y1][x].blue = color.blue;
						pixel[y1][x].green = color.green;
						pixel[y1][x].red = color.red;
					}
				}
				else
				{
					uint16 alpha_blue = alpha*color.blue;
					uint16 alpha_green = alpha*color.green;
					uint16 alpha_red = alpha*color.red;
					uint16 beta = 255 - alpha;
					for (register int i = a; i <= b; ++i)
					{
						register int x = x1 + i;

						pixel[y1][x].blue = static_cast<unsigned char>((alpha_blue + beta*pixel[y1][x].blue)/255);
						pixel[y1][x].green = static_cast<unsigned char>((alpha_green + beta*pixel[y1][x].green)/255);
						pixel[y1][x].red = static_cast<unsigned char>((alpha_red + beta*pixel[y1][x].red)/255);
					}
				}

				return;
			}

			{
				int ax = 0;
				int bx = d;
				if (x2 > x1)
				{
					if ((x1 > x_max) || (x2 < x_min))
					{
						return;
					}
					if (x1 < x_min)
					{
						ax = ((x_min - x1)*d)/dx;
						if (x1 + (ax*dx)/d < x_min)
						{
							++ax;
						}
					}
					if (x2 > x_max)
					{
						bx = ((x_max - x1)*d)/dx;
						if (x1 + (bx*dx)/d < x_max)
						{
							++bx;
						}
					}
				}
				else
				{
					if ((x1 < x_min) || (x2 > x_max))
					{
						return;
					}
					if (x1 > x_max)
					{
						ax = ((x_max - x1)*d)/dx;
						if (x1 + (ax*dx)/d > x_max)
						{
							++ax;
						}
					}
					if (x2 < x_min)
					{
						bx = ((x_min - x1)*d)/dx;
						if (x1 + (bx*dx)/d > x_min)
						{
							++bx;
						}
					}
				}
				
				int ay = 0;
				int by = d;
				if (y2 > y1)
				{
					if ((y1 > y_max) || (y2 < y_min))
					{
						return;
					}
					if (y1 < y_min)
					{
						ay = ((y_min - y1)*d)/dy;
						if (y1 + (ay*dy)/d < y_min)
						{
							++ay;
						}
					}
					if (y2 > y_max)
					{
						by = ((y_max - y1)*d)/dy;
						if (y1 + (by*dy)/d < y_max)
						{
							++by;
						}
					}
				}
				else
				{
					if ((y1 < y_min) || (y2 > y_max))
					{
						return;
					}
					if (y1 > y_max)
					{
						ay = ((y_max - y1)*d)/dy;
						if (y1 + (ay*dy)/d > y_max)
						{
							++ay;
						}
					}
					if (y2 < y_min)
					{
						by = ((y_min - y1)*d)/dy;
						if (y1 + (by*dy)/d > y_min)
						{
							++by;
						}
					}
				}

				a = ax > ay ? ax : ay;
				b = bx < by ? bx : by;
			}

			if (alpha == 255)
			{
				for (register int i = a; i <= b; ++i)
				{
					register int x = x1 + (i*dx)/d;
					register int y = y1 + (i*dy)/d;

					pixel[y][x].blue = color.blue;
					pixel[y][x].green = color.green;
					pixel[y][x].red = color.red;
				}
			}
			else
			{
				uint16 alpha_blue = alpha*color.blue;
				uint16 alpha_green = alpha*color.green;
				uint16 alpha_red = alpha*color.red;
				uint16 beta = 255 - alpha;
				for (register int i = a; i <= b; ++i)
				{
					register int x = x1 + (i*dx)/d;
					register int y = y1 + (i*dy)/d;

					pixel[y][x].blue = static_cast<unsigned char>((alpha_blue + beta*pixel[y][x].blue)/255);
					pixel[y][x].green = static_cast<unsigned char>((alpha_green + beta*pixel[y][x].green)/255);
					pixel[y][x].red = static_cast<unsigned char>((alpha_red + beta*pixel[y][x].red)/255);
				}
			}
		}
		else // Single point
		{
			if ((x1 >= x_max) || (x1 < x_min))
			{
				return;
			}
			if ((y1 >= y_max) || (y1 < y_min))
			{
				return;
			}

			if (alpha == 255)
			{
				pixel[y1][x1].blue = color.blue;
				pixel[y1][x1].green = color.green;
				pixel[y1][x1].red = color.red;
			}
			else
			{
				uint16 beta = 255 - alpha;
				pixel[y1][x1].blue = static_cast<unsigned char>((alpha*color.blue + beta*pixel[y1][x1].blue)/255);
				pixel[y1][x1].green = static_cast<unsigned char>((alpha*color.green + beta*pixel[y1][x1].green)/255);
				pixel[y1][x1].red = static_cast<unsigned char>((alpha*color.red + beta*pixel[y1][x1].red)/255);
			}
		}
	}


	int Image::DrawText(const char* text, int x, int y, const Font& font, const Color& color) throw()
	{
		Assert(text);

		int advance = 0;
		for (const char* t = text; *t; ++t)
		{
			const Glyph& glyph = font.glyphs[static_cast<unsigned char>(*t)];
			if (glyph.data)
			{
				int x_min = x + advance + glyph.offset_x;
				if (x_min >= width)
				{
					advance += glyph.advance;
					continue;
				}
				int x_max = x_min + glyph.width;
				if (x_max < 0)
				{
					advance += glyph.advance;
					continue;
				}

				int y_min = y - glyph.offset_y - glyph.height;
				if (y_min >= height)
				{
					advance += glyph.advance;
					continue;
				}
				int y_max = y_min + glyph.height;
				if (y_max < 0)
				{
					advance += glyph.advance;
					continue;
				}

				int r_min = (y_min < 0) ? -y_min : 0;
				int r_max = (y_max >= height) ? glyph.height + height - y_max : glyph.height;
				int c_min = (x_min < 0) ? -x_min : 0;
				int c_max = (x_max >= width) ? glyph.width + width - x_max : glyph.width;
				for (int r = r_min; r < r_max; ++r)
				{
					register Color* __restrict pixel_y_r = pixel[y_min + r] + x_min;
					const register unsigned char* __restrict glyph_data_r = glyph.data + r*glyph.width;
					for (register int c = c_min; c < c_max; ++c)
					{
						const register uint16 alpha = glyph_data_r[c];
						const register uint16 beta = 255 - alpha;
						pixel_y_r[c].blue = static_cast<unsigned char>((alpha*color.blue + beta*pixel_y_r[c].blue)/255);
						pixel_y_r[c].green = static_cast<unsigned char>((alpha*color.green + beta*pixel_y_r[c].green)/255);
						pixel_y_r[c].red = static_cast<unsigned char>((alpha*color.red + beta*pixel_y_r[c].red)/255);
					}
				}
			}
			advance += glyph.advance;
		}
		return advance;
	}


	int Image::DrawText(const char* text, int x, int y, const Font& font, const Color& color, const Box& clip_box) throw()
	{
		Assert(text);

		int advance = 0;
		for (const char* t = text; *t; ++t)
		{
			const Glyph& glyph = font.glyphs[static_cast<unsigned char>(*t)];
			if (glyph.data)
			{
				int x_min = x + advance + glyph.offset_x;
				if ((x_min > clip_box.x2) || (x_min >= width))
				{
					advance += glyph.advance;
					continue;
				}
				int x_max = x_min + glyph.width;
				if ((x_max <= clip_box.x1) || (x_max < 0))
				{
					advance += glyph.advance;
					continue;
				}

				int y_min = y - glyph.offset_y - glyph.height;
				if ((y_min > clip_box.y2) || (y_min >= height))
				{
					advance += glyph.advance;
					continue;
				}
				int y_max = y_min + glyph.height;
				if ((y_max <= clip_box.y1) || (y_max < 0))
				{
					advance += glyph.advance;
					continue;
				}

				int r_min = (y_min > clip_box.y1) ? (y_min < 0 ? -y_min : 0) : (y_min < clip_box.y1 ? clip_box.y1 - y_min : 0);
				int r_max = (y_max < clip_box.y2) ? (y_max >= height ? glyph.height + height - y_max : glyph.height) : (y_max >= clip_box.y2 ? glyph.height + clip_box.y2 - y_max : glyph.height);
				int c_min = (x_min > clip_box.x1) ? (x_min < 0 ? -x_min : 0) : (x_min < clip_box.x1 ? clip_box.x1 - x_min : 0);
				int c_max = (x_max < clip_box.x2) ? (x_max >= width ? glyph.width + width - x_max : glyph.width) : (x_max >= clip_box.x2 ? glyph.width + clip_box.x2 - x_max : glyph.width);
				for (int r = r_min; r < r_max; ++r)
				{
					register Color* __restrict pixel_y_r = pixel[y_min + r] + x_min;
					const register unsigned char* __restrict glyph_data_r = glyph.data + r*glyph.width;
					for (register int c = c_min; c < c_max; ++c)
					{
						const register uint16 alpha = glyph_data_r[c];
						const register uint16 beta = 255 - alpha;
						pixel_y_r[c].blue = static_cast<unsigned char>((alpha*color.blue + beta*pixel_y_r[c].blue)/255);
						pixel_y_r[c].green = static_cast<unsigned char>((alpha*color.green + beta*pixel_y_r[c].green)/255);
						pixel_y_r[c].red = static_cast<unsigned char>((alpha*color.red + beta*pixel_y_r[c].red)/255);
					}
				}
			}
			advance += glyph.advance;
		}
		return advance;
	}


	void Image::DrawCircle(int x0, int y0, int r, const Color& color, unsigned char alpha) throw()
	{
		int y_min = y0 - r;
		int y_max = y0 + r;
		for (int y = y_min; y <= y_max; ++y)
		{
			int s = static_cast<int>(sqrtf(static_cast<float>(r*r - (y - y0)*(y - y0))));
			int x_min = x0 - s;
			int x_max = x0 + s;
			DrawLine(x_min, y, x_max, y, color, alpha);
		}

	}


	void Image::DrawRectangle(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha) throw()
	{
		int x_min;
		int x_max;
		if (x1 < x2)
		{
			if ((x2 < 0) || (x1 >= width))
			{
				return;
			}
			x_min = x1 > 0 ? x1 : 0;
			x_max = x2 < width ? x2 + 1 : width;
		}
		else
		{
			if ((x1 < 0) || (x2 >= width))
			{
				return;
			}
			x_min = x2 > 0 ? x2 : 0;
			x_max = x1 < width ? x1 + 1 : width;
		}
		if (x_min == x_max)
		{
			return;
		}

		int y_min;
		int y_max;
		if (y1 < y2)
		{
			if (y2 < 0)
			{
				return;
			}
			if (y1 >= height)
			{
				return;
			}
			y_min = y1 > 0 ? y1 : 0;
			y_max = y2 < height ? y2 + 1 : height;
		}
		else
		{
			if (y1 < 0)
			{
				return;
			}
			if (y2 >= height)
			{
				return;
			}
			y_min = y2 > 0 ? y2 : 0;
			y_max = y1 < height ? y1 + 1 : height;
		}
		if (y_min == y_max)
		{
			return;
		}

		const register uint16 alpha_blue = static_cast<uint16>(alpha*color.blue);
		const register uint16 alpha_green = static_cast<uint16>(alpha*color.green);
		const register uint16 alpha_red = static_cast<uint16>(alpha*color.red);
		const register uint16 beta = 255 - alpha;
		for (int y = y_min; y < y_max; ++y)
		{
			register Color* __restrict pixel_y = pixel[y];
			for (register int x = x_min; x < x_max; ++x)
			{
				pixel_y[x].blue = static_cast<unsigned char>((alpha_blue + beta*pixel_y[x].blue)/255);
				pixel_y[x].green = static_cast<unsigned char>((alpha_green + beta*pixel_y[x].green)/255);
				pixel_y[x].red = static_cast<unsigned char>((alpha_red + beta*pixel_y[x].red)/255);
			}
		}
	}


	void Image::DrawRectangle(int x1, int y1, int x2, int y2, const Color& color, unsigned char alpha, const Box& clip_box) throw()
	{
		int x_min;
		int x_max;
		if (x1 < x2)
		{
			if ((x2 < 0) || (x2 < clip_box.x1))
			{
				return;
			}
			if ((x1 >= width) || (x1 > clip_box.x2))
			{
				return;
			}
			x_min = (clip_box.x1 > x1) ? (clip_box.x1 > 0 ? clip_box.x1 : 0) : (x1 > 0 ? x1 : 0);
			x_max = (clip_box.x2 < x2) ? (clip_box.x2 < width ? clip_box.x2 + 1 : width) : (x2 < width ? x2 + 1 : width);
		}
		else
		{
			if ((x1 < 0) || (x1 < clip_box.x1))
			{
				return;
			}
			if ((x2 >= width) || (x2 > clip_box.x2))
			{
				return;
			}
			x_min = (clip_box.x1 > x2) ? (clip_box.x1 > 0 ? clip_box.x1 : 0) : (x2 > 0 ? x2 : 0);
			x_max = (clip_box.x2 < x1) ? (clip_box.x2 < width ? clip_box.x2 + 1 : width) : (x1 < width ? x1 + 1 : width);
		}
		if (x_min == x_max)
		{
			return;
		}

		int y_min;
		int y_max;
		if (y1 < y2)
		{
			if ((y2 < 0) || (y2 < clip_box.y1))
			{
				return;
			}
			if ((y1 >= height) || (y1 > clip_box.y2))
			{
				return;
			}
			y_min = (clip_box.y1 > y1) ? (clip_box.y1 > 0 ? clip_box.y1 : 0) : (y1 > 0 ? y1 : 0);
			y_max = (clip_box.y2 < y2) ? (clip_box.y2 < height ? clip_box.y2 + 1 : height) : (y2 < width ? y2 + 1 : height);
		}
		else
		{
			if ((y1 < 0) || (y1 < clip_box.y1))
			{
				return;
			}
			if ((y2 >= height) || (y2 > clip_box.y2))
			{
				return;
			}
			y_min = (clip_box.y1 > y2) ? (clip_box.y1 > 0 ? clip_box.y1 : 0) : (y2 > 0 ? y2 : 0);
			y_max = (clip_box.y2 < y1) ? (clip_box.y2 < height ? clip_box.y2 + 1 : height) : (y1 < height ? y1 + 1 : height);
		}
		if (y_min == y_max)
		{
			return;
		}

		const register uint16 alpha_blue = static_cast<uint16>(alpha*color.blue);
		const register uint16 alpha_green = static_cast<uint16>(alpha*color.green);
		const register uint16 alpha_red = static_cast<uint16>(alpha*color.red);
		const register uint16 beta = 255 - alpha;
		for (int y = y_min; y < y_max; ++y)
		{
			register Color* __restrict pixel_y = pixel[y];
			for (register int x = x_min; x < x_max; ++x)
			{
				pixel_y[x].blue = static_cast<unsigned char>((alpha_blue + beta*pixel_y[x].blue)/255);
				pixel_y[x].green = static_cast<unsigned char>((alpha_green + beta*pixel_y[x].green)/255);
				pixel_y[x].red = static_cast<unsigned char>((alpha_red + beta*pixel_y[x].red)/255);
			}
		}
	}


	void Image::LoadPPM(const char* file_name, bool top_to_bottom, int align) throw(MemoryException, FileException)
	{
		try
		{
			File file;
			file.Open(file_name);
			file.comment_delimiter = '#';

			char id[3];
			file.Read(id, 2);
			if ((id[0] != 'P') || (id[1] != '6'))
			{
				DebugMessage("Invalid image format");
				Throw(FileException(FileException::format_error));
			}

			file.SkipComments();

			int new_width;
			int new_height;
			file.Get(new_width);
			file.Get(new_height);
			Resize(new_width, new_height, top_to_bottom, align);

			int max;
			file.Get(max);

			file.SkipComments();

			for (int y = 0; y < height; ++y)
			{
				Color* __restrict pixel_y = pixel[y];
				for (int x = 0; x < width; ++x)
				{
					file.Read(pixel_y[x].red);
					file.Read(pixel_y[x].green);
					file.Read(pixel_y[x].blue);
				}
			}
			file.Close();
		}
		catch (...)
		{
			ReThrow();
		}
	}
	

	void Image::Resize(int width, int height, bool top_to_bottom, int align) throw(MemoryException)
	{
		Assert(width >= 0);
		Assert(height >= 0);

		if (pixel)
		{
			delete [] pixel[this->top_to_bottom ? 0 : this->height - 1];
			delete [] pixel;
		}

		if ((width == 0) && (height == 0))
		{
			pixel = static_cast<Color**>(0);
			this->width = 0;
			this->height = 0;
			this->align = 0;
			this->top_to_bottom = false;
			return;
		}

		pixel = new Color*[height];
		if (!pixel)
		{
			Throw(MemoryException());
		}

		size_t line_size = SIZE_WITH_PAD(Color, width, align);
		register unsigned char* __restrict data = new unsigned char[static_cast<unsigned int>(height)*line_size];
		if (!data)
		{
			delete [] pixel;
			Throw(MemoryException());
		}

		if (top_to_bottom)
		{
			for (register int y = 0; y < height; ++y)
			{
				pixel[y] = reinterpret_cast<Color*>(data);
				data += line_size;
			}
		}
		else
		{
			for (register int y = height - 1; y >= 0; --y)
			{
				pixel[y] = reinterpret_cast<Color*>(data);
				data += line_size;
			}
		}

		this->width = width;
		this->height = height;
		this->align = align;
		this->top_to_bottom = top_to_bottom;
	}


	void Image::ResizeToFit(int width, int height, bool top_to_bottom, int align) throw(MemoryException)
	{
		if ((width > this->width) || (height > this->height))
		{
			Resize(width > this->width ? width : this->width, height > this->height ? height : this->height, top_to_bottom, align);
		}
	}


	void Image::SavePPM(const char* file_name) throw(FileException)
	{
		try
		{
			IntegerFormat integer_format(false, false, 1, IntegerNotation::decimal, false);

			File file;
			file.Create(file_name);
			file.Put("P6\n");
			file.Put(width, integer_format);
			file.Put(" ");
			file.Put(height, integer_format);
			file.Put(" 255\n");
			for (int y = 0; y < height; ++y)
			{
				Color* __restrict pixel_y = pixel[y];
				for (int x = 0; x < width; ++x)
				{
					file.Write(pixel_y[x].red);
					file.Write(pixel_y[x].green);
					file.Write(pixel_y[x].blue);
				}
			}
			file.Close();
		}
		catch (...)
		{
			ReThrow();
		}
	}
}
