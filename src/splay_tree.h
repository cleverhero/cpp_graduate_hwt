#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "nodes.h"
#include "binary_search_tree.h"

using std::optional, std::nullopt;
using std::unique_ptr, std::make_unique;
using std::vector;

namespace hwt
{
    template<
        typename MT = metadata::Empty,
        typename = typename MT::base_trait
    >
    class SplayTree : public BinarySearchTree<MT> {
    protected:
        using BST = BinarySearchTree<MT>;

        using BST::nodes;
        using BST::root_id;

        using BST::set_root;

        using BST::get_right_id;
        using BST::get_left_id;
        using BST::get_metadata;

        using BST::link_right;
        using BST::link_left;

        using BST::right_rotate;
        using BST::left_rotate;


    public:
        bool find(int key) override {
            if (!root_id)
                return false;

            splay(key);
            return (nodes[root_id.value()]->key == key);
        }

        void splay(int key) {
            if (!root_id)
                return;

            id_opt_t right_min = nullopt;
            id_opt_t right_head = nullopt;
            id_opt_t left_max = nullopt;
            id_opt_t left_head = nullopt;

            while (true) {
                id_t curr_root_id = root_id.value();
                auto left_id = get_left_id(curr_root_id);
                auto right_id = get_right_id(curr_root_id);
                auto root_key = nodes[curr_root_id]->key;

                if (key < root_key) {
                    if (!left_id)
                        break;
                    if (key < nodes[left_id.value()]->key) {
                        set_root(right_rotate(curr_root_id));
                        curr_root_id = root_id.value();

                        if (!get_left_id(curr_root_id))
                            break;
                    }
                    if (!right_min)
                        right_head = root_id;
                    else
                        link_left(right_min.value(), root_id);

                    right_min = root_id;
                    set_root(get_left_id(curr_root_id).value());
                    curr_root_id = root_id.value();

                    if (right_min)
                        link_left(right_min.value(), nullopt);
                }
                else if (key > root_key) {
                    if (!right_id)
                        break;
                    if (key > nodes[right_id.value()]->key) {
                        set_root(left_rotate(curr_root_id));
                        curr_root_id = root_id.value();

                        if (!get_right_id(curr_root_id))
                            break;
                    }

                    if (!left_max)
                        left_head = root_id;
                    else
                        link_right(left_max.value(), root_id);

                    left_max = root_id;
                    set_root(get_right_id(curr_root_id).value());
                    curr_root_id = root_id.value();

                    if (left_max)
                        link_right(left_max.value(), nullopt);
                }
                else
                    break;
            }
            if (!left_max)
                left_head = get_left_id(root_id.value());
            else
                link_right(left_max.value(), get_left_id(root_id.value()));

            if (!right_min)
                right_head = get_right_id(root_id.value());
            else
                link_left(right_min.value(), get_right_id(root_id.value()));

            link_left(root_id.value(), left_head);
            link_right(root_id.value(), right_head);
        }

    public:
        using BST::insert;
    };
}