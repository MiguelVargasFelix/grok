// File.cpp
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
#include <Basic/File.h>
#include <Basic/Format.h>
#include <Basic/Memory.h>
#include <Basic/System.h>

#include <stdio.h>


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
	#include <fcntl.h>
	#include <io.h>

	#ifndef fseeko
		#define fseeko _fseeki64
	#endif
	#ifndef ftello
		#define ftello _ftelli64
	#endif

#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_FreeBSD) || defined(OS_Linux)

	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>

#endif


namespace Grok
{
	#if defined(CC_Microsoft)
		#pragma warning(push)
		#pragma warning(disable: 4996) // This function or variable may be unsafe
	#elif defined(CC_Clang)
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wformat-nonliteral" // format string is not a string literal
		#pragma clang diagnostic ignored "-Wformat-security" // Format string is not a string literal (potentially insecure)
	#endif

	File::File() throw()
	:	file_stream(static_cast<void*>(0)),
		comment_delimiter('#')
	{
	}


	File::File(const File& file) throw()
	:	file_stream(file.file_stream),
		comment_delimiter(file.comment_delimiter)
	{
	}


	File::~File() throw()
	{
		Assert(!file_stream);
	}


	File& File::operator = (const File& file) throw()
	{
		file_stream = file.file_stream;
		comment_delimiter = file.comment_delimiter;
		return *this;
	}


	void File::Close() throw()
	{
		Assert(file_stream);

		fclose(reinterpret_cast<FILE*>(file_stream));
		file_stream = static_cast<void*>(0);
	}


	void File::Create(const char* file_name) throw(FileException)
	{
		Assert(file_name);

		file_stream = fopen(file_name, "wb");
		if (!file_stream)
		{
			Throw(FileException(FileException::create_error));
		}
	}


	void File::CreatePipe(const char* pipe_name, bool read, bool write) throw(FileException)
	{
		Assert(pipe_name);
		Assert(read || write);

		#if defined(OS_Windows)

			#define PIPE_BUFFER_SIZE 655360
			#define PIPE_TIMEOUT 3600000 // 3600 seconds

			DWORD open_mode = FILE_FLAG_FIRST_PIPE_INSTANCE | ((!write) ? PIPE_ACCESS_INBOUND : (!read) ? PIPE_ACCESS_OUTBOUND : PIPE_ACCESS_DUPLEX);
			DWORD pipe_mode =  PIPE_WAIT | ((!write) ? PIPE_READMODE_BYTE : (!read) ? PIPE_TYPE_BYTE : PIPE_READMODE_BYTE | PIPE_TYPE_BYTE);
			HANDLE pipe_handle = CreateNamedPipe(pipe_name, open_mode, pipe_mode, PIPE_UNLIMITED_INSTANCES, PIPE_BUFFER_SIZE, PIPE_BUFFER_SIZE, PIPE_TIMEOUT, NULL);
			if (pipe_handle == INVALID_HANDLE_VALUE)
			{
				Throw(FileException(FileException::create_error));
			}
			if (ConnectNamedPipe(pipe_handle, NULL) == 0)
			{
				Throw(FileException(FileException::create_error));
			}

			int flags = (!write) ? _O_RDONLY : 0;
			int file_descriptor = _open_osfhandle((intptr_t)pipe_handle, flags);
			if (file_descriptor == -1)
			{
				Throw(FileException(FileException::create_error));
			}

			const char* stream_mode = (!write) ? "rb" : (!read) ? "wb" : "w+b";
			file_stream = _fdopen(file_descriptor, stream_mode);
			if (!file_stream)
			{
				Throw(FileException(FileException::open_error));
			}

		#elif defined(OS_MacOSX) || defined(OS_Cygwin) || defined(OS_FreeBSD) || defined(OS_Linux)

			unlink(pipe_name);
			if (mkfifo(pipe_name, 0600) == -1)
			{
				Throw(FileException(FileException::create_error));
			}
			const char* stream_mode = (!write) ? "rb" : (!read) ? "wb" : "w+b";
			file_stream = fopen(pipe_name, stream_mode);
			if (!file_stream)
			{
				Throw(FileException(FileException::open_error));
			}

		#endif
	}


