// Log.h
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

#include <Basic/Exception.h>
#include <Basic/Time.h>


namespace Grok
{
	namespace LogLevel
	{
		enum ID
		{
			error     = 0,
			warning   = 1,
			success   = 2,
			highlight = 3,
			message   = 4,
			debug     = 5
		};
	}


	struct LogException : public Exception
	{
	};


	class Log
	{
		protected:

			void* file_stream;


		public:

			Time start_time;


			LogLevel::ID level;


			bool use_colors;


			Log(LogLevel::ID level = LogLevel::message, bool use_colors = true) throw();


			void SetOutputToFile(const char* file_name, bool append = false, LogLevel::ID level = LogLevel::message, bool use_colors = false) throw(LogException);


			void SetOutputToConsole(LogLevel::ID level = LogLevel::message, bool use_colors = true) throw(LogException);


			~Log() throw();


			void Post(LogLevel::ID type, const char* format, ...) throw(LogException);


			void RestartTime() throw();
	};


	extern Log message_log;
}
