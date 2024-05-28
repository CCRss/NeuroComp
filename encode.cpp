#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

struct Match
{
    int offset;
    int length;
    char next_char;
};

std::vector<Match> compress_data(const std::vector<char> &data, int window_size)
{
    std::vector<Match> encoded_data;
    int i = 0;
    while (i < data.size())
    {
        int match_length = 0;
        int match_offset = 0;
        for (int j = std::max(0, i - window_size); j < i; ++j)
        {
            int k = 0;
            while (i + k < data.size() && data[j + k] == data[i + k])
            {
                ++k;
            }
            if (k > match_length)
            {
                match_length = k;
                match_offset = i - j;
            }
        }
        if (match_length == 0)
        {
            encoded_data.push_back({0, 0, data[i]});
            ++i;
        }
        else
        {
            encoded_data.push_back({match_offset, match_length, data[i + match_length]});
            i += match_length + 1;
        }
    }
    return encoded_data;
}

void write_compressed_data(const std::vector<Match> &encoded_data, std::ostream &output)
{
    for (const auto &match : encoded_data)
    {
        output.write(reinterpret_cast<const char *>(&match.offset), sizeof(match.offset));
        output.write(reinterpret_cast<const char *>(&match.length), sizeof(match.length));
        output.put(match.next_char);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input wav> <output lz77>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Error opening input file: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();

    auto start = std::chrono::high_resolution_clock::now();
    auto encoded_data = compress_data(buffer, 4096);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> compression_time = end - start;
    std::cout << "Compression time: " << compression_time.count() << " ms" << std::endl;

    std::ofstream output_file(argv[2], std::ios::binary);
    if (!output_file)
    {
        std::cerr << "Error opening output file: " << argv[2] << std::endl;
        return 1;
    }

    write_compressed_data(encoded_data, output_file);
    output_file.close();

    return 0;
}