	void File::Flush() throw(FileException)
	{
		Assert(file_stream);

		if (fflush(reinterpret_cast<FILE*>(file_stream)) != 0)
		{
			Throw(FileException(FileException::flush_error));
		}
	}


	void File::Get(bool& value) throw(FileException)
	{
		Assert(file_stream);

		int integer_value;
		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%i", &integer_value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
		value = (integer_value != 0);
	}


	void File::Get(unsigned char& value) throw(FileException)
	{
		unsigned int integer_value;
		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%u", &integer_value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
		value = static_cast<unsigned char>(integer_value);
	}


	void File::Get(short& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%hi", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(unsigned short& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%hu", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(int& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%i", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(unsigned int& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%u", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(long& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%li", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(unsigned long& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%lu", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(float& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%f", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(double& value) throw(FileException)
	{
		Assert(file_stream);

		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%lf", &value) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(char* string, int max_size, const char* separators) throw(FileException)
	{
		Assert(file_stream);
		Assert(string);
		Assert(separators);

		char* format = Alloca(char, Length(separators) + 32);
		sprintf(format, "%%%i[^%s]", max_size - 1, separators);
		if (fscanf(reinterpret_cast<FILE*>(file_stream), format, string) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Get(String& string, int max_size, const char* separators) throw(FileException)
	{
		Assert(file_stream);
		Assert(max_size > 0);

		char* __restrict tmp = (max_size <= STRING_DEFAULT_MAXIMUM_SIZE) ? Alloca(char, max_size) : new char[max_size];
		if (!tmp)
		{
			Throw(MemoryException());
		}

		char* format = Alloca(char, Length(separators) + 32);
		sprintf(format, "%%%i[^%s]", max_size - 1, separators);
		if (fscanf(reinterpret_cast<FILE*>(file_stream), format, tmp) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}

		string.Set(tmp);

		if (max_size > STRING_DEFAULT_MAXIMUM_SIZE)
		{
			delete [] tmp;
		}
	}


	void File::GetLine(char* string, int max_size) throw(FileException)
	{
		Assert(file_stream);
		Assert(string);

		char format[32];
		sprintf(format, "%%%i[^\r\n]", max_size - 1);
		if (fscanf(reinterpret_cast<FILE*>(file_stream), format, string) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
		fscanf(reinterpret_cast<FILE*>(file_stream), "%*1[\r]");
		fscanf(reinterpret_cast<FILE*>(file_stream), "%*1[\n]");
	}


	void File::GetLine(String& string, int max_size) throw(FileException)
	{
		Assert(file_stream);
		Assert(max_size > 0);

		char* __restrict tmp = (max_size <= STRING_DEFAULT_MAXIMUM_SIZE) ? Alloca(char, max_size) : new char[max_size];
		if (!tmp)
		{
			Throw(MemoryException());
		}

		char format[32];
		sprintf(format, "%%%i[^\r\n]", max_size - 1);
		if (fscanf(reinterpret_cast<FILE*>(file_stream), format, tmp) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
		fscanf(reinterpret_cast<FILE*>(file_stream), "%*1[\r]");
		fscanf(reinterpret_cast<FILE*>(file_stream), "%*1[\n]");

		string.Set(tmp);

		if (max_size > STRING_DEFAULT_MAXIMUM_SIZE)
		{
			delete [] tmp;
		}
	}


	void File::GoBegin(sint64 offset_bytes) throw(FileException)
	{
		Assert(file_stream);

		if (fseeko(reinterpret_cast<FILE*>(file_stream), offset_bytes, SEEK_SET))
		{
			Throw(FileException(FileException::read_error));
		}
	}


	void File::GoEnd(sint64 offset_bytes) throw(FileException)
	{
		Assert(file_stream);

		if (fseeko(reinterpret_cast<FILE*>(file_stream), offset_bytes, SEEK_END))
		{
			Throw(FileException(FileException::seek_error));
		}
	}


	void File::GoTo(sint64 offset_bytes) throw(FileException)
	{
		Assert(file_stream);

		if (fseeko(reinterpret_cast<FILE*>(file_stream), offset_bytes, SEEK_CUR))
		{
			Throw(FileException(FileException::seek_error));
		}
	}


	void File::Open(const char* file_name) throw(FileException)
	{
		file_stream = static_cast<void*>(fopen(file_name, "rb"));
		if (!file_stream)
		{
			Throw(FileException(FileException::open_error));
		}
	}


	void File::Put(const bool& value) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), "%i", value ? 1 : 0) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const unsigned char& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const short& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const unsigned short& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const int& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const unsigned int& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const long& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const unsigned long& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const float& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const double& value, const Format& format) throw(FileException)
	{
		Assert(file_stream);

		if (fprintf(reinterpret_cast<FILE*>(file_stream), format.definition, value) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Put(const char* string) throw(FileException)
	{
		Assert(string);

		if (fputs(string, reinterpret_cast<FILE*>(file_stream)) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Read(unsigned char& character) throw(FileException)
	{
		Assert(file_stream);

		int integer_char = fgetc(reinterpret_cast<FILE*>(file_stream));
		if (integer_char == EOF)
		{
			Throw(FileException(FileException::eof_error));
		}
		character = static_cast<unsigned char>(integer_char);
	}


	void File::Read(void* data, size_t size) throw(FileException)
	{
		Assert(file_stream);
		Assert(data);

		if (fread(data, size, 1, reinterpret_cast<FILE*>(file_stream)) != 1)
		{
			if (feof(reinterpret_cast<FILE*>(file_stream)))
			{
				Throw(FileException(FileException::eof_error));
			}
			else
			{
				Throw(FileException(FileException::read_error));
			}
		}
	}


	void File::Skip(const char* characters) throw()
	{
		char* format = Alloca(char, Length(characters) + 8);
		sprintf(format, "%%*[%s]", characters);
		fscanf(reinterpret_cast<FILE*>(file_stream), format);
	}


	void File::SkipComments() throw()
	{
		Assert(file_stream);

		char format[6] = "%1[ ]";
		format[3] = comment_delimiter;

		char comment_start[2];
		if (fscanf(reinterpret_cast<FILE*>(file_stream), "%*[ \t\r\n]") == EOF)
		{
			return;
		}
		while (fscanf(reinterpret_cast<FILE*>(file_stream), format, comment_start) == 1)
		{
			if (fscanf(reinterpret_cast<FILE*>(file_stream), "%*[^\r\n]%*[\r\n]") == EOF)
			{
				return;
			}
		}
	}


	sint64 File::Tell() throw(FileException)
	{
		Assert(file_stream);

		sint64 offset = ftello(reinterpret_cast<FILE*>(file_stream));
		if (offset == -1)
		{
			Throw(FileException(FileException::tell_error));
		}
		return offset;
	}


	void File::Write(const unsigned char& character) throw(FileException)
	{
		if (fputc(character, reinterpret_cast<FILE*>(file_stream)) < 0)
		{
			Throw(FileException(FileException::write_error));
		}
	}


	void File::Write(const void* data, size_t size) throw(FileException)
	{
		Assert(file_stream);
		Assert(data);

		if (fwrite(data, size, 1, reinterpret_cast<FILE*>(file_stream)) != 1)
		{
			Throw(FileException(FileException::write_error));
		}
	}

	#if defined(CC_Microsoft)
		#pragma warning(pop)
	#elif defined(CC_Clang)
		#pragma clang diagnostic pop
	#endif
}
