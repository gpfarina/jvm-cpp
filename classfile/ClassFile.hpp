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

struct CONSTANT_String_info {
  uint8_t tag;
  uint16_t string_index;
};

struct CONSTANT_Integer_info {
  uint8_t tag;
  uint32_t bytes;
};

struct CONSTANT_Float_info {
  uint8_t tag;
  uint32_t bytes;
};

struct CONSTANT_Long_info {
  uint8_t tag;
  uint32_t high_bytes;
  uint32_t low_bytes;
};

struct CONSTANT_Double_info {
  uint8_t tag;
  uint32_t high_bytes;
  uint32_t low_bytes;
};

struct CONSTANT_NameAndType_info {
  uint8_t tag;
  uint16_t name_index;
  uint16_t descriptor_index;
};
struct CONSTANT_Utf8_info {
  uint8_t tag;
  uint16_t length;
  uint8_t* bytes;
};

struct CONSTANT_MethodHandle_info {
  uint8_t tag;
  uint8_t reference_kind;
  uint16_t reference_index;
};

struct CONSTANT_MethodType_info {
  uint8_t tag;
  uint16_t descriptor_index;
};

struct CONSTANT_Dynamic_InvokeDynamic_info {
  uint8_t tag;
  uint16_t bootstrap_method_attr_index;
  uint16_t name_and_type_index;
};

struct CONSTANT_Module_info {
  uint8_t tag;
  uint16_t name_index;
};

struct CONSTANT_Package_info {
  uint8_t tag;
  uint16_t name_index;
};

union CONSTANT_pool_entry {
  CONSTANT_Class_info constant_class;
  CONSTANT_FieldMethodIMethodref_info constant_field_method_interface_method;
  CONSTANT_String_info constant_string;
  CONSTANT_Integer_info constant_integer;
  CONSTANT_Float_info constant_float;
  CONSTANT_Long_info constant_long;
  CONSTANT_Double_info constant_double;
  CONSTANT_NameAndType_info constant_name_and_type;
  CONSTANT_Utf8_info constant_utf8;
  CONSTANT_MethodHandle_info constant_method_handle;
  CONSTANT_MethodType_info constant_method_type;
  CONSTANT_Dynamic_InvokeDynamic_info constant_dynamic;
  CONSTANT_Module_info constant_module;
  CONSTANT_Package_info constant_package;
};

struct AttributeInfo {
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint8_t* info;
};

struct FieldInfo_entry {
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t descriptor_index;
  uint16_t attributes_count;
  AttributeInfo* attributes;  // Pointer to an array of AttributeInfo
};
struct MethodInfo {
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t descriptor_index;
  uint16_t attributes_count;
  AttributeInfo* attributes;  // Pointer to an array of AttributeInfo
};

typedef CONSTANT_pool_entry* ConstantPoolTable;
typedef FieldInfo_entry* FieldsInfo;
typedef MethodInfo* MethodsInfo;
typedef AttributeInfo* AttributesInfo;
class ClassFile {
 public:
  ClassFile(uint32_t magicNumber, uint16_t minorVersion, uint16_t majorVersion,
            uint16_t constantPoolCount, ConstantPoolTable constantPool,
            uint16_t accessFlags, uint16_t thisClass, uint16_t superClass,
            uint16_t interfacesCount, std::vector<uint16_t> interfaces,
            uint16_t fieldsCount, FieldsInfo fields, uint16_t methodsCount,
            MethodsInfo methods, uint16_t attributesCount,
            AttributesInfo attributes);
  void setMagicNumber(uint32_t magicNumber);
  void setMinorVersion(uint16_t minorVersion);
  void setMajorVersion(uint16_t majorVersion);
  void setConstantPoolCount(uint16_t constantPoolCount);
  void setConstantPool(ConstantPoolTable cpInfoTable);
  void setAccessFlags(uint16_t accessFlags);
  void setThisClass(uint16_t thisClass);
  void setSuperClass(uint16_t superClass);
  void setInterfacesCount(uint16_t interfacesCount);
  void setInterfaces(std::vector<uint16_t>* interfaces);
  void setFieldsCount(uint16_t fieldsCount);
  void setFields(FieldsInfo fields);
  void setMethodsCount(uint16_t methodsCount);
  void setMethods(MethodsInfo methods);
  void setAttributesCount(uint16_t attributesCount);
  void setAttributes(AttributesInfo attributes);

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
  FieldsInfo getFields();
  u_int16_t getMethodsCount();
  MethodsInfo getMethods();
  uint16_t getAttributesCount();
  AttributesInfo getAttributes();

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
  FieldsInfo fields;
  uint16_t methodsCount;
  MethodsInfo methods;
  uint16_t attributesCount;
  AttributesInfo attributes;

  void checkInvariants();
};
