#pragma once
#include <map>
#include <ostream>

using PowersType = size_t;
using CoeffsType = int64_t;
using PolynomialContainer = std::map<PowersType, CoeffsType>;

class Poly {
public:
    Poly() = default;
    Poly(std::initializer_list<CoeffsType> coeffs);
    Poly(std::initializer_list<std::pair<PowersType, CoeffsType>> coeffs);

    Poly(const Poly& other) = default;

    Poly(Poly&& other) = default;

    Poly& operator=(const Poly& other);

    ~Poly() = default;
    CoeffsType operator()(CoeffsType x_value) const;
    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const = default;
    Poly operator+(const Poly& other) const;
    Poly operator-(const Poly& other) const;
    Poly& operator+=(const Poly& other);
    Poly& operator-=(const Poly& other);
    Poly operator*(const Poly& other) const;
    Poly& operator*=(const Poly& other);
    Poly operator-() const;
    friend std::ostream& operator<<(std::ostream& os, const Poly& poly);

private:
    PolynomialContainer coeffs_;
    void TrimZeros();
    void AddWithMulitplier(const Poly& other, CoeffsType delta, PowersType offset = 0);
};
