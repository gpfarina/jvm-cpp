#include <cstdint>
#include <string>

#include "class/Class.hpp"

#pragma once
class ClassLoader {
 public:
  ClassLoader(const std::string fileName);
  Class parse();
  static const uint32_t MAGIC_NUMBER = 0xCAFEBABE;

 private:
  std::string fileName;
};
