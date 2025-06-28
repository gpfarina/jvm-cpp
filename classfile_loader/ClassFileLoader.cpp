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

ConstantPoolTable parseConstantPoolTable(uint16_t constant_pool_count,
                                         std::ifstream& file) {
  ConstantPoolTable cpTable = new CONSTANT_pool_entry[constant_pool_count - 1];
  // The constant pool count includes the 0th entry, which is not used.
  // We allocate an array of CONSTANT_pool_entry of size constant_pool_count
  // - 1.
  uint8_t tag;
  for (unsigned short int idx = 0; idx < constant_pool_count - 1; idx++) {
    file.read(reinterpret_cast<char*>(&tag), sizeof(tag));
    // tag 2, 13, 14 , 15 are not used, we throw an exception if we find them
    if (tag == 2 || tag == 13 || tag == 14 || tag == 21) {
      throw std::invalid_argument(
          std::format("Reserved constant pool tag: {} at index {}",
                      static_cast<int>(tag), idx));
    }
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
      case 15:
        cpTable[idx].constant_method_handle.tag = tag;
        file.read(reinterpret_cast<char*>(
                      &cpTable[idx].constant_method_handle.reference_kind),
                  sizeof(uint8_t));
        cpTable[idx].constant_method_handle.reference_index =
            readBigEndian<uint16_t>(file);
        break;
      case 16:
        cpTable[idx].constant_method_type.tag = tag;
        cpTable[idx].constant_method_type.descriptor_index =
            readBigEndian<uint16_t>(file);
        break;
      case 17:
      case 18:
        cpTable[idx].constant_dynamic.tag = tag;
        cpTable[idx].constant_dynamic.bootstrap_method_attr_index =
            readBigEndian<uint16_t>(file);
        cpTable[idx].constant_dynamic.name_and_type_index =
            readBigEndian<uint16_t>(file);
        break;
      case 19:
        cpTable[idx].constant_module.tag = tag;
        cpTable[idx].constant_module.name_index = readBigEndian<uint16_t>(file);
        break;
      case 20:
        cpTable[idx].constant_package.tag = tag;
        cpTable[idx].constant_package.name_index =
            readBigEndian<uint16_t>(file);
        break;
      default:
        throw std::invalid_argument(std::format("Unknown constant pool tag: {}",
                                                static_cast<int>(tag)));
    }
  }
  return cpTable;
}

FieldsInfo parseFieldsInfo(uint16_t fieldsCount, std::ifstream& file) {
  FieldsInfo fields;
  if (fieldsCount > 0) {
    fields = new FieldInfo_entry[fieldsCount];
    for (uint16_t i = 0; i < fieldsCount; i++) {
      fields[i].access_flags = readBigEndian<uint16_t>(file);
      fields[i].name_index = readBigEndian<uint16_t>(file);
      fields[i].descriptor_index = readBigEndian<uint16_t>(file);
      uint16_t attributes_count = readBigEndian<uint16_t>(file);
      fields[i].attributes_count = attributes_count;
      fields[i].attributes =
          new AttributeInfo[attributes_count];  // allocate memory for
                                                // attributes
      for (uint16_t j = 0; j < attributes_count; j++) {
        fields[i].attributes[j].attribute_name_index =
            readBigEndian<uint16_t>(file);
        fields[i].attributes[j].attribute_length =
            readBigEndian<uint32_t>(file);
        fields[i].attributes[j].info =
            new uint8_t[fields[i].attributes[j].attribute_length];
        file.read(reinterpret_cast<char*>(fields[i].attributes[j].info),
                  fields[i].attributes[j].attribute_length);
      }
    }
  } else {
    fields = nullptr;
  }
  return fields;
}

