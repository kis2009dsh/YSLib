﻿/*
	© 2012-2013 FrankHB.

	This file is part of the YSLib project, and may only be used,
	modified, and distributed under the terms of the YSLib project
	license, LICENSE.TXT.  By continuing to use, modify, or distribute
	this file you indicate that you have read the license and
	understand and accept it fully.
*/

/*!	\file MemoryMapping.cpp
\ingroup YCLib
\brief 内存映射文件。
\version r145
\author FrankHB <frankhb1989@gmail.com>
\since build 324
\par 创建时间:
	2012-07-11 21:59:21 +0800
\par 修改时间:
	2013-10-01 01:37 +0800
\par 文本编码:
	UTF-8
\par 模块名称:
	YCLib::MemoryMapping
*/


#include "YCLib/MemoryMapping.h"
#include "YCLib/FileSystem.h" // for platform::uopen;
#include <fcntl.h>
#include <stdexcept> // for std::runtime_error;
#if YCL_DS
#	include <unistd.h>
#elif YCL_MinGW32
#	include <Windows.h>
#	include <sys/stat.h>

namespace
{

#define MAP_FAILED (reinterpret_cast<void*>(-1))

void*
map_file(size_t len, int fd)
{
	void* p_mapped{};

	if(len != 0)
	{
		::HANDLE h(::HANDLE(::_get_osfhandle(fd)));

		if(h != INVALID_HANDLE_VALUE)
			if(::HANDLE fm = ::CreateFileMapping(h, nullptr, PAGE_READONLY, 0,
				len, nullptr))
			{
				p_mapped = ::MapViewOfFile(fm, FILE_MAP_READ, 0, 0, len);
				::CloseHandle(fm);
			}
	}
	return p_mapped ? p_mapped : MAP_FAILED;
}

} // unnamed namespace;

#endif

namespace platform
{

MappedFile::MappedFile(const char* path)
	: fd(uopen(path, O_RDONLY, S_IRUSR | S_IWUSR))
{
	::fstat(fd, &st);
#if YCL_DS
	addr = new ystdex::byte[st.st_size];

	::read(fd, addr, st.st_size);
#elif YCL_MinGW32
//	const auto p(::mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0)));
	const auto p(map_file(st.st_size, fd));

	if(p == MAP_FAILED)
		throw std::runtime_error("Mapping failed.");
	// TODO: Create specific exception type.
	addr = static_cast<ystdex::byte*>(p);
#else
#	error "Unsupported platform found."
#endif
}
MappedFile::~MappedFile()
{
#if YCL_DS
	delete addr;
#elif YCL_MinGW32
//	::munmap(addr, st.st_size);
	::UnmapViewOfFile(addr);
#endif
	::close(fd);
}

size_t
MappedFile::GetSize() const
{
	return st.st_size;
}

} // namespace platform;

