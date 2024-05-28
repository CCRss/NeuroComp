#include <iostream>
#include <fstream>
#include <vector>
#include <FLAC++/decoder.h>
#include <chrono>

class FlacDecoder : public FLAC::Decoder::File
{
public:
    std::vector<int32_t> decoded_data;

    FLAC__StreamDecoderWriteStatus write_callback(const FLAC__Frame *frame, const FLAC__int32 *const buffer[]) override
    {
        size_t num_samples = frame->header.blocksize * frame->header.channels;
        decoded_data.insert(decoded_data.end(), buffer[0], buffer[0] + num_samples);
        return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
    }

    void error_callback(FLAC__StreamDecoderErrorStatus status) override
    {
        std::cerr << "Got error callback: " << FLAC__StreamDecoderErrorStatusString[status] << std::endl;
    }

    bool decode(const std::string &input_file)
    {
        FLAC__StreamDecoderInitStatus init_status = init(input_file.c_str());
        if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK)
            return false;

        if (!process_until_end_of_stream())
            return false;

        return true;
    }
};

void decompress_data(const std::string &input_file, std::ostream &output)
{
    FlacDecoder decoder;
    auto start = std::chrono::high_resolution_clock::now();
    if (!decoder.decode(input_file))
    {
        std::cerr << "Decompression failed" << std::endl;
        exit(1);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> decompression_time = end - start;
    std::cout << "Decompression time: " << decompression_time.count() << " ms" << std::endl;

    for (int32_t sample : decoder.decoded_data)
    {
        int16_t s = static_cast<int16_t>(sample);
        output.put(s & 0xff);
        output.put((s >> 8) & 0xff);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input flac> <output wav>" << std::endl;
        return 1;
    }

    std::ofstream output_file(argv[2], std::ios::binary);
    if (!output_file)
    {
        std::cerr << "Error opening output file: " << argv[2] << std::endl;
        return 1;
    }

    decompress_data(argv[1], output_file);

    output_file.close();
    return 0;
}