MethodsInfo parseMethodsInfo(uint16_t methodsCount, std::ifstream& file) {
  MethodsInfo methods;
  if (methodsCount > 0) {
    methods = new MethodInfo[methodsCount];
    for (uint16_t i = 0; i < methodsCount; i++) {
      methods[i].access_flags = readBigEndian<uint16_t>(file);
      methods[i].name_index = readBigEndian<uint16_t>(file);
      methods[i].descriptor_index = readBigEndian<uint16_t>(file);
      uint16_t attributes_count = readBigEndian<uint16_t>(file);
      methods[i].attributes_count = attributes_count;
      methods[i].attributes =
          new AttributeInfo[attributes_count];  // allocate memory for
                                                // attributes
      for (uint16_t j = 0; j < attributes_count; j++) {
        methods[i].attributes[j].attribute_name_index =
            readBigEndian<uint16_t>(file);
        methods[i].attributes[j].attribute_length =
            readBigEndian<uint32_t>(file);
        methods[i].attributes[j].info =
            new uint8_t[methods[i].attributes[j].attribute_length];
        file.read(reinterpret_cast<char*>(methods[i].attributes[j].info),
                  methods[i].attributes[j].attribute_length);
      }
    }
  } else {
    methods = nullptr;
  }
  return methods;
}

AttributesInfo parseAttributesInfo(uint16_t attributesCount,
                                   std::ifstream& file) {
  AttributesInfo attributes;
  if (attributesCount > 0) {
    attributes = new AttributeInfo[attributesCount];
    for (uint16_t i = 0; i < attributesCount; i++) {
      attributes[i].attribute_name_index = readBigEndian<uint16_t>(file);
      attributes[i].attribute_length = readBigEndian<uint32_t>(file);
      attributes[i].info = new uint8_t[attributes[i].attribute_length];
      file.read(reinterpret_cast<char*>(attributes[i].info),
                attributes[i].attribute_length);
    }
  } else {
    attributes = nullptr;
  }
  return attributes;
}

ClassFileLoader::ClassFileLoader(const string fileName) {
  ClassFileLoader::fileName = fileName;
}

ClassFile ClassFileLoader::parse() {
  ifstream file;
  uint32_t magicNumber;
  uint16_t minorVersion;
  uint16_t majorVersion;
  uint16_t constantPoolCount;
  ConstantPoolTable cpTable;
  uint16_t accessFlags;
  uint16_t thisClass;
  uint16_t superClass;
  uint16_t interfacesCount;
  std::vector<uint16_t> interfaces;
  uint16_t fieldsCount;
  FieldsInfo fields;
  uint16_t methodsCount;
  MethodsInfo methods;
  uint16_t attributesCount;
  AttributesInfo attributes;
  file.open(fileName, std::ios::binary);

  if (!file) {
    throw std::invalid_argument(
        std::format("Error opening file: {}", fileName));
  }

  magicNumber = readBigEndian<uint32_t>(file);
  minorVersion = readBigEndian<uint16_t>(file);
  majorVersion = readBigEndian<uint16_t>(file);
  constantPoolCount = readBigEndian<uint16_t>(file);
  cpTable = parseConstantPoolTable(constantPoolCount, file);
  accessFlags = readBigEndian<uint16_t>(file);
  thisClass = readBigEndian<uint16_t>(file);
  superClass = readBigEndian<uint16_t>(file);
  interfacesCount = readBigEndian<uint16_t>(file);
  if (interfacesCount > 0) {
    interfaces.resize(interfacesCount);
    for (uint16_t i = 0; i < interfacesCount; i++) {
      interfaces[i] = readBigEndian<uint16_t>(file);
    }
  }
  fieldsCount = readBigEndian<uint16_t>(file);
  fields = parseFieldsInfo(fieldsCount, file);
  methodsCount = readBigEndian<uint16_t>(file);
  methods = parseMethodsInfo(methodsCount, file);
  attributesCount = readBigEndian<uint16_t>(file);
  attributes = parseAttributesInfo(attributesCount, file);
  file.close();
  return ClassFile(magicNumber, minorVersion, majorVersion, constantPoolCount,
                   cpTable, accessFlags, thisClass, superClass, interfacesCount,
                   interfaces, fieldsCount, fields, methodsCount, methods,
                   attributesCount, attributes);
}
