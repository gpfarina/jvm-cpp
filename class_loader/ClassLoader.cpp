#include "ClassLoader.hpp"

#include <cassert>
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
  minorVersion = readBigEndian<uint16_t>(file);
  majorVersion = readBigEndian<uint16_t>(file);
  constant_pool_count = readBigEndian<uint16_t>(file);

  ConstantPoolTable cpTable = new CONSTANT_pool_entry[constant_pool_count - 1];

  for (unsigned short int idx = 0; idx < constant_pool_count - 1; ++idx) {
    file.read(reinterpret_cast<char*>(&tag), sizeof(tag));
    switch (tag) {
      case 7:
        cpTable[idx].constant_class.tag = tag;
        cpTable[idx].constant_class.name_index = readBigEndian<uint16_t>(file);
        break;
      case 9:
      case 10:
      case 11:
        cpTable[idx].constant_field_method_interface_method.tag = tag;
        cpTable[idx].constant_field_method_interface_method.class_index =
            readBigEndian<uint16_t>(file);
        cpTable[idx]
            .constant_field_method_interface_method.name_and_type_index =
            readBigEndian<uint16_t>(file);
        break;
    }
  }
  std::vector<uint16_t> vec(1, 0);
  return Class(magicNumber, minorVersion, majorVersion, constant_pool_count,
               cpTable, 0, 0, 0, 0, vec, 0, 0);
}
