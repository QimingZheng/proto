#pragma once
#include <util/util.h>
#include <util/exceptions.h>
#include <compiler/descriptor/descriptor.h>
#include <compiler/parser/parser.h>

namespace proto_compiler {
namespace generator {

class CodeGenerator {
 public:
  CodeGenerator(std::string output_path) : output_path_(output_path) {}

  std::string generateInterface(
      proto_compiler::descriptor::MessageDescriptor* messageDescriptor);

  std::string generateInterface(
      proto_compiler::descriptor::FileDescriptor* fileDescriptor);

  std::string generateInterface(
      proto_compiler::descriptor::FieldDescriptor* fieldDescriptor);

  std::string generateImpl(
      proto_compiler::descriptor::MessageDescriptor* messageDescriptor);

  std::string generateImpl(
      proto_compiler::descriptor::FileDescriptor* fileDescriptor);

  std::string generateImpl(
      proto_compiler::descriptor::FieldDescriptor* fieldDescriptor);

 private:
  static std::string getEnclosingPrefix(
      proto_compiler::descriptor::Descriptor* descriptor);

  std::string output_path_;
  proto_compiler::descriptor::FileDescriptor* fileDescriptor;
};

}  // namespace generator
}  // namespace proto_compiler
