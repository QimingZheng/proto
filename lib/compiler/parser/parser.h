#pragma once
#include <util/util.h>
#include <compiler/tokenizer/tokenizer.h>
#include <compiler/descriptor/descriptor.h>
#include <util/exceptions.h>

namespace proto_compiler {
namespace parser {

class Parser {
 public:
  Parser(std::vector<proto_compiler::tokenizer::Token> token_stream);

  proto_compiler::descriptor::FileDescriptor* getFileDescriptor();

  proto_compiler::descriptor::MessageDescriptor* getMessageDescriptor();

  proto_compiler::descriptor::FieldDescriptor* getFieldDescriptor();

 private:
  void expectTokenType(proto_compiler::tokenizer::TokenType token_type,
                       proto_compiler::tokenizer::Token token);

  void amendDescriptorTree(
      std::vector<proto_compiler::descriptor::MessageDescriptor*> messages,
      proto_compiler::descriptor::MessageDescriptor* descriptor);

  std::vector<proto_compiler::tokenizer::Token> token_stream_;
  int token_index_;
  std::vector<proto_compiler::descriptor::Descriptor*> parsing_context_;
};

}  // namespace parser
}  // namespace proto_compiler