// File.h
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
#include <Basic/Integer.h>
#include <Basic/String.h>

#define STRING_DEFAULT_MAXIMUM_SIZE 512


namespace Grok
{
	struct FileException : public Exception
	{
		enum ErrorType
		{
			create_error = 1,
			eof_error    = 2,
			flush_error  = 3,
			format_error = 4,
			open_error   = 5,
			read_error   = 6,
			seek_error   = 7,
			tell_error   = 8,
			write_error  = 9
		};

		ErrorType error_type;

		FileException(ErrorType error_type) throw()
		:	Exception(),
			error_type(error_type)
		{
		}
	};


	class File
	{
		public:

			File() throw();


			File(const File& file) throw();


			~File() throw();


			File& operator = (const File& file) throw();


			void Close() throw();


			void Create(const char* file_name) throw(FileException);


			void CreatePipe(const char* pipe_name, bool read, bool write) throw(FileException);


			void Flush() throw(FileException);


			void Get(bool& value) throw(FileException);


			void Get(unsigned char& value) throw(FileException);


			void Get(short& value) throw(FileException);


			void Get(unsigned short& value) throw(FileException);


			void Get(int& value) throw(FileException);


			void Get(unsigned int& value) throw(FileException);


			void Get(long& value) throw(FileException);


			void Get(unsigned long& value) throw(FileException);


			void Get(float& value) throw(FileException);


			void Get(double& value) throw(FileException);


			void Get(char* string, int max_size, const char* separators = " \f\n\r\t\v") throw(FileException);


			void Get(String& string, int max_size = STRING_DEFAULT_MAXIMUM_SIZE, const char* separators = " \f\n\r\t\v") throw(FileException);


			void GetLine(char* string, int max_size) throw(FileException);


			void GetLine(String& string, int max_size = STRING_DEFAULT_MAXIMUM_SIZE) throw(FileException);


			void GoBegin(sint64 offset_bytes = 0) throw(FileException);


			void GoEnd(sint64 offset_bytes = 0) throw(FileException);


			void GoTo(sint64 offset_bytes) throw(FileException);


			void Open(const char* file_name) throw(FileException);


			void Put(const bool& value) throw(FileException);


			void Put(const unsigned char& value, const Format& format) throw(FileException);


			void Put(const short& value, const Format& format) throw(FileException);


			void Put(const unsigned short& value, const Format& format) throw(FileException);


			void Put(const int& value, const Format& format) throw(FileException);


			void Put(const unsigned int& value, const Format& format) throw(FileException);


			void Put(const long& value, const Format& format) throw(FileException);


			void Put(const unsigned long& value, const Format& format) throw(FileException);


			void Put(const float& value, const Format& format) throw(FileException);


			void Put(const double& value, const Format& format) throw(FileException);


			void Put(const char* string) throw(FileException);


			void Read(unsigned char& character) throw(FileException);


			template <typename T>
			inline void Read(T* data, size_t count) throw(FileException)
			{
				Read(static_cast<void*>(data), count*sizeof(T));
			}


			void Read(void* data, size_t size) throw(FileException);


			void Skip(const char* characters = " \f\n\r\t\v") throw();


			void SkipComments() throw();


			sint64 Tell() throw(FileException);


			void Write(const unsigned char& character) throw(FileException);


			template <typename T>
			inline void Write(const T* data, size_t count) throw(FileException)
			{
				Write(static_cast<const void*>(data), count*sizeof(T));
			}


			void Write(const void* data, size_t size) throw(FileException);


		protected:

			void* file_stream;


		public:

			char comment_delimiter;
	};
}
