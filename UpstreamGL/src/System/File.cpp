#include <cstdio>
#include <sstream>

#include <UpstreamGL/System/File.h>

namespace upgl {
File::File() : m_fstream() {}

File::File(const std::string &path, int mode) : m_fstream() { Open(path, mode); }

void File::Open(const std::string &path, int mode) {
  m_fstream.open(path, static_cast<std::ios_base::openmode>(mode));
  if (m_fstream.fail()) {
    printf("Unable to open file: %s", path.c_str());
  }
}

File::~File() {
  if (IsOpen())
    Close();
}

bool File::IsOpen() const { return m_fstream.is_open(); }

size_t File::GetSize() const {
  if (!IsOpen())
    return 0;
  std::filebuf *buf = m_fstream.rdbuf();
  size_t size = buf->pubseekoff(0, m_fstream.end, m_fstream.in);
  buf->pubseekpos(0, m_fstream.in);
  return size;
}

void File::Clear() { m_fstream.clear(); }

void File::Write(const std::stringstream &str) { m_fstream << str.rdbuf(); }

void File::WriteStr(const std::string &str) {
  if (!IsOpen())
    return;
  m_fstream.write(str.c_str(), str.size());
}

void File::Close() { m_fstream.close(); }

void File::Flush() { m_fstream.flush(); }

void File::Read(char *buffer, size_t count) const {
  if (!IsOpen())
    return;
  std::filebuf *buf = m_fstream.rdbuf();
  buf->sgetn(buffer, count);
}

std::string File::Str() {
  if (!IsOpen())
    return "";
  std::stringstream str;
  str << m_fstream.rdbuf();
  return str.str();
}
} // namespace upgl