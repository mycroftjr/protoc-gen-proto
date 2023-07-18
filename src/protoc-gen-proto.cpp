#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <iostream>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>

#include <google/protobuf/io/zero_copy_stream.h>

namespace google {
    namespace protobuf {
        class FileDescriptor;

        namespace compiler {
            class CodeGenerator;
            class GeneratorContext;

            class MyCodeGenerator : public CodeGenerator {
                // Generates code for the given proto file, generating one or more files in
                // the given output directory.
                //
                // A parameter to be passed to the generator can be specified on the
                // command line.  This is intended to be used by Java and similar languages
                // to specify which specific class from the proto file is to be generated,
                // though it could have other uses as well.  It is empty if no parameter was
                // given.
                //
                // Returns true if successful.  Otherwise, sets *error to a description of
                // the problem (e.g. "invalid parameter") and returns false.
                virtual bool Generate(const FileDescriptor* file, const string& parameter, GeneratorContext* generator_context, string* error) const override
                {
                    /*
                    google::protobuf::FileDescriptorProto fileProto;
                    fileProto.ParseFromFileDescriptor(0);
                    google::protobuf::DescriptorPool pool;
                    const google::protobuf::FileDescriptor* desc =
                        pool.BuildFile(fileProto);
                    std::cout << file->DebugString() << std::endl;
                    */

                    const std::string& path = file->name();
                    std::string basename = path.substr(path.find_last_of("/\\") + 1);
                    basename = cpp::StripProto(basename) + ".pb";

                    {
                        scoped_ptr<io::ZeroCopyOutputStream> output(
                            generator_context->Open(basename + ".proto"));
                        io::CodedOutputStream cos(output.get());

                        std::string debugStr = file->DebugString();
                        cos.WriteRaw(debugStr.c_str(), debugStr.length());
                    }
                    return true;
                }
            };
        }
    }
}


int main(int argc, char** argv) {
    using namespace google::protobuf::compiler;
    MyCodeGenerator generator;
    return PluginMain(argc, argv, &generator);
}