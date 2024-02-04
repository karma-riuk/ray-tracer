#pragma once

#include "object.hpp"

#include <memory>
#include <vector>

class object_list : public object {
  public:
    std::vector<std::shared_ptr<object>> objects;

    void clear() {
        objects.clear();
    }

    void add(std::shared_ptr<object> obj) {
        objects.push_back(obj);
    }

    hit intersect(const ray& r, double t_min, double t_max) const override {
        hit tmp_hit;
        hit hit;
        auto closest_so_far = t_max;

        for (const auto& obj : objects) {
            tmp_hit = obj->intersect(r, t_min, closest_so_far);
            if (tmp_hit.hit) {
                closest_so_far = tmp_hit.t;
                hit = tmp_hit;
            }
        }

        return hit;
    }
};
