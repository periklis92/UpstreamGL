#pragma once

#include <string>
#include <fstream>
#include <sstream>

class File
{
public:
	enum FileMode
	{
		Input = std::ios_base::in,
		Output = std::ios_base::out,
		Binary = std::ios_base::binary,
		Append = std::ios_base::app
	};

public:
	File();
	explicit File(const std::string& path, int mode = Input | Output);
	void Open(const std::string& path, int mode = Input);
#if WIN32
	explicit File(const std::wstring& path, int mode = Input);
	void Open(const std::wstring& path, int mode = Input);
#endif
	~File();

	bool IsOpen() const;
	size_t GetSize() const;
	void Clear();
	void Write(const std::stringstream& str);
	void WriteStr(const std::string& str);
	void Close();
	void Flush();
	void Read(char* buffer, size_t count) const;

	std::string ToString();

	inline File& operator<<(const std::stringstream& str)
	{
		Write(str);
		return *this;
	}

	inline File& operator<<(const std::string& str)
	{
		WriteStr(str);
		return *this;
	}

private:
	std::fstream m_fstream;
};