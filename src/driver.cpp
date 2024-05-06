#include <iostream>     // std::cerr
#include <fcntl.h>      // open, O_RDONLY
#include <cerrno>       // errno
#include <cstring>      // strerror
#include <sys/stat.h>   // struct stat, fstat
#include <unistd.h>     // close
#include <sys/mman.h>   // mmap, PROT_READ, MAP_PRIVATE, MAP_FAILED, munmap

int main() {

  // Obtain the size of the file
  // TODO pass filename as argument
  int fp = open("test.lox", O_RDONLY);

  if (fp == -1) {
    std::cerr << "Error getting file size: " << strerror(errno) << std::endl;
    return 1;
  }

  struct stat fileStat;
  if (fstat(fp, &fileStat) == -1) {
    std::cerr << "Error getting file size: " << strerror(errno) << std::endl;
    close(fp);
    return 1;
  }

  // Map file onto memory
  // PROT_READ -> read-only mode
  // MAP_PRIVATE -> modifications should not be written back to original file
  char* loxFileMap = mmap(NULL, fileStat.st_size, PROT_READ, MAP_PRIVATE, fp, 0);

  if (loxFileMap == MAP_FAILED) {
    std::cerr << "Error mapping file: " << strerror(errno) << std::endl;
    close(fp);
    return 1;
  }


  // End of program
  munmap(loxFileMap, fileStat.st_size);
  close(fp);
}
