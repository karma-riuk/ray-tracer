#pragma once

#include "objects/object.hpp"
#include "objects/object_list.hpp"

#include <algorithm>

class bvh_node : public object {
    shared_ptr<object> left, right;
    aabb bbox;


  public:
    static inline size_t LEAF_SIZE;

    bvh_node(const object_list& list)
        : bvh_node(list.objects, 0, list.objects.size(), 0) {}

    bvh_node(
        const std::vector<shared_ptr<object>>& src_objects,
        size_t start,
        size_t end,
        int axis
    ) {
        auto objects = src_objects; // Create a modifiable array of the
                                    // source scene objects

        // int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;

        size_t object_span = end - start;
        auto mid = start + object_span / 2;

        if (object_span < 2 * LEAF_SIZE) {
            shared_ptr<object_list> tmp_left = make_shared<object_list>();
            for (size_t i = start; i < mid; i++)
                tmp_left->add(objects[i]);
            left = tmp_left;

            shared_ptr<object_list> tmp_right = make_shared<object_list>();
            for (size_t i = mid; i < end; i++)
                tmp_right->add(objects[i]);
            right = tmp_right;
        } else {
            std::sort(
                objects.begin() + start,
                objects.begin() + end,
                comparator
            );

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

    int calculate_depth() const override {
        return std::max(left->calculate_depth(), right->calculate_depth()) + 1;
    }

    // print the bvh_node and use tabs to indent the children
    std::ostream& operator<<(std::ostream& out) const override {
        out << "bvh_node(\n";
        out << "\t" << *left << ",\n";
        out << "\t" << *right << "\n";
        out << ")";
        return out;
    }
};
