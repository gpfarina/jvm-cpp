#include <cstdint>
#include <string>
#include <vector>

#pragma once

class Class {
 public:
  void setMagicNumber(uint32_t magicNumber);
  void setMinorVersion(uint16_t minorVersion);
  void setMajorVersion(uint16_t majorVersion);
  void setConstantPoolCount(uint16_t constantPoolCount);
  // void setConstantPool(cpInfo *cpInfoTable);
  void setAccessFlags();
  void setThisClass();
  void setSuperClass();
  void setInterfacesCount();
  void setInterfaces();
  void setFieldsCount();
  void setMethodsCount();

  uint32_t getMagicNumber();
  uint16_t getMinorVersion();
  uint16_t getMajorVersion();
  uint16_t getConstantPoolCount();
  // cpInfo* getConstantPool();
  uint16_t getAccessFlags();
  uint16_t getThisClass();
  uint16_t getSuperClass();
  uint16_t getInterfacesCount();
  std::vector<uint16_t> getInterfaces();
  u_int16_t getFieldsCount();
  u_int16_t getMethodsCount();

 private:
  uint32_t magicNumber;
  uint16_t minorVersion;
  uint16_t majorVersion;
  uint16_t constantPoolCount;
  // cpInfo *constantPool;
  uint16_t accessFlags;
  uint16_t thisClass;
  uint16_t superClass;
  uint16_t interfacesCount;
  std::vector<uint16_t> interfaces;
  uint16_t fieldsCount;
  uint16_t methodsCount;
};
