#include "ClassLoader.hpp"

#include <cstdint>
#include <fstream>
#include <iostream>
using namespace std;

ClassLoader::ClassLoader(const string fileName) {
  ClassLoader::fileName = fileName;
}

bool ClassLoader::isLittleEndian() {
  uint16_t num = 0x1;
  uint8_t* ptr = reinterpret_cast<uint8_t*>(&num);
  return ptr[0] == 0x1;
}
int ClassLoader::parse() {
  ifstream file;
  uint32_t magicNumber;

  file.open(fileName, std::ios::binary);

  if (!file) {
    cerr << "Error opening file: " << fileName << endl;
    return -1;
  }
  file.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));

  if ((isLittleEndian() &&
       magicNumber != ClassLoader::MAGIC_NUMBER_LITTLE_ENDIAN) ||
      (!isLittleEndian() &&
       magicNumber != ClassLoader::MAGIC_NUMBER_BIG_ENDIAN)) {
    cerr << "Invalid magic number. This is not a valid .class file.: "
         << std::hex << magicNumber << endl;
    return -2;
  }
  return 0;
}
