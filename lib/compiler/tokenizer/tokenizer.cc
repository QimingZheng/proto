#include <compiler/tokenizer/tokenizer.h>

namespace proto_compiler {
namespace tokenizer {

Token::Token(TokenType type, std::string lexem,
             proto::io::SourceInfo source_info)
    : type_(type), lexem_(lexem), source_info_(source_info) {}

TokenType Token::type() { return type_; }

std::string Token::lexeme() { return lexem_; }

Tokenizer::Tokenizer(proto::io::FileStream &file_stream)
    : file_stream_(file_stream) {}

std::vector<Token> Tokenizer::scan() {
  while (!file_stream_.isEof()) {
    skipEscapes();
    char ch = file_stream_.next();
    if (isAlpha(ch)) {
      tokens_.push_back(consumeIdentifier());
      continue;
    }

    if (isDigit(ch)) {
      tokens_.push_back(consumeFieldID());
      continue;
    }

    switch (ch) {
      case '{':
        tokens_.push_back(
            Token(TokenType::TOKEN_LEFT_BRACE, "{",
                  file_stream_.getSourceInfo(file_stream_.getCurrentOffset(),
                                             file_stream_.getCurrentOffset())));
        break;
      case '}':
        tokens_.push_back(
            Token(TokenType::TOKEN_RIGHT_BRACE, "}",
                  file_stream_.getSourceInfo(file_stream_.getCurrentOffset(),
                                             file_stream_.getCurrentOffset())));
        break;
      case ';':
        tokens_.push_back(
            Token(TokenType::TOKEN_SEMICOLON, ";",
                  file_stream_.getSourceInfo(file_stream_.getCurrentOffset(),
                                             file_stream_.getCurrentOffset())));

        break;
      case '=':
        tokens_.push_back(
            Token(TokenType::TOKEN_EQUAL, "=",
                  file_stream_.getSourceInfo(file_stream_.getCurrentOffset(),
                                             file_stream_.getCurrentOffset())));

        break;
      default:
        throw proto::util::TokenizerException(
            "Tokenizer meet unknow symbols: " + ch);
        break;
    }
  }
  return tokens_;
}

std::string Token::getTokenTypeString(TokenType type) {
  switch (type) {
    case TOKEN_MESSAGE:
      return "message";
    case TOKEN_STRING:
      return "string";
    case TOKEN_INT32:
      return "int32";
    case TOKEN_IDENTIFIER:
      return "identifier";
    case TOKEN_REPEATED:
      return "repeated";
    case TOKEN_MAP:
      return "map";
    case TOKEN_EQUAL:
      return "=";
    case TOKEN_SEMICOLON:
      return ";";
    case TOKEN_LEFT_BRACE:
      return "{";
    case TOKEN_RIGHT_BRACE:
      return "}";
    case TOKEN_FIELD_ID:
      return "<integer>";

    default:
      throw proto::util::TokenizerException("Unknown TokenType");
  }
}

Token Tokenizer::consumeFieldID() {
  int start_offset = file_stream_.getCurrentOffset();
  while (isDigit(file_stream_.peek(1))) {
    file_stream_.next();
  }
  int end_offset = file_stream_.getCurrentOffset();
  return Token(TOKEN_FIELD_ID,
               file_stream_.getSegment(start_offset, end_offset),
               file_stream_.getSourceInfo(start_offset, end_offset));
}

Token Tokenizer::consumeIdentifier() {
  int start_offset = file_stream_.getCurrentOffset();
  while (isDigit(file_stream_.peek(1)) || isAlpha(file_stream_.peek(1))) {
    file_stream_.next();
  }
  int end_offset = file_stream_.getCurrentOffset();
  std::string lexeme = file_stream_.getSegment(start_offset, end_offset);
  proto::io::SourceInfo sourceInfo =
      file_stream_.getSourceInfo(start_offset, end_offset);
  if (lexeme == "string") {
    return Token(TOKEN_STRING, lexeme, sourceInfo);
  } else if (lexeme == "int32") {
    return Token(TOKEN_INT32, lexeme, sourceInfo);
  } else if (lexeme == "message") {
    return Token(TOKEN_MESSAGE, lexeme, sourceInfo);
  } else if (lexeme == "repeated") {
    return Token(TOKEN_REPEATED, lexeme, sourceInfo);
  } else {
    return Token(TOKEN_IDENTIFIER, lexeme, sourceInfo);
  }
}

bool Tokenizer::isEscape(char s) {
  return s == '\n' || s == ' ' || s == '\t' || s == '\r';
}

bool Tokenizer::isDigit(char s) { return s >= '0' && s <= '9'; }

bool Tokenizer::isAlpha(char s) {
  return (s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z');
}

void Tokenizer::skipEscapes() {
  while (isEscape(file_stream_.peek(1))) {
    file_stream_.next();
  }
}

}  // namespace tokenizer
}  // namespace proto_compiler