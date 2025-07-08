#ifndef INTTOOL_H
#define INTTOOL_H
#endif // INTTOOL_H


#include <string>
#include <vector>

#include <cstdint>
#include <cstddef>
#include <cstring>


// Image helpers
static uint16_t Read16(char *data) { return (((uint16_t)((uint8_t)data[0])) << 0) | (((uint16_t)((uint8_t)data[1])) << 8); }
static uint32_t Read32(char *data) { return (((uint32_t)((uint8_t)data[0])) << 0) | (((uint32_t)((uint8_t)data[1])) << 8) | (((uint32_t)((uint8_t)data[2])) << 16) | (((uint32_t)((uint8_t)data[3])) << 24); }

static void Write16(char *d, uint16_t x)
{
    d[0] = (x >> 0);
    d[1] = (x >> 8);
}

static void Write32(char *d, uint32_t x)
{
    d[0] = (x >> 0);
    d[1] = (x >> 8);
    d[2] = (x >> 16);
    d[3] = (x >> 24);
}

// Int archive constants
static constexpr unsigned int INT_BLOCK_SIZE = 4 << 11;

// Int archive structures
struct IntArchive_Directory
{
    ~IntArchive_Directory() { delete[] data; }

    std::string name;
    size_t offset, size;
    char *data = nullptr;
};

struct IntArchive_Block
{
    uint32_t type;
    std::vector<IntArchive_Directory> directory;
};

class IntToolUtil {

    public:

    virtual void ImportFile(std::string, std::string);

    virtual void ExportFile(std::string, std::string, std::string);

};
