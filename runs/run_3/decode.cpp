#include <iostream>
#include <fstream>
#include <vector>
#include <lzma.h>

void decompress_data(std::istream &input, std::ostream &output)
{
    std::vector<char> compressed_data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    size_t compressed_len = compressed_data.size();
    size_t decompressed_len = compressed_len * 10; // Initial guess for decompressed size
    std::vector<uint8_t> decompressed_data(decompressed_len);

    uint64_t memlimit = UINT64_MAX; // No memory limit
    uint32_t flags = 0;             // No flags

    size_t in_pos = 0;
    size_t out_pos = 0;

    lzma_ret ret = lzma_stream_buffer_decode(&memlimit, flags, nullptr,
                                             reinterpret_cast<const uint8_t *>(compressed_data.data()), &in_pos, compressed_len,
                                             decompressed_data.data(), &out_pos, decompressed_len);

    if (ret != LZMA_OK)
    {
        std::cerr << "Decompression failed with error code: " << ret << std::endl;
        exit(1);
    }

    output.write(reinterpret_cast<const char *>(decompressed_data.data()), out_pos);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input compressed> <output wav>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Error opening input file: " << argv[1] << std::endl;
        return 1;
    }

    std::ofstream output_file(argv[2], std::ios::binary);
    if (!output_file)
    {
        std::cerr << "Error opening output file: " << argv[2] << std::endl;
        return 1;
    }

    decompress_data(input_file, output_file);

    input_file.close();
    output_file.close();

    return 0;
}
