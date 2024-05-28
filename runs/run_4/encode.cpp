#include <iostream>
#include <fstream>
#include <vector>
#include <brotli/encode.h>
#include <chrono>

void compress_data(std::istream &input, std::ostream &output)
{
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    size_t source_len = buffer.size();
    size_t max_compressed_size = BrotliEncoderMaxCompressedSize(source_len);
    std::vector<uint8_t> compressed_data(max_compressed_size);

    size_t compressed_size = compressed_data.size();

    auto start = std::chrono::high_resolution_clock::now();
    BROTLI_BOOL result = BrotliEncoderCompress(BROTLI_DEFAULT_QUALITY, BROTLI_DEFAULT_WINDOW, BROTLI_MODE_GENERIC,
                                               source_len, reinterpret_cast<const uint8_t *>(buffer.data()),
                                               &compressed_size, compressed_data.data());
    auto end = std::chrono::high_resolution_clock::now();

    if (result == BROTLI_FALSE)
    {
        std::cerr << "Compression failed" << std::endl;
        exit(1);
    }

    std::chrono::duration<double, std::milli> compression_time = end - start;
    std::cout << "Compression time: " << compression_time.count() << " ms" << std::endl;

    output.write(reinterpret_cast<const char *>(compressed_data.data()), compressed_size);
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
