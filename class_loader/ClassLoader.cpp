#include "ClassLoader.hpp"

#include <cstdint>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>

#include "class/Class.hpp"
using namespace std;

bool isLittleEndian() {
  uint16_t num = 0x1;
  uint8_t* ptr = reinterpret_cast<uint8_t*>(&num);
  return ptr[0] == 0x1;
}

template <typename T>
T readBigEndian(std::ifstream& file) {
  T value;
  file.read(reinterpret_cast<char*>(&value), sizeof(T));

  if constexpr (std::is_floating_point<T>::value) {
    if (isLittleEndian()) {
      if (sizeof(T) == sizeof(float)) {
        uint32_t temp;
        std::memcpy(&temp, &value, sizeof(temp));
        temp = std::byteswap(temp);
        std::memcpy(&value, &temp, sizeof(value));
      } else if (sizeof(T) == sizeof(double)) {
        uint64_t temp;
        std::memcpy(&temp, &value, sizeof(temp));
        temp = std::byteswap(temp);
        std::memcpy(&value, &temp, sizeof(value));
      }
    }
  } else {
    if (isLittleEndian()) {
      value = std::byteswap(value);
    }
  }
  return value;
}

ClassLoader::ClassLoader(const string fileName) {
  ClassLoader::fileName = fileName;
}

Class ClassLoader::parse() {
  Class _class;
  ifstream file;
  uint32_t magicNumber;
  uint16_t minorVersion;
  uint16_t majorVersion;
  uint16_t constant_pool_count;
  uint8_t tag;

  file.open(fileName, std::ios::binary);

  if (!file) {
    throw std::invalid_argument(
        std::format("Error opening file: {}", fileName));
  }
  magicNumber = readBigEndian<uint32_t>(file);
  if (magicNumber != ClassLoader::MAGIC_NUMBER) {
    throw std::invalid_argument(std::format(
        "Invalid magic number: {:x}. This is not a valid .class file",
        magicNumber));
  }
  _class.setMagicNumber(magicNumber);
  minorVersion = readBigEndian<uint16_t>(file);
  majorVersion = readBigEndian<uint16_t>(file);

  if (majorVersion >= 56 && minorVersion != 0 && minorVersion != 65535) {
    throw std::invalid_argument(
        "A major version >= 56 must have a minor version of 0 or 65535");
  }
  _class.setMinorVersion(minorVersion);
  _class.setMajorVersion(majorVersion);
  constant_pool_count = readBigEndian<uint16_t>(file);
  _class.setConstantPoolCount(constant_pool_count);
  for (unsigned short int idx = 0; idx < _class.getConstantPoolCount() - 1;
       ++idx) {
    file.read(reinterpret_cast<char*>(&tag),
              sizeof(tag));  // single byte no need for endianness conversion
    switch (tag) {
      case 10:
        uint16_t class_index = readBigEndian<uint16_t>(file);
        uint16_t name_and_type_index = readBigEndian<uint16_t>(file);
        break;
    }
  }

  // _class.setConstantPool(cpInfoTable);
  return _class;
}
