// Color.h
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
	struct Color
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
	};


	struct ColorHSL
	{
		float hue;
		float saturation;
		float lightness;
	};


	void BGRtoHSL(const Color& bgr, ColorHSL& hsl) throw();


	void HSLtoBGR(const ColorHSL& hsl, Color& bgr) throw();


	namespace Colors // http://www.w3.org/TR/css3-color/#svg-color
	{
		extern Color aliceblue;
		extern Color antiquewhite;
		extern Color aqua;
		extern Color aquamarine;
		extern Color azure;
		extern Color beige;
		extern Color bisque;
		extern Color black;
		extern Color blanchedalmond;
		extern Color blue;
		extern Color blueviolet;
		extern Color brown;
		extern Color burlywood;
		extern Color cadetblue;
		extern Color chartreuse;
		extern Color chocolate;
		extern Color coral;
		extern Color cornflowerblue;
		extern Color cornsilk;
		extern Color crimson;
		extern Color cyan;
		extern Color darkblue;
		extern Color darkcyan;
		extern Color darkgoldenrod;
		extern Color darkgray;
		extern Color darkgreen;
		extern Color darkgrey;
		extern Color darkkhaki;
		extern Color darkmagenta;
		extern Color darkolivegreen;
		extern Color darkorange;
		extern Color darkorchid;
		extern Color darkred;
		extern Color darksalmon;
		extern Color darkseagreen;
		extern Color darkslateblue;
		extern Color darkslategray;
		extern Color darkslategrey;
		extern Color darkturquoise;
		extern Color darkviolet;
		extern Color deeppink;
		extern Color deepskyblue;
		extern Color dimgray;
		extern Color dimgrey;
		extern Color dodgerblue;
		extern Color firebrick;
		extern Color floralwhite;
		extern Color forestgreen;
		extern Color fuchsia;
		extern Color gainsboro;
		extern Color ghostwhite;
		extern Color gold;
		extern Color goldenrod;
		extern Color gray;
		extern Color green;
		extern Color greenyellow;
		extern Color grey;
		extern Color honeydew;
		extern Color hotpink;
		extern Color indianred;
		extern Color indigo;
		extern Color ivory;
		extern Color khaki;
		extern Color lavender;
		extern Color lavenderblush;
		extern Color lawngreen;
		extern Color lemonchiffon;
		extern Color lightblue;
		extern Color lightcoral;
		extern Color lightcyan;
		extern Color lightgoldenrodyellow;
		extern Color lightgray;
		extern Color lightgreen;
		extern Color lightgrey;
		extern Color lightpink;
		extern Color lightsalmon;
		extern Color lightseagreen;
		extern Color lightskyblue;
		extern Color lightslategray;
		extern Color lightslategrey;
		extern Color lightsteelblue;
		extern Color lightyellow;
		extern Color lime;
		extern Color limegreen;
		extern Color linen;
		extern Color magenta;
		extern Color maroon;
		extern Color mediumaquamarine;
		extern Color mediumblue;
		extern Color mediumorchid;
		extern Color mediumpurple;
		extern Color mediumseagreen;
		extern Color mediumslateblue;
		extern Color mediumspringgreen;
		extern Color mediumturquoise;
		extern Color mediumvioletred;
		extern Color midnightblue;
		extern Color mintcream;
		extern Color mistyrose;
		extern Color moccasin;
		extern Color navajowhite;
		extern Color navy;
		extern Color oldlace;
		extern Color olive;
		extern Color olivedrab;
		extern Color orange;
		extern Color orangered;
		extern Color orchid;
		extern Color palegoldenrod;
		extern Color palegreen;
		extern Color paleturquoise;
		extern Color palevioletred;
		extern Color papayawhip;
		extern Color peachpuff;
		extern Color peru;
		extern Color pink;
		extern Color plum;
		extern Color powderblue;
		extern Color purple;
		extern Color red;
		extern Color rosybrown;
		extern Color royalblue;
		extern Color saddlebrown;
		extern Color salmon;
		extern Color sandybrown;
		extern Color seagreen;
		extern Color seashell;
		extern Color sienna;
		extern Color silver;
		extern Color skyblue;
		extern Color slateblue;
		extern Color slategray;
		extern Color slategrey;
		extern Color snow;
		extern Color springgreen;
		extern Color steelblue;
		extern Color tan;
		extern Color teal;
		extern Color thistle;
		extern Color tomato;
		extern Color turquoise;
		extern Color violet;
		extern Color wheat;
		extern Color white;
		extern Color whitesmoke;
		extern Color yellow;
		extern Color yellowgreen;
	}
}
