// Color.cpp
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
#include <Image/Color.h>

#include <cmath>


namespace Grok
{
	namespace Colors
	{
		Color aliceblue = {255,248,240};
		Color antiquewhite = {215,235,250};
		Color aqua = {255,255,0};
		Color aquamarine = {212,255,127};
		Color azure = {255,255,240};
		Color beige = {220,245,245};
		Color bisque = {196,228,255};
		Color black = {0,0,0};
		Color blanchedalmond = {205,235,255};
		Color blue = {255,0,0};
		Color blueviolet = {226,43,138};
		Color brown = {42,42,165};
		Color burlywood = {135,184,222};
		Color cadetblue = {160,158,95};
		Color chartreuse = {0,255,127};
		Color chocolate = {30,105,210};
		Color coral = {80,127,255};
		Color cornflowerblue = {237,149,100};
		Color cornsilk = {220,248,255};
		Color crimson = {60,20,220};
		Color cyan = {255,255,0};
		Color darkblue = {139,0,0};
		Color darkcyan = {139,139,0};
		Color darkgoldenrod = {11,134,184};
		Color darkgray = {169,169,169};
		Color darkgreen = {0,100,0};
		Color darkgrey = {169,169,169};
		Color darkkhaki = {107,183,189};
		Color darkmagenta = {139,0,139};
		Color darkolivegreen = {47,107,85};
		Color darkorange = {0,140,255};
		Color darkorchid = {204,50,153};
		Color darkred = {0,0,139};
		Color darksalmon = {122,150,233};
		Color darkseagreen = {143,188,143};
		Color darkslateblue = {139,61,72};
		Color darkslategray = {79,79,47};
		Color darkslategrey = {79,79,47};
		Color darkturquoise = {209,206,0};
		Color darkviolet = {211,0,148};
		Color deeppink = {147,20,255};
		Color deepskyblue = {255,191,0};
		Color dimgray = {105,105,105};
		Color dimgrey = {105,105,105};
		Color dodgerblue = {255,144,30};
		Color firebrick = {34,34,178};
		Color floralwhite = {240,250,255};
		Color forestgreen = {34,139,34};
		Color fuchsia = {255,0,255};
		Color gainsboro = {220,220,220};
		Color ghostwhite = {255,248,248};
		Color gold = {0,215,255};
		Color goldenrod = {32,165,218};
		Color gray = {128,128,128};
		Color green = {0,128,0};
		Color greenyellow = {47,255,173};
		Color grey = {128,128,128};
		Color honeydew = {240,255,240};
		Color hotpink = {180,105,255};
		Color indianred = {92,92,205};
		Color indigo = {130,0,75};
		Color ivory = {240,255,255};
		Color khaki = {140,230,240};
		Color lavender = {250,230,230};
		Color lavenderblush = {245,240,255};
		Color lawngreen = {0,252,124};
		Color lemonchiffon = {205,250,255};
		Color lightblue = {230,216,173};
		Color lightcoral = {128,128,240};
		Color lightcyan = {255,255,224};
		Color lightgoldenrodyellow = {210,250,250};
		Color lightgray = {211,211,211};
		Color lightgreen = {144,238,144};
		Color lightgrey = {211,211,211};
		Color lightpink = {193,182,255};
		Color lightsalmon = {122,160,255};
		Color lightseagreen = {170,178,32};
		Color lightskyblue = {250,206,135};
		Color lightslategray = {153,136,119};
		Color lightslategrey = {153,136,119};
		Color lightsteelblue = {222,196,176};
		Color lightyellow = {224,255,255};
		Color lime = {0,255,0};
		Color limegreen = {50,205,50};
		Color linen = {230,240,250};
		Color magenta = {255,0,255};
		Color maroon = {0,0,128};
		Color mediumaquamarine = {170,205,102};
		Color mediumblue = {205,0,0};
		Color mediumorchid = {211,85,186};
		Color mediumpurple = {219,112,147};
		Color mediumseagreen = {113,179,60};
		Color mediumslateblue = {238,104,123};
		Color mediumspringgreen = {154,250,0};
		Color mediumturquoise = {204,209,72};
		Color mediumvioletred = {133,21,199};
		Color midnightblue = {112,25,25};
		Color mintcream = {250,255,245};
		Color mistyrose = {225,228,255};
		Color moccasin = {181,228,255};
		Color navajowhite = {173,222,255};
		Color navy = {128,0,0};
		Color oldlace = {230,245,253};
		Color olive = {0,128,128};
		Color olivedrab = {35,142,107};
		Color orange = {0,165,255};
		Color orangered = {0,69,255};
		Color orchid = {214,112,218};
		Color palegoldenrod = {170,232,238};
		Color palegreen = {152,251,152};
		Color paleturquoise = {238,238,175};
		Color palevioletred = {147,112,219};
		Color papayawhip = {213,239,255};
		Color peachpuff = {185,218,255};
		Color peru = {63,133,205};
		Color pink = {203,192,255};
		Color plum = {221,160,221};
		Color powderblue = {230,224,176};
		Color purple = {128,0,128};
		Color red = {0,0,255};
		Color rosybrown = {143,143,188};
		Color royalblue = {225,105,65};
		Color saddlebrown = {19,69,139};
		Color salmon = {114,128,250};
		Color sandybrown = {96,164,244};
		Color seagreen = {87,139,46};
		Color seashell = {238,245,255};
		Color sienna = {45,82,160};
		Color silver = {192,192,192};
		Color skyblue = {235,206,135};
		Color slateblue = {205,90,106};
		Color slategray = {144,128,112};
		Color slategrey = {144,128,112};
		Color snow = {250,250,255};
		Color springgreen = {127,255,0};
		Color steelblue = {180,130,70};
		Color tan = {140,180,210};
		Color teal = {128,128,0};
		Color thistle = {216,191,216};
		Color tomato = {71,99,255};
		Color turquoise = {208,224,64};
		Color violet = {238,130,238};
		Color wheat = {179,222,245};
		Color white = {255,255,255};
		Color whitesmoke = {245,245,245};
		Color yellow = {0,255,255};
		Color yellowgreen = {50,205,154};
	}


