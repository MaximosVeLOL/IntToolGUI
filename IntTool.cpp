#include <iostream>
#include <fstream>
#include <filesystem>
#include <stack>
#include <iomanip>
#include <QMessageBox>
#include "IntTool.h"

void IntToolUtil::ImportFile(std::string arg_int, std::string arg_name) { //arg_int = int file, arg_name = output directory
    std::vector<IntArchive_Block> blocks;

    {
        // Open int file
        std::ifstream stream_int(arg_int, std::ios::binary);
        if (!stream_int)
        {
            throw("Failed to read file!");
        }

        while (1)
        {
            // Read block header
            char block[INT_BLOCK_SIZE];
            if (!stream_int.read(block, INT_BLOCK_SIZE))
            {
                std::cerr << "Archive failed to read block" << std::endl;
                return;
            }

            uint32_t block_type = Read32(block + 0);
            uint32_t block_files = Read32(block + 4);
            uint32_t block_datasectors = Read32(block + 8);
            uint32_t block_datasize = block_datasectors << 11;

            // Check block type
            if (block_type == 0xFFFFFFFF)
                break;

            std::string prefix = "";
            switch (block_type)
            {
            case 1: // TIM
                prefix = "TIM/";
                break;
            case 2: // VAB
                prefix = "VAB/";
                break;
            case 3: // MEM
                break;
            default:
            {
                std::cerr << "Archive unrecognized block type" << std::endl;
                return;
            }
            }

            // Index directory
            IntArchive_Block block_entry;
            block_entry.type = block_type;

            if ((0x10 + block_files * 0x14) > INT_BLOCK_SIZE)
            {
                std::cerr << "Archive block directory too large" << std::endl;
                return;
            }

            char *dirp = block + 0x10;
            size_t file_offset = 0;

            for (uint32_t i = 0; i < block_files; i++)
            {
                // Read directory header
                size_t dir_size = Read32(dirp + 0);

                char dir_name[0x11];
                std::memcpy(dir_name, dirp + 4, 0x10);
                dir_name[0x10] = '\0';

                if (dir_size > block_datasize || file_offset > (block_datasize - dir_size))
                {
                    std::cerr << "Archive block data doesn't fit in allocated size" << std::endl;
                    return;
                }

                // Push file entry
                IntArchive_Directory file_entry;
                file_entry.name = prefix + std::string(dir_name);
                file_entry.offset = (long long)stream_int.tellg() + file_offset;
                file_entry.size = dir_size;
                block_entry.directory.push_back(file_entry);

                // Index next file
                dirp += 0x14;

                file_offset += dir_size;
                if (dir_size & 3)
                    file_offset += (4 - (dir_size & 3));
            }

            // Seek past data and push block entry
            stream_int.seekg(block_datasize, std::ios::cur);

            blocks.push_back(block_entry);
        }
    }

    {
        // Open txt file
        std::ofstream stream_txt(arg_name + ".txt");
        if (!stream_txt)
        {
            std::cerr << "Failed to open " << arg_name << ".txt" << std::endl;
            return;
        }

        // Write blocks
        for (auto &i : blocks)
        {
            stream_txt << (int)i.type << " " << (int)i.directory.size() << std::endl;
            for (auto &j : i.directory)
                stream_txt << "\"" << j.name << "\"" << std::endl;
        }
    }

    {
        // Open int file
        std::ifstream stream_int(arg_int, std::ios::binary);
        if (!stream_int)
        {
            std::cerr << "Failed to open " << arg_int << std::endl;
            return;
        }

        // Write files
        for (auto &i : blocks)
        {
            for (auto &j : i.directory)
            {
                // Make folder for file
                std::filesystem::path file_path = arg_name;
                file_path /= j.name;
                file_path = file_path.parent_path();

                std::stack<std::filesystem::path> file_paths;
                while (!file_path.empty() && !std::filesystem::exists(file_path))
                {
                    file_paths.push(file_path);
                    file_path = file_path.parent_path();
                }

                while (!file_paths.empty())
                {
                    std::filesystem::create_directory(file_paths.top());
                    file_paths.pop();
                }

                // Open file
                std::ofstream stream_file(arg_name + "/" + j.name, std::ios::binary);
                if (!stream_file)
                {
                    std::cerr << "Failed to open " << arg_name << "/" << j.name << std::endl;
                    return;
                }

                // Read data from int file
                j.data = new char[j.size];
                stream_int.seekg(j.offset, std::ios::beg);
                if (!stream_int.read(j.data, j.size))
                {
                    std::cerr << "Failed to read " << j.name << std::endl;
                    return;
                }

                // Write data to file
                stream_file.write(j.data, j.size);

                // Free data
                delete[] j.data;
                j.data = nullptr;
            }
        }
    }
}

