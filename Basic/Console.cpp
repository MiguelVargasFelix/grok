// Console.cpp
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

#include <Basic/Console.h>
#include <Basic/System.h>


#if defined(OS_Windows)

	#define WINVER         0x0502
	#define _WIN32_WINNT   0x0502
	#define _WIN32_WINDOWS 0x0502
	#define WIN32_LEAN_AND_MEAN
	#define NOCOMM
	#define NODEFERWINDOWPOS
	#define NOHELP
	#define NOIME
	#define NOMCX
	#define NOPROFILER
	#define NOSERVICE
	#define NOWH
	#include <windows.h>


	namespace Grok
	{
		static WORD GetDefaultAttributes() throw()
		{
			HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
			GetConsoleScreenBufferInfo(std_output_handle, &console_screen_buffer_info);

			return console_screen_buffer_info.wAttributes;
		}


		static const WORD default_attributes = GetDefaultAttributes();


		void Console::SetForeground(ConsoleColor::ID color, bool bold) throw()
		{
			static const WORD color_code[8] = // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682088%28v=vs.85%29.aspx
			{
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // white
				FOREGROUND_RED,   // red
				FOREGROUND_GREEN, // green
				FOREGROUND_BLUE,  // blue
				FOREGROUND_GREEN | FOREGROUND_BLUE, // cyan
				FOREGROUND_RED | FOREGROUND_BLUE,   // magenta
				FOREGROUND_RED | FOREGROUND_GREEN,  // yellow
				0 // black
			};

			HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
			GetConsoleScreenBufferInfo(std_output_handle, &console_screen_buffer_info);

			WORD attributes = console_screen_buffer_info.wAttributes;
			attributes &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			if (color == ConsoleColor::standard)
			{
				attributes |= default_attributes & (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			else
			{
				attributes |= color_code[(int)color];
			}
			if (bold)
			{
				attributes |= FOREGROUND_INTENSITY;
			}
			SetConsoleTextAttribute(std_output_handle, attributes);
		}


		void Console::SetBackground(ConsoleColor::ID color) throw()
		{
			static const WORD color_code[8] = // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682088%28v=vs.85%29.aspx
			{
				BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE, // white
				BACKGROUND_RED,   // red
				BACKGROUND_GREEN, // green
				BACKGROUND_BLUE,  // blue
				BACKGROUND_GREEN | BACKGROUND_BLUE, // cyan
				BACKGROUND_RED | BACKGROUND_BLUE,   // magenta
				BACKGROUND_RED | BACKGROUND_GREEN,  // yellow
				0 // black
			};

			HANDLE std_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
			GetConsoleScreenBufferInfo(std_output_handle, &console_screen_buffer_info);

			WORD attributes = console_screen_buffer_info.wAttributes;
			attributes &= ~(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			if (color == ConsoleColor::standard)
			{
				attributes |= default_attributes & (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
			}
			else
			{
				attributes |= color_code[(int)color];
			}
			SetConsoleTextAttribute(std_output_handle, attributes);
		}
	}

#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_FreeBSD) || defined(OS_Linux)

	#include <stdio.h>


	namespace Grok
	{
		static const char ansi_color_code[] = // http://en.wikipedia.org/wiki/Ansi_escape_codes
		{
			7, // white
			1, // red
			2, // green
			4, // blue
			6, // cyan
			5, // magenta
			3, // yellow
			0  // black
		};


		void Console::SetForeground(ConsoleColor::ID color, bool bold) throw()
		{
			char string_code[16];
			register char* s = string_code;
			*s = '\x1b';
			*(++s) = '[';
			*(++s) = '3';
			*(++s) = (color == ConsoleColor::standard) ? '9' : '0' + ansi_color_code[static_cast<int>(color)];
			if (bold)
			{
				*(++s) = ';';
				*(++s) = '1';
			}
			else
			{
				*(++s) = ';';
				*(++s) = '2';
				*(++s) = '2';
			}
			*(++s) = 'm';
			*(++s) = '\0';
			fputs(string_code, stdout);
		}


		void Console::SetBackground(ConsoleColor::ID color) throw()
		{
			char string_code[8];
			register char* s = string_code;
			*s = '\x1b';
			*(++s) = '[';
			*(++s) = '4';
			*(++s) = (color == ConsoleColor::standard) ? '9' : '0' + ansi_color_code[static_cast<int>(color)];
			*(++s) = 'm';
			*(++s) = '\0';
			fputs(string_code, stdout);
		}
	}

#endif
