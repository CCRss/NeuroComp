#include <iostream>
#include <fstream>
#include <vector>
#include <brotli/decode.h>
#include <chrono>

void decompress_data(std::istream &input, std::ostream &output)
{
    std::vector<char> compressed_data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    size_t compressed_len = compressed_data.size();
    size_t decompressed_len = compressed_len * 10; // Initial guess for decompressed size
    std::vector<uint8_t> decompressed_data(decompressed_len);

    BrotliDecoderResult result;
    size_t available_in = compressed_len;
    const uint8_t *next_in = reinterpret_cast<const uint8_t *>(compressed_data.data());
    size_t available_out = decompressed_data.size();
    uint8_t *next_out = decompressed_data.data();
    BrotliDecoderState *state = BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);

    auto start = std::chrono::high_resolution_clock::now();
    result = BrotliDecoderDecompressStream(state, &available_in, &next_in, &available_out, &next_out, nullptr);
    auto end = std::chrono::high_resolution_clock::now();

    if (result == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT)
    {
        size_t offset = next_out - decompressed_data.data();
        decompressed_data.resize(decompressed_data.size() * 2);
        next_out = decompressed_data.data() + offset;
        available_out = decompressed_data.size() - offset;
        result = BrotliDecoderDecompressStream(state, &available_in, &next_in, &available_out, &next_out, nullptr);
    }

    BrotliDecoderDestroyInstance(state);

    if (result != BROTLI_DECODER_RESULT_SUCCESS)
    {
        std::cerr << "Decompression failed with error code: " << result << std::endl;
        exit(1);
    }

    std::chrono::duration<double, std::milli> decompression_time = end - start;
    std::cout << "Decompression time: " << decompression_time.count() << " ms" << std::endl;

    output.write(reinterpret_cast<const char *>(decompressed_data.data()), next_out - decompressed_data.data());
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
