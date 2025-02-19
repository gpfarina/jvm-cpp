#include "Class.hpp"

#include <cassert>

Class::Class(uint32_t magicNumber, uint16_t minorVersion, uint16_t majorVersion,
             uint16_t constantPoolCount, ConstantPoolTable constantPool,
             uint16_t accessFlags, uint16_t thisClass, uint16_t superClass,
             uint16_t interfacesCount, std::vector<uint16_t> interfaces,
             uint16_t fieldsCount, uint16_t methodsCount) {
  this->setMagicNumber(magicNumber);
  this->setMinorVersion(minorVersion);
  this->setMajorVersion(majorVersion);
  this->setConstantPoolCount(constantPoolCount);

  checkInvariants();
}

void Class::checkInvariants() {
  assert(this->getMagicNumber() == MAGIC_NUMBER);
  assert(!(majorVersion >= 56 && minorVersion != 0 && minorVersion != 65535));
}

void Class::setMagicNumber(uint32_t magicNumber) {
  this->magicNumber = magicNumber;
}

void Class::setMinorVersion(uint16_t minorVersion) {
  this->minorVersion = minorVersion;
}

void Class::setMajorVersion(uint16_t majorVersion) {
  this->majorVersion = majorVersion;
}

void Class::setConstantPoolCount(uint16_t constantPoolCount) {
  this->constantPoolCount = constantPoolCount;
}

void Class::setConstantPool(ConstantPoolTable cpInfoTable) {
  this->constantPool = cpInfoTable;
}

void Class::setAccessFlags() {}

void Class::setThisClass() {}

void Class::setSuperClass() {}

void Class::setInterfacesCount() {}

void Class::setInterfaces() {}

void Class::setFieldsCount() {}

void Class::setMethodsCount() {}

uint32_t Class::getMagicNumber() { return this->magicNumber; }

uint16_t Class::getMinorVersion() { return this->minorVersion; }

uint16_t Class::getMajorVersion() { return this->majorVersion; }

uint16_t Class::getConstantPoolCount() { return this->constantPoolCount; }

// cpInfo* Class::getConstantPool() { return this->constantPool; }

uint16_t Class::getAccessFlags() { return 0; }

uint16_t Class::getThisClass() { return 0; }

uint16_t Class::getSuperClass() { return 0; }

uint16_t Class::getInterfacesCount() { return 0; }

std::vector<uint16_t> Class::getInterfaces() { return std::vector<uint16_t>(); }

u_int16_t Class::getFieldsCount() { return u_int16_t(); }

u_int16_t Class::getMethodsCount() { return u_int16_t(); }
