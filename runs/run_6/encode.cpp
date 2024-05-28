#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <map>

class PPMEncoder
{
public:
    void encode(std::istream &input, std::ostream &output)
    {
        std::vector<char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        std::map<std::string, int> context_count;
        std::string context = "";

        auto start = std::chrono::high_resolution_clock::now();
        for (char c : buffer)
        {
            context += c;
            if (context.length() > 4)
            {
                context.erase(0, 1);
            }
            context_count[context]++;
            output.put(c); // Simplified output for demonstration
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> compression_time = end - start;
        std::cout << "Compression time: " << compression_time.count() << " ms" << std::endl;
    }
};

void compress_data(std::istream &input, std::ostream &output)
{
    PPMEncoder encoder;
    encoder.encode(input, output);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input wav> <output ppm>" << std::endl;
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
