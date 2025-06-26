#include <iostream>
#include <memory>
#include <string>

#include "classfile/ClassFile.hpp"
#include "classfile_loader/ClassFileLoader.hpp"
#include "tools/cpp/runfiles/runfiles.h"

using bazel::tools::cpp::runfiles::Runfiles;

int main(int argc, char** argv) {
    std::string error;
    std::unique_ptr<Runfiles> runfiles(Runfiles::Create(argv[0], &error));
    if (!runfiles) {
        std::cerr << "ERROR: Failed to create runfiles: " << error << std::endl;
        return 1;
    }

    std::string path = runfiles->Rlocation("__main__/resources/Test.class");
    ClassFileLoader cl(path);
    ClassFile _classfile = cl.parse();

    bool passed = true;

    if (_classfile.getMajorVersion() != 65) {
        std::cerr << "FAIL: Expected major version 65, got " << _classfile.getMajorVersion() << std::endl;
        passed = false;
    }
    if (_classfile.getConstantPoolCount() != 15) {
        std::cerr << "FAIL: Expected constant pool count 15, got " << _classfile.getConstantPoolCount() << std::endl;
        passed = false;
    }

    if (passed) {
        std::cout << "PASS: All checks passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Some checks failed." << std::endl;
        return 1;
    }
}
