#include <gtest/gtest.h>

#include "classfile/ClassFile.hpp"
#include "classfile_loader/ClassFileLoader.hpp"
#include "tools/cpp/runfiles/runfiles.h"
using bazel::tools::cpp::runfiles::Runfiles;

TEST(ClassFileLoaderUnitTest /*unused*/, Passes /*unused*/) {
  std::string error;
  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest(&error));
  if (runfiles == nullptr) {
    std::cout << "ERROR: null runfiles" << std::endl;
  }
  std::string path = runfiles->Rlocation("__main__/resources/Test.class");
  ClassFileLoader cl = ClassFileLoader(path);
  ClassFile _classfile = cl.parse();
  EXPECT_EQ(_classfile.getMajorVersion(), 65);
  EXPECT_EQ(_classfile.getConstantPoolCount(), 15);
}
