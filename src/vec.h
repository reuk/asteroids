#pragma once

#include <array>
#include <tuple>
#include <ostream>
#include <numeric>
#include <cmath>

template<typename T, std::size_t S>
class Vec: public std::array<T, S> {
public:
    using value_type = T;
    using iterator = typename std::array<T, S>::iterator;
    using const_iterator = typename std::array<T, S>::const_iterator;

    template<typename... U>
    explicit Vec(U&&... t):
        std::array<T, S>({{static_cast<T>(t)...}})
    {}

    friend std::ostream & operator << (std::ostream & os, const Vec & v) {
        os << "{";
        for (auto i = 0; i != S; ++i)
            os << " " << v[i];
        os << " }";
        return os;
    }

private:
    template<typename Fun>
    void map(Fun f) {
        for (auto i = 0; i != S; ++i)
            f((*this)[i]);
    }

    template<typename Fun, typename V>
    void zip_with(Fun f, const V & v) {
        for (auto i = 0; i != S; ++i)
            f((*this)[i], v[i]);
    }

public:
    auto & operator += (const Vec & rhs) {
        zip_with([](auto & a, auto b){a += b;}, rhs);
        return *this;
    }

    auto & operator += (const T & t) {
        map([&t](auto & a){a += t;});
        return *this;
    }

    template<typename V>
    friend Vec operator + (Vec lhs, const V & rhs) {
        return lhs += rhs;
    }

    auto & operator -= (const Vec & rhs) {
        zip_with([](auto & a, auto b){a -= b;}, rhs);
        return *this;
    }

    auto & operator -= (const T & t) {
        map([&t](auto & a){a -= t;});
        return *this;
    }

    template<typename V>
    friend auto operator - (Vec lhs, const V & rhs) {
        return lhs -= rhs;
    }

    auto & operator *= (const Vec & rhs) {
        zip_with([](auto & a, auto b){a *= b;}, rhs);
        return *this;
    }

    auto & operator *= (const T & t) {
        map([&t](auto & a){a *= t;});
        return *this;
    }

    template<typename V>
    friend auto operator * (Vec lhs, const V & rhs) {
        return lhs *= rhs;
    }

    auto & operator /= (const Vec & rhs) {
        zip_with([](auto & a, auto b){a /= b;}, rhs);
        return *this;
    }

    auto & operator /= (const T & t) {
        map([&t](auto & a){a /= t;});
        return *this;
    }

    template<typename V>
    friend auto operator / (Vec lhs, const V & rhs) {
        return lhs /= rhs;
    }

    auto dot(const Vec & v) const {
        auto temp = *this * v;
        return std::accumulate(temp.begin(), temp.end(), static_cast<T>(0));
    }

    auto cross(const Vec & v) const {
        return Vec(
                y() * v.z() - z() * v.y(),
                z() * v.x() - x() * v.z(),
                x() * v.y() - y() * v.x());
    }

    auto sqr_length() const {
        return dot(*this);
    }

    auto length() const {
        return sqrt(sqr_length());
    }

    auto distance(const Vec & v) const {
        return (*this - v).length();
    }

    void normalize() {
        (*this) /= length();
    }

    auto normalized() const {
        auto ret = *this;
        ret.normalize();
        return ret;
    }

    const T & x() const { return (*this)[0]; }
    const T & y() const { return (*this)[1]; }
    const T & z() const { return (*this)[2]; }
    const T & w() const { return (*this)[3]; }

    T & x() { return (*this)[0]; }
    T & y() { return (*this)[1]; }
    T & z() { return (*this)[2]; }
    T & w() { return (*this)[3]; }
};

using Vecf2 = Vec<float, 2>;
using Vecf3 = Vec<float, 3>;
using Vecf4 = Vec<float, 4>;
using Vecd2 = Vec<double, 2>;
using Vecd3 = Vec<double, 3>;
using Vecd4 = Vec<double, 4>;
