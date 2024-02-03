#include "objects.hpp"

class sphere : public object {
    hit intersect(const ray& r) const override {
        hit h;
        h.hit = false;
        return h;
    }
};
