#include <io/file_stream.h>

namespace proto {
namespace io {

FileStream::FileStream(std::string filepath) : file_path_(filepath) {
  proto_file_stream_ = std::ifstream(file_path_);
  current_offset_ = -1;
  proto_file_stream_.seekg(0, std::ios::end);
  size_t size = proto_file_stream_.tellg();
  file_content_ = std::string(size, ' ');
  proto_file_stream_.seekg(0);
  proto_file_stream_.read(&file_content_[0], size);
}

char FileStream::peek(int64_t offset) {
  assert(offset >= 0);
  if (current_offset_ == -1 && offset == 0) {
    throw proto::util::TokenizerException("FileStream is not initiated");
  }
  if (current_offset_ + offset < file_content_.size()) {
    return file_content_[current_offset_ + offset];
  }
  throw proto::util::TokenizerException("Reach the end of the file stream");
}

char FileStream::next() {
  if (current_offset_ + 1 < file_content_.size()) {
    current_offset_++;
    return file_content_[current_offset_];
  }
  throw proto::util::TokenizerException("Reach the end of the file stream");
}

char FileStream::isEof() { return current_offset_ + 1 == file_content_.size(); }

SourceInfo FileStream::getSourceInfo(int start_offset, int end_offset) {
  return SourceInfo(file_path_, getLineno(start_offset), getPos(start_offset),
                    getLineno(end_offset), getPos(end_offset));
}

int64_t FileStream::getCurrentOffset() { return current_offset_; }

std::string FileStream::getSegment(int start_offset, int end_offset) {
  return file_content_.substr(start_offset, end_offset - start_offset + 1);
}

bool FileStream::fileExists(std::string filepath) {
  std::ifstream f(filepath);
  if (!f.is_open()) return false;
  f.close();
  return true;
}

int FileStream::getLineno(int offset) {
  int lineno = 1;
  for (int i = 0; i <= offset; i++) {
    if (file_content_[offset] == '\n') {
      lineno += 1;
    }
  }
  return lineno;
}

int FileStream::getPos(int offset) {
  int pos = 0;
  for (int i = offset; i >= 0; i--) {
    if (file_content_[i] != '\n') {
      pos += 1;
    } else {
      return pos;
    }
  }
  return pos;
}

}  // namespace io
}  // namespace proto