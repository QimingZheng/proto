#include <compiler/descriptor/descriptor.h>
#include <compiler/tokenizer/tokenizer.h>
#include <compiler/parser/parser.h>
#include <compiler/generator/code_generator.h>
#include <io/file_stream.h>

using namespace proto;
using namespace proto_compiler;

int main(int argc, char** argv) {
  if (argc < 3) {
    return 1;
  }
  std::string inputProtoFileName(argv[1]);
  std::string outputPathName(argv[2]);
  if (!io::FileStream::fileExists(inputProtoFileName)) {
    return 1;
  };

  io::FileStream fs(inputProtoFileName);

  tokenizer::Tokenizer tk(fs);

  auto tokens = tk.scan();

  parser::Parser ps(tokens);

  auto fileDescriptor = ps.getFileDescriptor();

  generator::CodeGenerator cg(outputPathName);

  auto interface = cg.generateInterface(fileDescriptor);
  auto impl = cg.generateImpl(fileDescriptor);

  std::ofstream interfaceF(outputPathName + "/pb.h");
  std::ofstream implF(outputPathName + "/pb.cc");
  interfaceF << "#include<iostream>\n#include<string>\n#include<vector>\nusing "
                "namespace std;\n" +
                    interface;
  interfaceF.close();
  implF << "#include \"pb.h\"\n" + impl;
  implF.close();

  return 0;
}