void IntToolUtil::ExportFile(std::string arg_int, std::string arg_name, std::string folderName) { //Arg_int = txt file , arg_name = output
    // Repack mode
    std::vector<IntArchive_Block> blocks;

    {
        // Open txt file
        std::ifstream stream_txt(arg_name + "/" + folderName + ".txt");
        if (!stream_txt)
        {
            std::cerr << "Failed to open " << arg_name << "/" << folderName << ".txt" << std::endl;
            return;
        }

        // Read blocks
        while (stream_txt && !stream_txt.eof())
        {
            IntArchive_Block block_entry;
            size_t files;

            stream_txt >> block_entry.type >> files;
            if (stream_txt.eof() || !stream_txt)
                break;

            if ((0x10 + files * 0x14) > INT_BLOCK_SIZE)
            {
                std::cerr << "Block has " << files << " which is too many, consider splitting" << std::endl;
                return;
            }

            block_entry.directory.resize(files);
            for (auto &j : block_entry.directory)
            {
                stream_txt >> std::quoted(j.name);
                if (stream_txt.eof() || !stream_txt)
                    break;
                if (j.name.size() > 16)
                {
                    std::cerr << "File name " << j.name << " is longer than 16 characters" << std::endl;
                    return;
                }
            }

            blocks.push_back(block_entry);
        }
    }

    {
        // Read files
        for (auto &i : blocks)
        {
            for (auto &j : i.directory)
            {
                // Open file
                std::ifstream stream_file(arg_name + "/" + folderName + "/" + j.name, std::ios::binary | std::ios::ate);
                if (!stream_file)
                {
                    std::cerr << "Failed to open " << arg_name << "/" << folderName << "/" << j.name << std::endl;
                    return;
                }

                // Read data from file
                j.size = stream_file.tellg();
                j.data = new char[j.size];
                stream_file.seekg(0, std::ios::beg);
                stream_file.read(j.data, j.size);
            }
        }
    }

    {
        // Calculate file offsets
        for (auto &i : blocks)
        {
            uint32_t file_offset = 0;
            for (auto &j : i.directory)
            {
                j.offset = file_offset;
                file_offset += j.size;
                if (file_offset & 3)
                    file_offset += (4 - (file_offset & 3));
            }
        }
    }

    {
        // Open int file
        std::ofstream stream_int(arg_int, std::ios::binary);
        if (!stream_int)
        {
            std::cerr << "Failed to open " << arg_int << std::endl;
            return;
        }

        // Write blocks
        for (auto &i : blocks)
        {
            // Calculate block data area
            char head_data[INT_BLOCK_SIZE] = {};

            size_t block_data_start = (long long)stream_int.tellp() + INT_BLOCK_SIZE;
            size_t block_data_area = 0;
            if (!i.directory.size())
            {
                std::cerr << "Block needs more than 0 files" << std::endl;
                return;
            }
            block_data_area = i.directory[i.directory.size() - 1].offset + i.directory[i.directory.size() - 1].size;
            if (block_data_area & 0x7FF)
                block_data_area += (0x800 - (block_data_area & 0x7FF));
            size_t block_data_end = block_data_start + block_data_area;

            // Write header
            Write32(head_data + 0, i.type);
            Write32(head_data + 4, i.directory.size());
            Write32(head_data + 8, block_data_area >> 11);
            Write32(head_data + 12, 0);//block_data_area);

            // Write file directory
            char *head_datap = head_data + 0x10;
            for (auto &j : i.directory)
            {
                Write32(head_datap + 0, j.size);
                std::memcpy(head_datap + 4, j.name.data(), j.name.size());
                head_datap += 0x14;
            }

            stream_int.write(head_data, INT_BLOCK_SIZE);

            // Write files
            char *body_data = new char[block_data_area]{};
            char *body_datap = body_data;
            for (auto &j : i.directory)
            {
                std::memcpy(body_datap, j.data, j.size);
                body_datap += j.size;
                if (j.size & 3)
                    body_datap += (4 - (j.size & 3));
            }
            stream_int.write(body_data, block_data_area);
            delete[] body_data;
        }

        // Write terminator block
        char head_data[INT_BLOCK_SIZE] = {};
        head_data[0] = 0xFF;
        head_data[1] = 0xFF;
        head_data[2] = 0xFF;
        head_data[3] = 0xFF;
        stream_int.write(head_data, INT_BLOCK_SIZE);
    }
}
