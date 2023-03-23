#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum FilterName { Gaussian, Crop, EdgeDetection, GrayScale, Negative, Sharpening };

struct FilterConfig {
    FilterName name;
    std::vector<std::string> arguments;
};

const std::unordered_map<std::string, FilterName> FILTERS_MAPING = {{"crop", Crop},          {"sharp", Sharpening},
                                                                    {"gs", GrayScale},       {"neg", Negative},
                                                                    {"edge", EdgeDetection}, {"blur", Gaussian}};

struct ParserResults {
    std::string input_path;
    std::string output_path;
    std::vector<FilterConfig> filters;
};

class Parser {
public:
    ParserResults Parse(int argc, char *argv[]) const;
};
