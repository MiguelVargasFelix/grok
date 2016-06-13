// Time.cpp
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

#include <Basic/Time.h>
#include <Basic/System.h>


#if defined(OS_FreeBSD)

	#include <sys/time.h>

	namespace Grok
	{
		void Time::UseCurrentTime() throw()
		{
			struct timeval time_value;
			gettimeofday(&time_value, static_cast<timezone*>(0));
			seconds = static_cast<long>(time_value.tv_sec);
			milliseconds = static_cast<int>(time_value.tv_usec/1000);
		}
	}

#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_Windows) || defined(OS_Linux)

	#include <sys/timeb.h>

	namespace Grok
	{
		void Time::UseCurrentTime() throw()
		{
			timeb system_time;
			ftime(&system_time);
			seconds = static_cast<long>(system_time.time);
			milliseconds = static_cast<int>(system_time.millitm);
		}
	}

#endif


namespace Grok
{
	Time operator - (const Time& a, const Time& b) throw()
	{
		long difference = (a.seconds - b.seconds)*1000 + (a.milliseconds - b.milliseconds);
		return Time(difference/1000, static_cast<int>(difference % 1000));
	}
}
