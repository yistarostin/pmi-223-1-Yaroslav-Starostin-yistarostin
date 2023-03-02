#pragma once
#include <array>
#include <cstdint>
#include <map>
#include <ostream>
#include <utility>
#include <vector>

class Poly {
public:
    Poly() = default;

    Poly(std::initializer_list<int32_t> coeffs);
    Poly(std::initializer_list<std::pair<size_t, int32_t>> coeffs);

    Poly(const Poly& other) = default;

    Poly(Poly&& other) = default;

    Poly& operator=(const Poly& other);

    ~Poly() = default;
    int64_t operator()(int32_t x_value) const;
    bool operator==(const Poly& other) const = default;
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
    std::vector<int32_t> coeffs_;
    void TrimZeros();
    void AddWithMulitplier(const Poly& other, std::int32_t delta, size_t offset = 0);
};
