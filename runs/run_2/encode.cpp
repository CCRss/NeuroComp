#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>
#include <chrono>

void compress_data(std::istream &input, std::ostream &output)
{
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    uLong source_len = buffer.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    auto start = std::chrono::high_resolution_clock::now();
    int result = compress(reinterpret_cast<Bytef *>(compressed_data.data()), &dest_len, reinterpret_cast<const Bytef *>(buffer.data()), source_len);
    auto end = std::chrono::high_resolution_clock::now();

    if (result != Z_OK)
    {
        std::cerr << "Compression failed" << std::endl;
        exit(1);
    }

    std::chrono::duration<double, std::milli> compression_time = end - start;
    std::cout << "Compression time: " << compression_time.count() << " ms" << std::endl;

    output.write(compressed_data.data(), dest_len);
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
