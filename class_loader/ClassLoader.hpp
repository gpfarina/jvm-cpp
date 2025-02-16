#pragma once
#include <string>
class ClassLoader {
 public:
  ClassLoader(const std::string fileName);
  int parse();
  static const int MAGIC_NUMBER_BIG_ENDIAN = 0xCAFEBABE;
  static const int MAGIC_NUMBER_LITTLE_ENDIAN = 0xBEBAFECA;

 private:
  std::string fileName;
  bool isLittleEndian();
};
