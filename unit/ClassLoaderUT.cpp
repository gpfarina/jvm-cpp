#include <gtest/gtest.h>

#include "class_loader/ClassLoader.hpp"
#include "tools/cpp/runfiles/runfiles.h"
using bazel::tools::cpp::runfiles::Runfiles;

TESTload_magic_number(ClassLoaderUnitTest /*unused*/, Passes /*unused*/) {
  std::string error;
  std::unique_ptr<Runfiles> runfiles(Runfiles::CreateForTest(&error));
  if (runfiles == nullptr) {
    std::cout << "ERROR: null runfiles" << std::endl;
  }
  std::string path = runfiles->Rlocation("__main__/resources/Test.class");
  ClassLoader cl = ClassLoader(path);
  EXPECT_EQ(cl.parse(), 0);
}
