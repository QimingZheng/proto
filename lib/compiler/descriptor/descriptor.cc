#include <compiler/descriptor/descriptor.h>

namespace proto_compiler {

namespace descriptor {

/// Descriptor

Descriptor::Descriptor() {
  type_ = UNSPECIFIED;
  type_name_ = "unspecified";
  enclosing_descriptor_ = nullptr;
}

Descriptor* Descriptor::getEnclosingDescriptor() {
  return enclosing_descriptor_;
}

void Descriptor::setEnclosingDescriptor(Descriptor* descriptor) {
  enclosing_descriptor_ = descriptor;
}

std::string Descriptor::debugString() { return "a descriptor"; }

/// MessageDescriptor

MessageDescriptor::MessageDescriptor() {
  field_descriptors_.clear();
  message_descriptors_.clear();
}

MessageDescriptor::~MessageDescriptor() {}

DescriptorType MessageDescriptor::type() { return MESSAGE; }

std::string MessageDescriptor::typeName() { return type_name_; }

void MessageDescriptor::setTypeName(std::string type_name) {
  type_name_ = type_name;
}

std::vector<FieldDescriptor*> MessageDescriptor::getFieldDescriptors() {
  return field_descriptors_;
}

std::vector<MessageDescriptor*> MessageDescriptor::getMessageDescriptors() {
  return message_descriptors_;
}

void MessageDescriptor::addMessageDescriptor(
    MessageDescriptor* messageDescriptor) {
  message_descriptors_.push_back(messageDescriptor);
}

void MessageDescriptor::addFieldDescriptor(FieldDescriptor* fieldDescriptor) {
  field_descriptors_.push_back(fieldDescriptor);
}

std::string MessageDescriptor::debugString() {
  std::string debug = "meeesage " + typeName() + ":\n";
  auto messages = getMessageDescriptors();
  for (auto& message : messages) {
    debug += message->debugString() + '\n';
  }
  auto fields = getFieldDescriptors();
  for (auto& field : fields) {
    debug += field->debugString() + '\n';
  }
  return debug;
}
/// FieldDescriptor

FieldDescriptor::FieldDescriptor() { message_descriptor_ = nullptr; }
FieldDescriptor::~FieldDescriptor() {}

DescriptorType FieldDescriptor::type() { return type_; }

void FieldDescriptor::setType(DescriptorType type) { type_ = type; }

void FieldDescriptor::setTypeName(std::string type_name) {
  type_name_ = type_name;
}

void FieldDescriptor::setFieldIndex(int field_index) {
  field_index_ = field_index;
}

void FieldDescriptor::setMessageDescriptor(
    MessageDescriptor* messageDescriptor) {
  message_descriptor_ = messageDescriptor;
}

void FieldDescriptor::setName(std::string name) { name_ = name; }

std::string FieldDescriptor::typeName() { return type_name_; }

bool FieldDescriptor::isRepeated() { return is_repeated_; }

std::string FieldDescriptor::name() { return name_; }

int FieldDescriptor::fieldIndex() { return field_index_; }

MessageDescriptor* FieldDescriptor::getMessageDescriptor() {
  return message_descriptor_;
}

void FieldDescriptor::setIsRepeated(bool value) { is_repeated_ = value; }

std::string FieldDescriptor::debugString() {
  std::string debug = "field " + name() +
                      (getMessageDescriptor() == nullptr
                           ? ""
                           : " type " + getMessageDescriptor()->typeName());
  return debug;
}

/// File Descriptor

DescriptorType FileDescriptor::type() { return FILE; }

std::string FileDescriptor::typeName() { return "namespace"; }

std::vector<MessageDescriptor*> FileDescriptor::getMessageDescriptors() {
  return message_descriptors_;
}

Descriptor* FileDescriptor::getEnclosingDescriptor() { return nullptr; }

void FileDescriptor::addMessageDescriptor(
    MessageDescriptor* messageDescriptor) {
  message_descriptors_.push_back(messageDescriptor);
}

FileDescriptor::FileDescriptor() { message_descriptors_.clear(); }
FileDescriptor::~FileDescriptor() {}

std::string FileDescriptor::debugString() {
  std::string debug = "";
  auto messages = getMessageDescriptors();
  for (auto& message : messages) {
    debug += message->debugString() + '\n';
  }
  return debug;
}

}  // namespace descriptor
}  // namespace proto_compiler