	void BGRtoHSL(const Color& bgr, ColorHSL& hsl) throw()
	{
		// http://en.wikipedia.org/wiki/HSL_and_HSV

		float M;
		float m;
		float C;
		float h;

		if (bgr.blue > bgr.green)
		{
			if (bgr.green > bgr.red)
			{
				M = bgr.blue/255.0f;
				m = bgr.red/255.0f;
				C = M - m;
				h = (static_cast<float>(bgr.red) - static_cast<float>(bgr.green))/255.0f/C + 4.0f;
			}
			else if (bgr.red > bgr.blue)
			{
				M = bgr.red/255.0f;
				m = bgr.green/255.0f;
				C = M - m;
				h = fmodf((static_cast<float>(bgr.green) - static_cast<float>(bgr.blue))/255.0f/C, 6.0f);
			}
			else
			{
				M = bgr.blue/255.0f;
				m = bgr.green/255.0f;
				C = M - m;
				h = (static_cast<float>(bgr.red) - static_cast<float>(bgr.green))/255.0f/C + 4.0f;
			}
		}
		else
		{
			if (bgr.blue > bgr.red)
			{
				M = bgr.green/255.0f;
				m = bgr.red/255.0f;
				C = M - m;
				h = (static_cast<float>(bgr.blue) - static_cast<float>(bgr.red))/255.0f/C + 2.0f;
			}
			else if (bgr.red > bgr.green)
			{
				M = bgr.red/255.0f;
				m = bgr.blue/255.0f;
				C = M - m;
				h = fmodf((static_cast<float>(bgr.green) - static_cast<float>(bgr.blue))/255.0f/C, 6.0f);
			}
			else
			{
				M = bgr.green/255.0f;
				m = bgr.blue/255.0f;
				C = M - m;
				h = (static_cast<float>(bgr.blue) - static_cast<float>(bgr.red))/255.0f/C + 2.0f;
			}
		}
		hsl.hue = h*60.0f;
		hsl.lightness = 0.5f*(M + m);
		hsl.saturation = (C == 0.0f) ? 0.0f : C/(1.0f - fabsf(2.0f*hsl.lightness - 1.0f));
	}


	void HSLtoBGR(const ColorHSL& hsl, Color& bgr) throw()
	{
		// http://en.wikipedia.org/wiki/HSL_and_HSV

		Assert((hsl.saturation >= 0.0f) && (hsl.saturation <= 1.0f));
		Assert((hsl.lightness >= 0.0f) && (hsl.lightness <= 1.0f));

		float hsl_hue = (hsl.hue < 0.0f) ? 0.0f : (hsl.hue > 360.0f) ? 360.0f : hsl.hue;
		float c = (1.0f - fabsf(2.0f*hsl.lightness - 1.0f))*hsl.saturation;
		float h = hsl_hue/60.0f;
		float x = c*(1.0f - fabsf(fmodf(h, 2.0f) - 1.0f));
		unsigned char ucc = static_cast<unsigned char>(c*255.0f);
		unsigned char ucx = static_cast<unsigned char>(x*255.0f);
		switch (static_cast<int>(floor(h)))
		{
			case 0:
			{
				bgr.blue = 0;
				bgr.green = ucx;
				bgr.red = ucc;
				break;
			}
			case 1:
			{
				bgr.blue = 0;
				bgr.green = ucc;
				bgr.red = ucx;
				break;
			}
			case 2:
			{
				bgr.blue = ucx;
				bgr.green = ucc;
				bgr.red = 0;
				break;
			}
			case 3:
			{
				bgr.blue = ucc;
				bgr.green = ucx;
				bgr.red = 0;
				break;
			}
			case 4:
			{
				bgr.blue = ucc;
				bgr.green = 0;
				bgr.red = ucx;
				break;
			}
			case 5:
			{
				bgr.blue = ucx;
				bgr.green = 0;
				bgr.red = ucc;
				break;
			}
		}
		unsigned char ucm = static_cast<unsigned char>((hsl.lightness - 0.5f*c)*255.0f);
		bgr.blue += ucm;
		bgr.green += ucm;
		bgr.red += ucm;
	}
}
