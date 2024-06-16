#ifndef MMAPFILE_HPP
#define MMAPFILE_HPP

#include <string>
#include <string_view>
#include <vector>

class FileDescriptor {
public:
    explicit FileDescriptor(const std::string &file_name);

    FileDescriptor(const FileDescriptor &) = delete;
    FileDescriptor &operator=(const FileDescriptor &) = delete;
    FileDescriptor(FileDescriptor &&rhs) noexcept;
    FileDescriptor &operator=(FileDescriptor &&rhs) noexcept;

    ~FileDescriptor();

    [[nodiscard]] int GetFileNo() const noexcept;
    [[nodiscard]] size_t GetFileSizeInBytes() const;

private:
    int fd;
};

class ROMmapFile {
public:
    explicit ROMmapFile(const std::string &file_name);
    ~ROMmapFile();

    ROMmapFile(ROMmapFile &&rhs);
    ROMmapFile &operator=(ROMmapFile &&rhs);

    [[nodiscard]] std::string_view GetContents() const noexcept;
    [[nodiscard]] std::vector<std::string_view> GetLines() const;

private:
    FileDescriptor file_descriptor;
    size_t file_size{0};
    void *mapped_buffer{nullptr};
};

#endif // MMAPFILE_HPP

