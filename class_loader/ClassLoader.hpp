#include <cstdint>
#include <string>

#include "class/Class.hpp"

#pragma once
class ClassLoader {
 public:
  ClassLoader(const std::string fileName);
  Class parse();
  static const uint32_t MAGIC_NUMBER_BIG_ENDIAN = 0xCAFEBABE;
  static const uint32_t MAGIC_NUMBER_LITTLE_ENDIAN = 0xBEBAFECA;

 private:
  std::string fileName;
  bool isLittleEndian();
};
