#include <cstdint>
#include <fstream>
#include <string>

#include "classfile/ClassFile.hpp"

#pragma once
class ClassFileLoader {
 public:
  ClassFileLoader(const std::string fileName);
  ClassFile parse();

 private:
  std::string fileName;
};
