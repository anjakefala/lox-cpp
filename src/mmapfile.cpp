#include "mmapfile.hpp"


#include <cerrno>  // errno
#include <cstring> // strerror
#include <fcntl.h> // open, O_RDONLY
#include <stdexcept>
#include <string_view>
#include <sys/mman.h> // mmap, PROT_READ, MAP_PRIVATE, MAP_FAILED, munmap
#include <sys/stat.h> // struct stat, fstat
#include <unistd.h>   // close

#include <utility>
#include <vector>

#include <sstream>
#include <iostream>

FileDescriptor::FileDescriptor(std::string const &file_name)
      : fd{open(file_name.c_str(), O_RDONLY)} {

    if (fd == -1) {
      std::ostringstream oss;
      oss << "Cannot open file: " << file_name << " (Error " << errno << ": " << strerror(errno) << ")";

      throw std::invalid_argument{oss.str()};
  }
}

FileDescriptor::FileDescriptor(FileDescriptor &&rhs) noexcept
      : fd{std::exchange(rhs.fd, -1)} {}

FileDescriptor &FileDescriptor::operator=(FileDescriptor &&rhs) noexcept {
    std::swap(fd, rhs.fd);
    return *this;
  }

FileDescriptor::~FileDescriptor() {
    if (fd != -1) {
      close(fd);
    }
  }

int FileDescriptor::GetFileNo() const noexcept { return fd; }

size_t FileDescriptor::GetFileSizeInBytes() const {
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
    std::ostringstream oss;
    oss << "Cannot get file size of fd=" << fd <<" (Error " << errno << ": " << strerror(errno) << ")";
      throw std::invalid_argument{oss.str()};
  }
    return fileStat.st_size;
}

ROMmapFile::ROMmapFile(const std::string &file_name)
      : file_descriptor{file_name},
        file_size{file_descriptor.GetFileSizeInBytes()},
        mapped_buffer{mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE,
                           file_descriptor.GetFileNo(), 0)} {
    if (mapped_buffer == MAP_FAILED) {
    std::ostringstream oss;
      oss << "Cannot map file: " << file_name << " (Error " << errno << ": " << strerror(errno) << ")";
      throw std::invalid_argument{oss.str()};
  }
}

ROMmapFile::~ROMmapFile() {
    if (mapped_buffer != MAP_FAILED) {
      munmap(mapped_buffer, file_size);
    }
  }

ROMmapFile::ROMmapFile(ROMmapFile &&rhs)
      : file_descriptor(std::move(rhs.file_descriptor)),
        file_size(std::exchange(rhs.file_size, 0)),
        mapped_buffer(std::exchange(rhs.mapped_buffer, MAP_FAILED)) {}

ROMmapFile &ROMmapFile::operator=(ROMmapFile &&rhs) {
    std::swap(file_descriptor, rhs.file_descriptor);
    std::swap(file_size, rhs.file_size);
    std::swap(mapped_buffer, rhs.mapped_buffer);
    return *this;
  }

std::string_view ROMmapFile::GetContents() const noexcept {
    return std::string_view{static_cast<char const *>(mapped_buffer),
                            file_size};
  }

std::vector<std::string_view> ROMmapFile::GetLines() const {
    std::vector<std::string_view> lines;
    std::string_view contents = GetContents();
    while (true) {
      size_t const location_of_newline = contents.find_first_of('\n');
      if (location_of_newline != std::string_view::npos) {
        lines.push_back(contents.substr(0, location_of_newline));
        contents = contents.substr(location_of_newline + 1);
      } else {
        if (not contents.empty()) {
          lines.push_back(std::move(contents));
        }
        break;
      }
    }
    return lines;
  }

