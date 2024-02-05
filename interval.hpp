#pragma once

#include "rtweekend.hpp"

class interval {
  public:
    double min;
    double max;

    interval(): min(+infinity), max(-infinity) {}

    interval(double min, double max): min(min), max(max) {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);
