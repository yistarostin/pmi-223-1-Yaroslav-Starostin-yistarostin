#include "controller.h"

#include <iostream>
#include <memory>

#include "filters/crop.h"
#include "filters/edge_detection.h"
#include "filters/filter.h"
#include "filters/gaussian_blur.h"
#include "filters/grayscale.h"
#include "filters/negative.h"
#include "filters/saturate.h"
#include "filters/sharpening.h"
#include "io.h"
#include "parser.h"

void ApplyFilters(Image& image, const std::vector<std::unique_ptr<Filter>>& filters);
std::vector<std::unique_ptr<Filter>> GenerateFilters(const ParserResults& parsed_results);

void PrintUsage() {
    std::cout << "Simple bmp processor\n--------------------\nAvailable Commands:\n\tCrop: '-crop [height] "
                 "[width]'\n\t\tCrops the image starting from left upper corner\n\tSaturate: '-saturate [{plus|minus}] "
                 "[delta]'\n\t\tChanges image saturation on specified delta\n\tSharpening: '-sharp'\n\t\tApplies "
                 "sharpening filter, multiplying each image kernel on specific matrix\n\tEdge detection: '-edge "
                 "[threshold]'\n\t\tSharpens the image and then finds edges higher than threshold "
                 "value\n\tGrayscale\n\t\tTurns image into colors between black and white \n\tNegative\n\t\tInverts "
                 "each pixel\n";
}

void Controller::Run(int argc, char** argv) const {
    if (argc == 1) {
        PrintUsage();
        return;
    }

    ParserResults parsed_results = Parser().Parse(argc, argv);
    auto filters = GenerateFilters(parsed_results);
    auto image = IO(parsed_results.input_path).Read();
    ApplyFilters(image, filters);
    IO(parsed_results.output_path).Write(image);
}

std::vector<std::unique_ptr<Filter>> GenerateFilters(const ParserResults& parsed_results) {
    std::vector<std::unique_ptr<Filter>> filters;
    for (const FilterConfig& fg : parsed_results.filters) {
        std::unique_ptr<Filter> current;
        switch (fg.name) {
            case FilterName::Crop:
                current = std::make_unique<CropFilter>(std::stoul(fg.arguments[0]), std::stoul(fg.arguments[1]));
                break;
            case FilterName::EdgeDetection:
                current = std::make_unique<EdgeDetectionFilter>(std::stod(fg.arguments[0]));
                break;
            case FilterName::Gaussian:
                current = std::make_unique<GaussianBlurFilter>(std::stod(fg.arguments[0]));
                break;
            case FilterName::GrayScale:
                current = std::make_unique<GrayScaleFilter>();
                break;
            case FilterName::Negative:
                current = std::make_unique<NegativeFilter>();
                break;
            case FilterName::Sharpening:
                current = std::make_unique<SharpeningFilter>();
                break;
            case FilterName::Saturate:
                if (fg.arguments.size() != 2 || (fg.arguments[0] != "plus" && fg.arguments[0] != "minus")) {
                    throw std::invalid_argument(
                        "Saturate filter excepts 2 arguments: sign in {plus, minus} and and integer between 0 and 255");
                }
                current = std::make_unique<SaturateFilter>(fg.arguments[0] == "plus", std::stoll(fg.arguments[1]));
                break;
        }
        filters.push_back(std::move(current));
    }
    return filters;
}

void ApplyFilters(Image& image, const std::vector<std::unique_ptr<Filter>>& filters) {
    for (const auto& filter : filters) {
        (*filter.get())(image);  // NOTE: this looks scary, proparbly should to filter.get()->Apply(image) instead of
                                 // operator()(image);
    }
}
