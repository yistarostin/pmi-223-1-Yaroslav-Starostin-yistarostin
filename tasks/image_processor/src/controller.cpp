#include "controller.h"

#include <memory>

#include "filters/crop.h"
#include "filters/edge_detection.h"
#include "filters/filter.h"
#include "filters/gaussian_blur.h"
#include "filters/grayscale.h"
#include "filters/negative.h"
#include "filters/sharpening.h"
#include "io.h"
#include "parser.h"

void ApplyFilters(Image& image, const std::vector<std::unique_ptr<Filter>>& filters);
std::vector<std::unique_ptr<Filter>> GenerateFilters(const ParserResults& parsed_results);

bool Controller::Run(int argc, char** argv) const {
    ParserResults parsed_results = Parser().Parse(argc, argv);
    auto filters = GenerateFilters(parsed_results);
    auto io_result = IO(parsed_results.input_path).Read();
    Image image = Image(io_result);
    ApplyFilters(image, filters);
    auto success = IO(parsed_results.output_path).Write(image.ToBytes());
    return success;
}

std::vector<std::unique_ptr<Filter>> GenerateFilters(const ParserResults& parsed_results) {
    std::vector<std::unique_ptr<Filter>> filters;
    for (const FilterConfig& fg : parsed_results.filters) {
        std::unique_ptr<Filter> current;
        switch (fg.name) {
            case FilterName::Crop:
                // current = make_unqiue(CropFilter(std::stoul(fg.arguments[0]), std::stoul(fg.arguments[1])));
                current = std::make_unique<CropFilter>(std::stoul(fg.arguments[0]), std::stoul(fg.arguments[1]));
                break;
            case FilterName::EdgeDetection:
                current = std::make_unique<EdgeDetectionFilter>(std::stoul(fg.arguments[0]));
                break;
            case FilterName::Gaussian:
                current = std::make_unique<GaussianBlurFilter>();
                break;
            case FilterName::GrayScale:
                current = std::make_unique<GrayScaleFilter>();
            case FilterName::Negative:
                current = std::make_unique<NegativeFilter>();
                break;
            case FilterName::Sharpening:
                current = std::make_unique<SharpeningFilter>();
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
