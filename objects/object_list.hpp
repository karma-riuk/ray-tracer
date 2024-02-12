#pragma once

#include "object.hpp"

#include <memory>
#include <vector>

class object_list : public object {
    aabb bbox;

  public:
    std::vector<std::shared_ptr<object>> objects;

    object_list() {}

    object_list(shared_ptr<object> object) {
        add(object);
    }

    void clear() {
        objects.clear();
    }

    void add(std::shared_ptr<object> obj) {
        objects.push_back(obj);
        bbox = aabb(bbox, obj->bounding_box());
    }

    hit intersect(const ray& r, interval ray_t) const override {
        hit tmp_hit;
        hit hit;
        auto closest_so_far = ray_t.max;

        for (const auto& obj : objects) {
            tmp_hit = obj->intersect(r, interval(ray_t.min, closest_so_far));
            if (tmp_hit.hit) {
                closest_so_far = tmp_hit.t;
                hit = tmp_hit;
            }
        }

        return hit;
    }

    aabb bounding_box() const override {
        return bbox;
    }
};
