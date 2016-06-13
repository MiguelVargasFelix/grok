// Log.cpp
// Copyright (C) 2013 Miguel Vargas-Felix (miguel.vargas@gmail.com)
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
#include <Basic/Console.h>
#include <Basic/Log.h>
#include <Basic/System.h>
#include <Basic/Time.h>

#if defined(CC_Microsoft)
	#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdarg.h>
#include <stdio.h>
#if defined(CC_Clang)
	#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
	#pragma clang diagnostic ignored "-Wformat-nonliteral"
	#pragma clang diagnostic ignored "-Wexit-time-destructors"
	#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif


namespace Grok
{
	Log message_log;


	Log::Log(LogLevel::ID level, bool use_colors) throw()
	{
		Assert(level >= 0);

		file_stream = (void*)stdout;
		start_time.SetEpochTime();
		this->level = level;
		this->use_colors = use_colors;
	}


	Log::~Log() throw()
	{
		Assert(file_stream);

		if (file_stream != stdout)
		{
			fclose((FILE*)file_stream);
		}
	}


	void Log::Post(LogLevel::ID type, const char* format, ...) throw(LogException)
	{
		Assert(type >= 0);
		Assert(file_stream);

		if (type <= level)
		{
			Time current_time;

			current_time.SetEpochTime();

			Time time_difference = current_time - start_time;

			if (use_colors)
			{
				switch (type)
				{
					case LogLevel::error:
					{
						Console::SetForeground(ConsoleColor::red, true);
						break;
					}
					case LogLevel::warning:
					{
						Console::SetForeground(ConsoleColor::yellow, true);
						break;
					}
					case LogLevel::success:
					{
						Console::SetForeground(ConsoleColor::green, true);
						break;
					}
					case LogLevel::highlight:
					{
						Console::SetForeground(ConsoleColor::standard, true);
						break;
					}
					case LogLevel::message:
					{
						Console::SetForeground(ConsoleColor::standard, false);
						break;
					}
					case LogLevel::debug:
					{
						Console::SetForeground(ConsoleColor::cyan, false);
						break;
					}
					case LogLevel::debug2:
					{
						Console::SetForeground(ConsoleColor::cyan, false);
						break;
					}
				};
			}

			char code_id;
			switch (type)
			{
				case LogLevel::error:
				{
					code_id = 'E';
					break;
				}
				case LogLevel::warning:
				{
					code_id = 'W';
					break;
				}
				case LogLevel::success:
				{
					code_id = 'S';
					break;
				}
				case LogLevel::highlight:
				{
					code_id = 'H';
					break;
				}
				case LogLevel::message:
				{
					code_id = 'M';
					break;
				}
				case LogLevel::debug:
				{
					code_id = 'D';
					break;
				}
				case LogLevel::debug2:
				{
					code_id = 'D';
					break;
				}
				default:
				{
					code_id = ' ';
					break;
				}
			};
			fprintf((FILE*)file_stream, "%c%9li.%03i  ", code_id, time_difference.seconds, time_difference.milliseconds);
			va_list arguments;
			va_start(arguments, format);
			vfprintf((FILE*)file_stream, format, arguments);
			va_end(arguments);
			if (use_colors)
			{
				Console::SetForeground(ConsoleColor::standard, false);
			}
			fputc('\n', (FILE*)file_stream);
			fflush((FILE*)file_stream);
		}
	}


	void Log::RestartTime() throw()
	{
		start_time.SetEpochTime();
	}


	void Log::SetOutputToFile(const char* file_name, bool append, LogLevel::ID level, bool use_colors) throw(LogException)
	{
		Assert(file_name);
		Assert(file_stream);

		if ((FILE*)file_stream != stdout)
		{
			fclose((FILE*)file_stream);
		}

		file_stream = (void*)fopen(file_name, append ? "ab" : "wb");
		if (!file_stream)
		{
			Throw(LogException());
		}
		this->level = level;
		this->use_colors = use_colors;
	}


	void Log::SetOutputToConsole(LogLevel::ID level, bool use_colors) throw(LogException)
	{
		Assert(file_stream);

		if ((FILE*)file_stream != stdout)
		{
			fclose((FILE*)file_stream);
		}
		file_stream = (void*)stdout;
		this->level = level;
		this->use_colors = use_colors;
	}
}
