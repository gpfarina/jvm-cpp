#include "ClassFileLoader.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>

#include "classfile/ClassFile.hpp"
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

ClassFileLoader::ClassFileLoader(const string fileName) {
  ClassFileLoader::fileName = fileName;
}

ClassFile ClassFileLoader::parse() {
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

  for (unsigned short int idx = 0; idx < constant_pool_count - 1; idx++) {
    file.read(reinterpret_cast<char*>(&tag), sizeof(tag));
    switch (tag) {
      case 1:
        cpTable[idx].constant_utf8.tag = tag;
        cpTable[idx].constant_utf8.length = readBigEndian<uint16_t>(file);
        cpTable[idx].constant_utf8.bytes =
            new uint8_t[cpTable[idx].constant_utf8.length];
        for (unsigned short int idxCounter = 0;
             idxCounter < cpTable[idx].constant_utf8.length; idxCounter++) {
          file.read(reinterpret_cast<char*>(
                        &cpTable[idx].constant_utf8.bytes[idxCounter]),
                    sizeof(uint8_t));
        }
        break;
      case 3:
        cpTable[idx].constant_integer.tag = tag;
        cpTable[idx].constant_integer.bytes = readBigEndian<uint32_t>(file);
        break;
      case 4:
        cpTable[idx].constant_float.tag = tag;
        cpTable[idx].constant_float.bytes =
            readBigEndian<uint32_t>(file);  // lets keep them as 32 bits ints
        break;
      case 5:
        cpTable[idx].constant_long.tag = tag;
        cpTable[idx].constant_long.low_bytes = readBigEndian<uint32_t>(file);
        cpTable[idx].constant_long.high_bytes = readBigEndian<uint32_t>(file);
        idx++;
        break;
      case 6:
        cpTable[idx].constant_double.tag = tag;
        cpTable[idx].constant_double.low_bytes = readBigEndian<uint32_t>(file);
        cpTable[idx].constant_double.high_bytes = readBigEndian<uint32_t>(file);
        idx++;
        break;
      case 7:
        cpTable[idx].constant_class.tag = tag;
        cpTable[idx].constant_class.name_index = readBigEndian<uint16_t>(file);
        break;
      case 8:
        cpTable[idx].constant_string.tag = tag;
        cpTable[idx].constant_string.string_index =
            readBigEndian<uint16_t>(file);
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
      case 12:
        cpTable[idx].constant_name_and_type.tag = tag;
        cpTable[idx].constant_name_and_type.name_index =
            readBigEndian<uint16_t>(file);
        cpTable[idx].constant_name_and_type.descriptor_index =
            readBigEndian<uint16_t>(file);
        break;
    }
  }
  std::vector<uint16_t> vec(1, 0);
  return ClassFile(magicNumber, minorVersion, majorVersion, constant_pool_count,
                   cpTable, 0, 0, 0, 0, vec, 0, 0);
}
