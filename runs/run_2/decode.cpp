#include <iostream>
#include <fstream>
#include <vector>
#include <zlib.h>
#include <chrono>

void decompress_data(std::istream &input, std::ostream &output)
{
    std::vector<char> compressed_data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    uLong compressed_len = compressed_data.size();
    uLong dest_len = compressed_len * 10; // Estimate decompressed size (adjust as needed)
    std::vector<char> decompressed_data(dest_len);

    auto start = std::chrono::high_resolution_clock::now();
    int result = uncompress(reinterpret_cast<Bytef *>(decompressed_data.data()), &dest_len, reinterpret_cast<const Bytef *>(compressed_data.data()), compressed_len);
    auto end = std::chrono::high_resolution_clock::now();

    if (result != Z_OK)
    {
        std::cerr << "Decompression failed" << std::endl;
        exit(1);
    }

    std::chrono::duration<double, std::milli> decompression_time = end - start;
    std::cout << "Decompression time: " << decompression_time.count() << " ms" << std::endl;

    output.write(decompressed_data.data(), dest_len);
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
