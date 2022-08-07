#pragma once

#include <util/util.h>
#include <util/exceptions.h>

namespace proto_compiler {

namespace descriptor {

enum DescriptorType {
  INT32,
  STRING,
  MESSAGE,
  FILE,
  UNSPECIFIED,
};

class Descriptor {
 public:
  Descriptor();

  virtual ~Descriptor() {}

  virtual DescriptorType type() {
    throw proto::util::DescriptorException(
        "Deescriptor.type() not implemented");
  }

  virtual std::string typeName() {
    throw proto::util::DescriptorException(
        "Deescriptor.typeName() not implemented");
  }

  virtual Descriptor* getEnclosingDescriptor();

  virtual std::string debugString();

  virtual void setEnclosingDescriptor(Descriptor*);

 protected:
  DescriptorType type_;
  std::string type_name_;
  Descriptor* enclosing_descriptor_;
};

class MessageDescriptor;

class FieldDescriptor : public Descriptor {
 public:
  FieldDescriptor();
  ~FieldDescriptor();
  DescriptorType type() override;

  std::string typeName() override;

  std::string name();

  int fieldIndex();

  bool isRepeated();

  void setIsRepeated(bool value);

  void setType(DescriptorType type);

  void setTypeName(std::string type_name);

  void setName(std::string name);

  void setFieldIndex(int field_index);

  void setMessageDescriptor(MessageDescriptor* messageDescriptor);

  MessageDescriptor* getMessageDescriptor();

  std::string debugString() override;

 private:
  std::string name_;
  bool is_repeated_;
  int field_index_;
  MessageDescriptor* message_descriptor_;
};

class MessageDescriptor : public Descriptor {
 public:
  MessageDescriptor();
  ~MessageDescriptor();

  DescriptorType type() override;

  std::string typeName() override;

  void setTypeName(std::string type_name);

  std::vector<FieldDescriptor*> getFieldDescriptors();

  std::vector<MessageDescriptor*> getMessageDescriptors();

  int fieldCount();

  void addMessageDescriptor(MessageDescriptor* messageDescriptor);

  void addFieldDescriptor(FieldDescriptor* fieldDescriptor);

  std::string debugString() override;

 private:
  std::vector<FieldDescriptor*> field_descriptors_;
  std::vector<MessageDescriptor*> message_descriptors_;
};

class FileDescriptor : public Descriptor {
 public:
  FileDescriptor();
  ~FileDescriptor();
  DescriptorType type() override;

  std::string typeName() override;

  std::vector<MessageDescriptor*> getMessageDescriptors();

  MessageDescriptor* getMessageDescriptor(std::string messageDescriptorName);

  // Always Null.
  Descriptor* getEnclosingDescriptor() override;

  void addMessageDescriptor(MessageDescriptor* messageDescriptor);

  std::string debugString() override;

 private:
  std::vector<MessageDescriptor*> message_descriptors_;
};

}  // namespace descriptor
}  // namespace proto_compiler
