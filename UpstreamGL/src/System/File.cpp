#include <UpstreamGL/System/File.h>

#include <cstdio>

File::File()
	:m_fstream()
{
}

File::File(const std::string& path, int mode)
	: m_fstream()
{
	Open(path, mode);
}

void File::Open(const std::string& path, int mode)
{
	m_fstream.open(path, static_cast<std::ios_base::openmode>(mode));
	if (m_fstream.fail())
	{
		printf("Unable to open file: %s", path.c_str());
	}
}
#if WIN32
File::File(const std::wstring& path, int mode)
{
	Open(path, mode);
}

void File::Open(const std::wstring& path, int mode)
{
	m_fstream.open(path, static_cast<std::ios_base::openmode>(mode));
	if (m_fstream.fail())
	{
		printf("Unable to open file: %ls", path.c_str());
	}
}
#endif
File::~File()
{
	if (IsOpen())
		Close();
}

bool File::IsOpen() const
{
	return m_fstream.is_open();
}

size_t File::GetSize() const
{
	if (!IsOpen()) return 0;
	std::filebuf* buf = m_fstream.rdbuf();
	size_t size = buf->pubseekoff(0, m_fstream.end, m_fstream.in);
	buf->pubseekpos(0, m_fstream.in);
	return size;
}

void File::Clear()
{
	m_fstream.clear();
}

void File::Write(const std::stringstream& str)
{
	m_fstream << str.rdbuf();
}

void File::WriteStr(const std::string& str)
{
	if (!IsOpen()) return;
	m_fstream.write(str.c_str(), str.size());
}

void File::Close()
{
	m_fstream.close();
}

void File::Flush() 
{
	m_fstream.flush();
}

void File::Read(char* buffer, size_t count) const
{
	if (!IsOpen()) return;
	std::filebuf* buf = m_fstream.rdbuf();
	buf->sgetn(buffer, count);
}

std::string File::ToString()
{
	if (!IsOpen()) return "";
	std::filebuf* buf = m_fstream.rdbuf();
	std::string str;
	size_t size = GetSize();
	str.resize(size);
	buf->sgetn(&str[0], size);
	return str;
}