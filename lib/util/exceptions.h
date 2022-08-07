#pragma once
#include <util/util.h>

namespace proto {
namespace util {

class TokenizerException : public std::exception {
 public:
  /** Constructor (C strings).
   *  @param message C-style string error message.
   *                 The string contents are copied upon construction.
   *                 Hence, responsibility for deleting the char* lies
   *                 with the caller.
   */
  explicit TokenizerException(const char* message) : msg_(message) {}

  /** Constructor (C++ STL strings).
   *  @param message The error message.
   */
  explicit TokenizerException(const std::string& message) : msg_(message) {}

  /** Destructor.
   * Virtual to allow for subclassing.
   */
  virtual ~TokenizerException() noexcept {}

  /** Returns a pointer to the (constant) error description.
   *  @return A pointer to a const char*. The underlying memory
   *          is in posession of the Exception object. Callers must
   *          not attempt to free the memory.
   */
  virtual const char* what() const noexcept { return msg_.c_str(); }

 protected:
  /** Error message.
   */
  std::string msg_;
};
class ParserException : public std::exception {
 public:
  explicit ParserException(const char* message) : msg_(message) {}

  explicit ParserException(const std::string& message) : msg_(message) {}

  virtual ~ParserException() noexcept {}

  virtual const char* what() const noexcept { return msg_.c_str(); }

 protected:
  // Error message.
  std::string msg_;
};
class DescriptorException : public std::exception {
 public:
  explicit DescriptorException(const char* message) : msg_(message) {}

  explicit DescriptorException(const std::string& message) : msg_(message) {}

  virtual ~DescriptorException() noexcept {}

  virtual const char* what() const noexcept { return msg_.c_str(); }

 protected:
  // Error message.
  std::string msg_;
};
}  // namespace util
}  // namespace proto