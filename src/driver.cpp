#include <iostream>     // std::cerr
#include <fcntl.h>      // open, O_RDONLY
#include <cerrno>       // errno
#include <cstring>      // strerror
#include <sys/stat.h>   // struct stat, fstat
#include <unistd.h>     // close
#include <sys/mman.h>   // mmap, PROT_READ, MAP_PRIVATE, MAP_FAILED, munmap
                        //
#include "mmapfile.hpp"

int main() {

  // TODO pass filename as argument
  try {
    ROMmapFile file{"test.lox"};

    std::cout << file.GetContents();
  } catch (std::exception const &e) {
    std::cerr << e.what();
    std::exit(-1);
  }

}
