#include <iostream>
#include <fstream>
#include <vector>
#include <lzma.h>

void compress_data(std::istream &input, std::ostream &output)
{
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    size_t source_len = buffer.size();
    size_t dest_len = lzma_stream_buffer_bound(source_len);
    std::vector<uint8_t> compressed_data(dest_len);

    lzma_ret ret = lzma_easy_buffer_encode(LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64,
                                           nullptr, reinterpret_cast<const uint8_t *>(buffer.data()), source_len,
                                           compressed_data.data(), &dest_len, compressed_data.size());

    if (ret != LZMA_OK)
    {
        std::cerr << "Compression failed with error code: " << ret << std::endl;
        exit(1);
    }

    output.write(reinterpret_cast<const char *>(compressed_data.data()), dest_len);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input wav> <output compressed>" << std::endl;
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

    compress_data(input_file, output_file);

    input_file.close();
    output_file.close();

    return 0;
}
