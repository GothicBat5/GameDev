#include "flatbuffers/file_manager.h"
#include <fstream>
#include <set>
#include <string>

namespace flatbuffers {

bool RealFileSaver::SaveFile(const char* name, const char* buf, size_t len, bool binary) 
{
  std::ofstream ofs(name, binary ? std::ofstream::binary : std::ofstream::out);
  
  if (!ofs.is_open()) return false;
  
  ofs.write(buf, len);
  
  return !ofs.bad();
}

}  // namespace flatbuffers
