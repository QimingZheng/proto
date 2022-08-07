#include <compiler/generator/code_generator.h>

namespace proto_compiler {
namespace generator {

std::string CodeGenerator::generateInterface(
    proto_compiler::descriptor::MessageDescriptor* messageDescriptor) {
  std::string code = "";
  code += "class " + messageDescriptor->typeName() + "{";
  for (auto& message : messageDescriptor->getMessageDescriptors()) {
    code += "class " + message->typeName() + ";";
  }
  for (auto& message : messageDescriptor->getMessageDescriptors()) {
    code += generateInterface(message);
  }
  for (auto& field : messageDescriptor->getFieldDescriptors()) {
    code += generateInterface(field);
  }
  code += "std::vector<byte> toBuffer();";
  code += messageDescriptor->typeName() + " fromBuffer(std::vector<byte>);";
  code += "};";
  return code;
}

std::string CodeGenerator::generateInterface(
    proto_compiler::descriptor::FileDescriptor* fileDescriptor) {
  std::string code = "";
  for (auto& message : fileDescriptor->getMessageDescriptors()) {
    code += "class " + message->typeName() + ";";
  }
  for (auto& message : fileDescriptor->getMessageDescriptors()) {
    code += generateInterface(message);
  }
  return code;
}

std::string CodeGenerator::generateInterface(
    proto_compiler::descriptor::FieldDescriptor* fieldDescriptor) {
  std::string code = "";
  code += "private: ";
  switch (fieldDescriptor->type()) {
    case proto_compiler::descriptor::INT32:
      code += "int32_t ";
      break;

    case proto_compiler::descriptor::STRING:
      code += "std::string ";
      break;

    case proto_compiler::descriptor::MESSAGE:
      code += fieldDescriptor->typeName() + " ";
      break;

    default:
      throw proto::util::ParserException("Unknown field type");
  }

  code += fieldDescriptor->name() + ";";

  code += "private: bool has" + fieldDescriptor->name() + "_;";

  code += "public: bool has" + fieldDescriptor->name() + "();";
  code += "public: " + fieldDescriptor->typeName() + " get" +
          fieldDescriptor->name() + "();";
  code += "public: void set" + fieldDescriptor->name() + "(" +
          fieldDescriptor->typeName() + ");";

  return code;
}

std::string CodeGenerator::generateImpl(
    proto_compiler::descriptor::MessageDescriptor* messageDescriptor) {
  std::string code = "";

  for (auto& message : messageDescriptor->getMessageDescriptors()) {
    code += generateImpl(message);
  }

  for (auto& field : messageDescriptor->getFieldDescriptors()) {
    code += generateImpl(field);
  }

  return code;
}

std::string CodeGenerator::generateImpl(
    proto_compiler::descriptor::FieldDescriptor* fieldDescriptor) {
  std::string code = "";

  std::string prefix = getEnclosingPrefix(fieldDescriptor);

  code += "bool " + prefix + "has" + fieldDescriptor->name() + " () { return has" +
          fieldDescriptor->name() + "_ ; }";

  code += "void " + prefix + "set" + fieldDescriptor->name() + "(" +
          fieldDescriptor->typeName() + " " + fieldDescriptor->name() +
          ") { this->" + fieldDescriptor->name() + " = " +
          fieldDescriptor->name() + "; this->has" + fieldDescriptor->name() +
          "_ = true; }";

  if (fieldDescriptor->type() == proto_compiler::descriptor::MESSAGE) {
    code += getEnclosingPrefix(fieldDescriptor->getMessageDescriptor());
  }

  code += fieldDescriptor->typeName() + " " + prefix + "get" +
          fieldDescriptor->name() + "() { return " + fieldDescriptor->name() +
          ";}";

  return code;
}

std::string CodeGenerator::generateImpl(
    proto_compiler::descriptor::FileDescriptor* fileDescriptor) {
  std::string code = "";

  for (auto& message : fileDescriptor->getMessageDescriptors()) {
    code += generateImpl(message);
  }

  return code;
}

std::string CodeGenerator::getEnclosingPrefix(
    proto_compiler::descriptor::Descriptor* descriptor) {
  assert(descriptor != nullptr);
  std::string prefix = "";
  std::vector<proto_compiler::descriptor::Descriptor*> enclosing_descriptors;
  while (descriptor->getEnclosingDescriptor() != nullptr) {
    enclosing_descriptors.push_back(descriptor->getEnclosingDescriptor());
    descriptor = descriptor->getEnclosingDescriptor();
  }
  std::reverse(enclosing_descriptors.begin(), enclosing_descriptors.end());
  for (auto& enclosing : enclosing_descriptors) {
    prefix += enclosing->typeName() + "::";
  }
  return prefix;
}

}  // namespace generator

}  // namespace proto_compiler