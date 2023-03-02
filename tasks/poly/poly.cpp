#include "poly.h"

#include <algorithm>

Poly::Poly(std::initializer_list<std::pair<std::size_t, int64_t>> powers_with_coeffs) {
    std::size_t maximum_power = std::max_element(powers_with_coeffs.begin(), powers_with_coeffs.end())->first;
    coeffs_.resize(maximum_power + 1);
    for (const auto& [power, coeff] : powers_with_coeffs) {
        coeffs_[power] = coeff;
    }
    TrimZeros();
};

Poly::Poly(std::initializer_list<int32_t> coeffs_values) : coeffs_(coeffs_values.begin(), coeffs_values.end()) {
    TrimZeros();
}

Poly& Poly::operator=(const Poly& other) {
    coeffs_ = other.coeffs_;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Poly& poly) {
    if (poly.coeffs_.empty()) {
        return os << "y = 0";
    }
    os << "y =";
    for (size_t degree = poly.coeffs_.size() - 1; degree > 0; --degree) {
        if (poly.coeffs_[degree] == 0) {
            continue;
        }
        os << " " << ((degree != poly.coeffs_.size() - 1 && poly.coeffs_[degree] > 0) ? "+ " : "")
           << poly.coeffs_[degree] << "x^" << degree;
    }
    if (poly.coeffs_[0] != 0) {
        os << " " << (poly.coeffs_[0] > 0 ? "+" : "-") << " " << std::abs(poly.coeffs_[0]);
    }
    return os;
}

Poly& Poly::operator+=(const Poly& other) {
    AddWithMulitplier(other, 1);
    TrimZeros();
    return *this;
}

Poly& Poly::operator-=(const Poly& other) {
    AddWithMulitplier(other, -1);
    TrimZeros();
    return *this;
}

void Poly::AddWithMulitplier(const Poly& other, int64_t lambda, size_t offset) {
    if (other.coeffs_.size() + offset > coeffs_.size()) {
        coeffs_.resize(other.coeffs_.size() + offset);
    }
    for (size_t degree = 0; degree < other.coeffs_.size(); ++degree) {
        coeffs_[degree + offset] += other.coeffs_[degree] * lambda;
    }
}

Poly Poly::operator+(const Poly& other) const {
    Poly now{*this};
    now += other;
    now.TrimZeros();
    return now;
}

Poly Poly::operator-(const Poly& other) const {
    Poly now{*this};
    now -= other;
    now.TrimZeros();
    return now;
}

// F*ck FFT, all my homies use O(n^2) algorithms
Poly Poly::operator*(const Poly& other) const {
    Poly res;
    res.coeffs_.resize((coeffs_.size() - 1) * (other.coeffs_.size() - 1));
    for (size_t c = 0; c < coeffs_.size(); ++c) {
        res.AddWithMulitplier(other, coeffs_[c], c);
    }
    res.TrimZeros();
    return res;
}

// This creates an extra copy, but we anyway do O(n^2), so this is not a bottle-neck
// The best way would be to do *= using Karatsuba/FFT, but it is hard
Poly& Poly::operator*=(const Poly& other) {
    *this = (*this) * other;
    TrimZeros();
    return *this;
}

int64_t Poly::operator()(__int128_t x_value) const {
    int64_t poly_value = 0;
    __int128_t current_value_degree = 1;  // x_value^0 is always 1
    for (size_t degree = 0; degree < coeffs_.size(); ++degree) {
        poly_value += coeffs_[degree] * current_value_degree;
        current_value_degree *= x_value;
    }
    return poly_value;
}

Poly Poly::operator-() const {
    Poly reversed_sign{*this};
    for (int64_t& coeff : reversed_sign.coeffs_) {
        coeff *= -1;
    }
    return reversed_sign;
}

void Poly::TrimZeros() {
    while (!coeffs_.empty() && coeffs_.back() == 0) {
        coeffs_.pop_back();
    }
}
