#include "ClassFile.hpp"

#include <cassert>

ClassFile::ClassFile(uint32_t magicNumber, uint16_t minorVersion,
                     uint16_t majorVersion, uint16_t constantPoolCount,
                     ConstantPoolTable constantPool, uint16_t accessFlags,
                     uint16_t thisClass, uint16_t superClass,
                     uint16_t interfacesCount, std::vector<uint16_t> interfaces,
                     uint16_t fieldsCount, uint16_t methodsCount) {
  this->setMagicNumber(magicNumber);
  this->setMinorVersion(minorVersion);
  this->setMajorVersion(majorVersion);
  this->setConstantPoolCount(constantPoolCount);

  checkInvariants();
}

void ClassFile::checkInvariants() {
  assert(this->getMagicNumber() == MAGIC_NUMBER);
  assert(!(majorVersion >= 56 && minorVersion != 0 && minorVersion != 65535));
}

void ClassFile::setMagicNumber(uint32_t magicNumber) {
  this->magicNumber = magicNumber;
}

void ClassFile::setMinorVersion(uint16_t minorVersion) {
  this->minorVersion = minorVersion;
}

void ClassFile::setMajorVersion(uint16_t majorVersion) {
  this->majorVersion = majorVersion;
}

void ClassFile::setConstantPoolCount(uint16_t constantPoolCount) {
  this->constantPoolCount = constantPoolCount;
}

void ClassFile::setConstantPool(ConstantPoolTable cpInfoTable) {
  this->constantPool = cpInfoTable;
}

void ClassFile::setAccessFlags() {}

void ClassFile::setThisClass() {}

void ClassFile::setSuperClass() {}

void ClassFile::setInterfacesCount() {}

void ClassFile::setInterfaces() {}

void ClassFile::setFieldsCount() {}

void ClassFile::setMethodsCount() {}

uint32_t ClassFile::getMagicNumber() { return this->magicNumber; }

uint16_t ClassFile::getMinorVersion() { return this->minorVersion; }

uint16_t ClassFile::getMajorVersion() { return this->majorVersion; }

uint16_t ClassFile::getConstantPoolCount() { return this->constantPoolCount; }

// cpInfo* Class::getConstantPool() { return this->constantPool; }

uint16_t ClassFile::getAccessFlags() { return 0; }

uint16_t ClassFile::getThisClass() { return 0; }

uint16_t ClassFile::getSuperClass() { return 0; }

uint16_t ClassFile::getInterfacesCount() { return 0; }

std::vector<uint16_t> ClassFile::getInterfaces() {
  return std::vector<uint16_t>();
}

u_int16_t ClassFile::getFieldsCount() { return u_int16_t(); }

u_int16_t ClassFile::getMethodsCount() { return u_int16_t(); }
