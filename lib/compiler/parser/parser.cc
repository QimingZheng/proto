#include <compiler/parser/parser.h>

namespace proto_compiler {

namespace parser {
Parser::Parser(std::vector<proto_compiler::tokenizer::Token> token_stream)
    : token_stream_(token_stream) {
  token_index_ = 0;
}

proto_compiler::descriptor::FileDescriptor* Parser::getFileDescriptor() {
  proto_compiler::descriptor::FileDescriptor* fileDescriptor =
      new proto_compiler::descriptor::FileDescriptor();
  while (token_index_ != token_stream_.size()) {
    // begin of a new message definition
    if (token_stream_[token_index_].type() ==
        proto_compiler::tokenizer::TOKEN_MESSAGE) {
      fileDescriptor->addMessageDescriptor(getMessageDescriptor());
    } else {
      throw proto::util::ParserException(
          "Parser meet unexpected token, only message are allowed in "
          "top-level");
    }
  }

  std::vector<proto_compiler::descriptor::MessageDescriptor*> messages =
      fileDescriptor->getMessageDescriptors();

  for (auto& message : fileDescriptor->getMessageDescriptors()) {
    amendDescriptorTree(messages, message);
  }

  return fileDescriptor;
}

proto_compiler::descriptor::MessageDescriptor* Parser::getMessageDescriptor() {
  proto_compiler::descriptor::MessageDescriptor* messageDescriptor =
      new proto_compiler::descriptor::MessageDescriptor();
  token_index_ += 1;
  expectTokenType(proto_compiler::tokenizer::TOKEN_IDENTIFIER,
                  token_stream_[token_index_]);
  messageDescriptor->setTypeName(token_stream_[token_index_].lexeme());
  token_index_ += 1;
  expectTokenType(proto_compiler::tokenizer::TOKEN_LEFT_BRACE,
                  token_stream_[token_index_]);
  token_index_ += 1;
  while (token_stream_[token_index_].type() !=
         proto_compiler::tokenizer::TOKEN_RIGHT_BRACE) {
    // begin of a new message definition
    if (token_stream_[token_index_].type() ==
        proto_compiler::tokenizer::TOKEN_MESSAGE) {
      messageDescriptor->addMessageDescriptor(getMessageDescriptor());
    }
    // begin of a new field
    else {
      messageDescriptor->addFieldDescriptor(getFieldDescriptor());
    }
  }
  for (auto& message : messageDescriptor->getMessageDescriptors()) {
    message->setEnclosingDescriptor(messageDescriptor);
  }
  for (auto& field : messageDescriptor->getFieldDescriptors()) {
    field->setEnclosingDescriptor(messageDescriptor);
  }
  token_index_ += 1;
  return messageDescriptor;
}

proto_compiler::descriptor::FieldDescriptor* Parser::getFieldDescriptor() {
  proto_compiler::descriptor::FieldDescriptor* fieldDescriptor =
      new proto_compiler::descriptor::FieldDescriptor();

  proto_compiler::tokenizer::Token tk = token_stream_[token_index_];
  if (tk.type() == proto_compiler::tokenizer::TOKEN_REPEATED) {
    fieldDescriptor->setIsRepeated(true);
    tk = token_stream_[++token_index_];
  }
  switch (tk.type()) {
    case proto_compiler::tokenizer::TOKEN_INT32:
      fieldDescriptor->setType(proto_compiler::descriptor::INT32);
      fieldDescriptor->setTypeName(
          proto_compiler::tokenizer::Token::getTokenTypeString(
              proto_compiler::tokenizer::TOKEN_INT32));
      break;
    case proto_compiler::tokenizer::TOKEN_STRING:
      fieldDescriptor->setType(proto_compiler::descriptor::STRING);
      fieldDescriptor->setTypeName(
          proto_compiler::tokenizer::Token::getTokenTypeString(
              proto_compiler::tokenizer::TOKEN_STRING));
      break;
    case proto_compiler::tokenizer::TOKEN_IDENTIFIER:
      fieldDescriptor->setType(proto_compiler::descriptor::MESSAGE);
      fieldDescriptor->setTypeName(tk.lexeme());
      break;
    default:
      throw proto::util::ParserException("Parser expect type");
  }
  tk = token_stream_[++token_index_];
  expectTokenType(proto_compiler::tokenizer::TOKEN_IDENTIFIER, tk);
  fieldDescriptor->setName(tk.lexeme());
  tk = token_stream_[++token_index_];
  expectTokenType(proto_compiler::tokenizer::TOKEN_EQUAL, tk);
  tk = token_stream_[++token_index_];
  expectTokenType(proto_compiler::tokenizer::TOKEN_FIELD_ID, tk);
  fieldDescriptor->setFieldIndex(atoi(tk.lexeme().c_str()));
  tk = token_stream_[++token_index_];
  expectTokenType(proto_compiler::tokenizer::TOKEN_SEMICOLON, tk);
  token_index_++;
  return fieldDescriptor;
}

void Parser::amendDescriptorTree(
    std::vector<proto_compiler::descriptor::MessageDescriptor*> messages,
    proto_compiler::descriptor::MessageDescriptor* descriptor) {
  std::vector<proto_compiler::descriptor::FieldDescriptor*> fields =
      descriptor->getFieldDescriptors();

  for (auto& field : fields) {
    if (field->type() == proto_compiler::descriptor::MESSAGE) {
      int found = 0;
      for (auto& message : messages) {
        assert(message != nullptr);
        assert(field != nullptr);
        if (message->typeName() == field->typeName()) {
          field->setMessageDescriptor(message);
          found += 1;
        }
      }
      if (found == 0) {
        throw proto::util::ParserException(
            "Parser Error: Message definition not found");
      }
      if (found > 1) {
        throw proto::util::ParserException(
            "Parser Error: Found more than one message definition match");
      }
    }
  }

  for (auto& message : descriptor->getMessageDescriptors()) {
    messages.push_back(message);
  }

  for (auto& message : descriptor->getMessageDescriptors()) {
    amendDescriptorTree(messages, message);
  }
}

void Parser::expectTokenType(proto_compiler::tokenizer::TokenType token_type,
                             proto_compiler::tokenizer::Token token) {
  if (token.type() != token_type) {
    throw proto::util::ParserException(
        "Parse Error: Expect token of type " +
        proto_compiler::tokenizer::Token::getTokenTypeString(token_type) +
        " actually get " +
        proto_compiler::tokenizer::Token::getTokenTypeString(token.type()));
  }
}

}  // namespace parser
}  // namespace proto_compiler