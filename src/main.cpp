#include <iostream>
#include <getopt.h>
#include "Grammar.h"
#include "BaseParser.h"
#include "EarleyParser.h"
#include "LR1Parser.h"
#include <fstream>

enum class Algo {
    Earley = 1,
    LR1 = 2
};

int main(int argc, char** argv) {
    const char* short_options = "a:f:v";
    const struct option long_options[] = {
            {"algo", required_argument, nullptr, 'a'},
            {"file", required_argument, nullptr, 'f'},
            {"verbose", no_argument, nullptr, 'v'},
            {nullptr, 0, nullptr, 0}
    };

    Algo algo = Algo::LR1;
    bool read_from_file = false;
    bool verbose = false;
    std::string filename;
    int res;
    while ((res = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (res) {
            case 'a': {
                std::string algo_str = optarg;
                if (algo_str == "Earley") {
                    algo = Algo::Earley;
                } else if (algo_str == "LR1") {
                    algo = Algo::LR1;
                } else {
                    throw std::runtime_error("Unknown algo - '" + algo_str + "'");
                }
                break;
            };
            case 'f': {
                read_from_file = true;
                filename = optarg;
                break;
            }
            case 'v': {
                verbose = true;
                break;
            }
            default: {
                throw std::runtime_error("Unknown argument");
            }
        }
    }

    if (verbose) {
        std::cout << "Selected algo - " << (algo == Algo::LR1 ? "LR1" : "Earley") << '\n';
        if (read_from_file) {
            std::cout << "Read from file - " << filename << '\n';
        } else {
            std::cout << "Read from standard input\n";
        }
    }

    Grammar grammar;
    if (read_from_file) {
        std::ifstream fin(filename);
        grammar.read(fin);
        fin.close();
    } else {
        grammar.read();
    }

    BaseParser* parser;
    if (algo == Algo::LR1) {
        parser = new LR1Parser;
    } else {
        parser = new EarleyParser;
    }
    parser->fit(grammar);

    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        std::cout << (parser->predict(word) ? "Yes" : "No") << '\n';
    }
    return 0;
}
