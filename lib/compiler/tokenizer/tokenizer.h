#pragma once
#include <util/util.h>
#include <io/file_stream.h>
#include <util/exceptions.h>

namespace proto_compiler {
namespace tokenizer {

enum TokenType {
  TOKEN_MESSAGE,
  TOKEN_STRING,
  TOKEN_INT32,
  TOKEN_IDENTIFIER,
  TOKEN_REPEATED,
  TOKEN_MAP,
  TOKEN_EQUAL,
  TOKEN_SEMICOLON,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_FIELD_ID,
};

class Token {
 public:
  Token(TokenType type, std::string lexem, proto::io::SourceInfo source_info);

  TokenType type();

  std::string lexeme();

  static std::string getTokenTypeString(TokenType type);

 private:
  TokenType type_;
  std::string lexem_;
  proto::io::SourceInfo source_info_;
};

class Tokenizer {
 public:
  Tokenizer(proto::io::FileStream &file_stream);

  std::vector<Token> scan();

  Token consumeIdentifier();

  Token consumeFieldID();

 private:
  bool isEscape(char s);

  bool isDigit(char s);

  bool isAlpha(char s);

  void skipEscapes();

  proto::io::FileStream &file_stream_;

  std::vector<Token> tokens_;
};
}  // namespace tokenizer
}  // namespace proto_compiler