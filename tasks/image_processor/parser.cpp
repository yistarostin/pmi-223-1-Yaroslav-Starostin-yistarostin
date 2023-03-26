
#include "parser.h"

ParserResults Parser::Parse(int argc, char *argv[]) const {
    ParserResults parser_results;
    parser_results.input_path = argv[1];
    parser_results.output_path = argv[2];
    bool need_grayscale = false;
    for (int index = 3; index < argc; ++index) {
        if (argv[index][0] == '-') {
            FilterConfig config;
            auto filter_name = std::string(argv[index]).substr(1);
            config.name = FILTERS_MAPPING.at(filter_name);
            if (filter_name == "edge") {
                need_grayscale = true;
            }
            std::vector<std::string> arguments;
            while (index + 1 < argc && argv[index + 1][0] != '-') {
                config.arguments.push_back(std::string(argv[index + 1]));
                ++index;
            }
            //--index; TODO: need that? I guess not
            parser_results.filters.push_back(config);
        }
    }
    if (need_grayscale) {
        parser_results.filters.insert(parser_results.filters.begin(), FilterConfig{GrayScale});
    }
    return parser_results;
}
