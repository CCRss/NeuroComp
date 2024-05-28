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

std::vector<char> decompress_data(const std::vector<Match> &encoded_data)
{
    std::vector<char> decoded_data;
    for (const auto &match : encoded_data)
    {
        if (match.offset == 0 && match.length == 0)
        {
            decoded_data.push_back(match.next_char);
        }
        else
        {
            int start = decoded_data.size() - match.offset;
            for (int i = 0; i < match.length; ++i)
            {
                decoded_data.push_back(decoded_data[start + i]);
            }
            decoded_data.push_back(match.next_char);
        }
    }
    return decoded_data;
}

std::vector<Match> read_compressed_data(std::istream &input)
{
    std::vector<Match> encoded_data;
    while (input)
    {
        Match match;
        input.read(reinterpret_cast<char *>(&match.offset), sizeof(match.offset));
        input.read(reinterpret_cast<char *>(&match.length), sizeof(match.length));
        match.next_char = input.get();
        if (input)
        {
            encoded_data.push_back(match);
        }
    }
    return encoded_data;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input lz77> <output wav>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Error opening input file: " << argv[1] << std::endl;
        return 1;
    }

    auto encoded_data = read_compressed_data(input_file);
    input_file.close();

    auto start = std::chrono::high_resolution_clock::now();
    auto decoded_data = decompress_data(encoded_data);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> decompression_time = end - start;
    std::cout << "Decompression time: " << decompression_time.count() << " ms" << std::endl;

    std::ofstream output_file(argv[2], std::ios::binary);
    if (!output_file)
    {
        std::cerr << "Error opening output file: " << argv[2] << std::endl;
        return 1;
    }

    output_file.write(decoded_data.data(), decoded_data.size());
    output_file.close();

    return 0;
}
