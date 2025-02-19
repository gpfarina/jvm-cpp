#include <cstdint>
#include <string>

#include "class/Class.hpp"

#pragma once
class ClassLoader {
 public:
  ClassLoader(const std::string fileName);
  Class parse();

 private:
  std::string fileName;
};
