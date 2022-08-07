#pragma once
#include <util/util.h>

namespace proto {
namespace io {

class SourceInfo {
 public:
  SourceInfo(std::string filepath, int start_lineno, int start_pos,
             int end_lineno, int end_pos)
      : source_file_path_(filepath),
        start_lineno_(start_lineno),
        end_lineno_(end_lineno),
        start_pos_(start_pos),
        end_pos_(end_pos) {}

  std::string getSourceFilePath() { return source_file_path_; }
  int getStartLineno() { return start_lineno_; }
  int getStartPos() { return start_pos_; }
  int getEndLineno() { return end_lineno_; }
  int getEndPos() { return end_pos_; }

 private:
  std::string source_file_path_;
  int start_lineno_;
  int start_pos_;
  int end_lineno_;
  int end_pos_;
};

}  // namespace io
}  // namespace proto