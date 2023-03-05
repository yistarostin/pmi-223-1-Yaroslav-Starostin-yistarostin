#include "poly.h"

Poly::Poly(std::initializer_list<std::pair<PowersType, CoeffsType>> powers_with_coeffs)
    : coeffs_(powers_with_coeffs.begin(), powers_with_coeffs.end()){};

Poly::Poly(std::initializer_list<CoeffsType> coeffs_values) {
    for (std::size_t i = 0; CoeffsType coef : coeffs_values) {
        if (coef != 0) {
            coeffs_[i] = coef;
        }
        ++i;
    }
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
    for (auto it = poly.coeffs_.rbegin(); it != poly.coeffs_.rend(); ++it) {
        const auto& [power, coeff] = *it;
        if (power != 0) {
            os << " " << ((it != poly.coeffs_.rbegin() && coeff > 0) ? "+ " : "") << coeff << "x^" << power;
        }
    }
    if (poly.coeffs_.count(0) != 0) {
        os << " " << (poly.coeffs_.at(0) > 0 ? "+" : "-") << " " << std::abs(poly.coeffs_.at(0));
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

void Poly::AddWithMulitplier(const Poly& other, CoeffsType lambda, PowersType offset) {
    for (auto& [power, coeff] : other.coeffs_) {
        if (coeffs_.count(power + offset) == 0) {
            coeffs_[power + offset] = 0;
        }
        coeffs_[power + offset] = coeffs_[power + offset] + coeff * lambda;
    }
}

Poly Poly::operator+(const Poly& other) const {
    Poly now{*this};
    now += other;
    return now;
}

Poly Poly::operator-(const Poly& other) const {
    Poly now{*this};
    now -= other;
    return now;
}

// F*ck FFT, all my homies use O(n^2) algorithms
Poly Poly::operator*(const Poly& other) const {
    if (coeffs_.empty() || other.coeffs_.empty()) {  // in case some polynomial is identically zero
        return Poly{};
    }
    Poly res;
    for (auto& [power, coeff] : other.coeffs_) {
        res.AddWithMulitplier(other, coeff, power);
    }
    res.TrimZeros();
    return res;
}

// This creates an extra copy, but we anyway do O(n^2), so this is not a bottle-neck
// The best way would be to do *= using Karatsuba/FFT, but it is hard
Poly& Poly::operator*=(const Poly& other) {
    return *this = (*this) * other;
}

bool Poly::operator==(const Poly& other) const {
    bool eq = coeffs_ == other.coeffs_;
    return eq;
}

CoeffsType Pow(CoeffsType x, PowersType n) {
    if (n == 0) {
        return 1;
    }
    if (n == 1) {
        return x;
    }
    if (n % 2 == 0) {
        return Pow(x * x, n / 2);
    } else {
        return Pow(x * x, n / 2) * x;
    }
}

CoeffsType Poly::operator()(CoeffsType x_value) const {
    CoeffsType poly_value = 0;
    for (const auto& [power, coef] : coeffs_) {
        poly_value += Pow(x_value, power) * coef;
    }
    return poly_value;
}

Poly Poly::operator-() const {
    Poly reversed_sign{*this};
    for (auto& [_, coeff] : reversed_sign.coeffs_) {
        coeff *= -1;
    }
    return reversed_sign;
}

void Poly::TrimZeros() {
    for (auto it = coeffs_.begin(); it != coeffs_.end();) {
        if (it->second == 0) {
            it = coeffs_.erase(it);
        } else {
            ++it;
        }
    }
}
