#include <cstdint>
#include <string>
#include <vector>

#pragma once
static const uint32_t MAGIC_NUMBER = 0xCAFEBABE;
struct CONSTANT_FieldMethodIMethodref_info {
  uint8_t tag;
  uint16_t class_index;
  uint16_t name_and_type_index;
};

struct CONSTANT_Class_info {
  uint8_t tag;
  uint16_t name_index;
};

struct CONSTANT_Utf8_info {
  uint8_t tag;
  uint16_t length;
  uint8_t* bytes;
};
union CONSTANT_pool_entry {
  CONSTANT_Class_info constant_class;
  CONSTANT_FieldMethodIMethodref_info constant_field_method_interface_method;
  CONSTANT_Utf8_info constant_utf8_info;
};

typedef CONSTANT_pool_entry* ConstantPoolTable;
class ClassFile {
 public:
  ClassFile(uint32_t magicNumber, uint16_t minorVersion, uint16_t majorVersion,
            uint16_t constantPoolCount, ConstantPoolTable constantPool,
            uint16_t accessFlags, uint16_t thisClass, uint16_t superClass,
            uint16_t interfacesCount, std::vector<uint16_t> interfaces,
            uint16_t fieldsCount, uint16_t methodsCount);
  void setMagicNumber(uint32_t magicNumber);
  void setMinorVersion(uint16_t minorVersion);
  void setMajorVersion(uint16_t majorVersion);
  void setConstantPoolCount(uint16_t constantPoolCount);
  void setConstantPool(ConstantPoolTable cpInfoTable);
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
  ConstantPoolTable getConstantPool();
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
  ConstantPoolTable constantPool;
  uint16_t accessFlags;
  uint16_t thisClass;
  uint16_t superClass;
  uint16_t interfacesCount;
  std::vector<uint16_t> interfaces;
  uint16_t fieldsCount;
  uint16_t methodsCount;

  void checkInvariants();
};
