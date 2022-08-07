#pragma once
#include <util/util.h>
#include <util/exceptions.h>
#include <io/source_info.h>

namespace proto {
namespace io {

class FileStream {
 public:
  FileStream(std::string filepath);

  // Get the next offset-th byte from the file stream without consuming any
  // byte. Throws exception when reach the end of the file.
  char peek(int64_t offset = 0);

  // Get the next byte from the file stream and proceed one byte forward. Throws
  // exception when reach the end of the file.
  char next();

  // Returns true when reach the end of the filestream.
  char isEof();

  // Return true when the file defined in filepath exists.
  static bool fileExists(std::string filepath);

  SourceInfo getSourceInfo(int start_offset, int end_offset);

  int64_t getCurrentOffset();

  std::string getSegment(int start_offset, int end_offset);

 private:
  int getLineno(int offset);
  int getPos(int offset);

  std::string file_path_;
  int64_t current_offset_;
  std::string file_content_;
  std::ifstream proto_file_stream_;
};

}  // namespace io
}  // namespace proto