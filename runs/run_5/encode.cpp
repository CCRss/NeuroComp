#include <iostream>
#include <fstream>
#include <vector>
#include <FLAC++/encoder.h>
#include <chrono>

class FlacEncoder : public FLAC::Encoder::File
{
public:
    bool encode(const std::vector<int32_t> &data, const std::string &output_file)
    {
        if (!set_verify(true))
            return false;
        if (!set_compression_level(8))
            return false;
        if (!set_channels(1))
            return false;
        if (!set_bits_per_sample(16))
            return false;
        if (!set_sample_rate(20000))
            return false;

        FLAC__StreamEncoderInitStatus init_status = init(output_file.c_str());
        if (init_status != FLAC__STREAM_ENCODER_INIT_STATUS_OK)
            return false;

        const FLAC__int32 *buffer = data.data();
        if (!process_interleaved(buffer, data.size()))
            return false;

        if (!finish())
            return false;

        return true;
    }
};

void compress_data(std::istream &input, const std::string &output_file)
{
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    std::vector<int32_t> samples(buffer.size() / sizeof(int16_t));

    for (size_t i = 0; i < samples.size(); ++i)
    {
        samples[i] = static_cast<int16_t>((buffer[2 * i + 1] << 8) | (buffer[2 * i] & 0xff));
    }

    FlacEncoder encoder;
    auto start = std::chrono::high_resolution_clock::now();
    if (!encoder.encode(samples, output_file))
    {
        std::cerr << "Compression failed" << std::endl;
        exit(1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> compression_time = end - start;
    std::cout << "Compression time: " << compression_time.count() << " ms" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input wav> <output flac>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Error opening input file: " << argv[1] << std::endl;
        return 1;
    }

    compress_data(input_file, argv[2]);

    input_file.close();
    return 0;
}
