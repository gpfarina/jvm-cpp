#include "ClassLoader.hpp"

#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>

#include "class/Class.hpp"
using namespace std;

ClassLoader::ClassLoader(const string fileName) {
  ClassLoader::fileName = fileName;
}

bool ClassLoader::isLittleEndian() {
  uint16_t num = 0x1;
  uint8_t* ptr = reinterpret_cast<uint8_t*>(&num);
  return ptr[0] == 0x1;
}

Class ClassLoader::parse() {
  Class _class;
  ifstream file;
  uint32_t magicNumber;
  uint16_t minorVersion;
  uint16_t majorVersion;

  file.open(fileName, std::ios::binary);

  if (!file) {
    throw std::invalid_argument(
        std::format("Error opening file: {}", fileName));
  }
  file.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
  if (magicNumber != ClassLoader::MAGIC_NUMBER_LITTLE_ENDIAN) {
    throw std::invalid_argument(std::format(
        "Invalid magic number: {:x}. This is not a valid .class file",
        magicNumber));
  }
  _class.setMagicNumber(isLittleEndian() ? byteswap(magicNumber) : magicNumber);
  file.read(reinterpret_cast<char*>(&minorVersion), sizeof(minorVersion));
  _class.setMinorVersion(isLittleEndian() ? byteswap(minorVersion)
                                          : minorVersion);
  file.read(reinterpret_cast<char*>(&majorVersion), sizeof(majorVersion));
  _class.setMajorVersion(isLittleEndian() ? byteswap(majorVersion)
                                          : majorVersion);

  if (_class.getMajorVersion() >= 56 && _class.getMinorVersion() != 0 &&
      _class.getMinorVersion() != 65535) {
    throw std::invalid_argument(
        "A major version >= 56 must have a minor version of 0 or 65535");
  }
  return _class;
}
