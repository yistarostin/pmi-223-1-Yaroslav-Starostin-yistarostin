#include "gaussian_blur.h"

GaussianBlurFilter::GaussianBlurFilter(double sigma) : sigma_(sigma) {
}

void GaussianBlurFilter::operator()(Image &image) const {
}

GaussianBlurFilter::~GaussianBlurFilter() noexcept {
}