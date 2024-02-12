#pragma once

#include "objects/object.hpp"
#include "objects/object_list.hpp"

#include <algorithm>

class bvh_node : public object {
    shared_ptr<object> left, right;
    aabb bbox;

  public:
    bvh_node(const object_list& list)
        : bvh_node(list.objects, 0, list.objects.size(), 0) {}

    bvh_node(
        const std::vector<shared_ptr<object>>& src_objects,
        size_t start,
        size_t end,
        int axis
    ) {
        auto objects = src_objects; // Create a modifiable array of the source
                                    // scene objects

        // int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;

        size_t object_span = end - start;


        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start + 1])) {
                left = objects[start];
                right = objects[start + 1];
            } else {
                left = objects[start + 1];
                right = objects[start];
            }
        } else {
            std::sort(
                objects.begin() + start,
                objects.begin() + end,
                comparator
            );

            auto mid = start + object_span / 2;
            axis = (axis + 1) % 3;
            left = make_shared<bvh_node>(objects, start, mid, axis);
            right = make_shared<bvh_node>(objects, mid, end, axis);
        }

        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    hit intersect(const ray& r, interval ray_t) const override {
        hit h{};
        if (!bbox.hit(r, ray_t))
            return h;

        hit hit_left = left->intersect(r, ray_t);
        hit hit_right = right->intersect(
            r,
            interval(ray_t.min, hit_left.hit ? hit_left.t : ray_t.max)
        );

        if (hit_left.hit && hit_right.hit)
            return hit_left.t < hit_right.t ? hit_left : hit_right;
        else if (hit_left.hit)
            return hit_left;
        else if (hit_right.hit)
            return hit_right;
        else
            return h;
    }

    static bool box_compare(
        const shared_ptr<object> a, const shared_ptr<object> b, int axis
    ) {
        aabb box_a = a->bounding_box();
        aabb box_b = b->bounding_box();
        return box_a.axis(axis).min < box_b.axis(axis).min;
    }

    static bool
    box_x_compare(const shared_ptr<object> a, const shared_ptr<object> b) {
        return box_compare(a, b, 0);
    }

    static bool
    box_y_compare(const shared_ptr<object> a, const shared_ptr<object> b) {
        return box_compare(a, b, 1);
    }

    static bool
    box_z_compare(const shared_ptr<object> a, const shared_ptr<object> b) {
        return box_compare(a, b, 2);
    }

    aabb bounding_box() const override {
        return bbox;
    }
};
