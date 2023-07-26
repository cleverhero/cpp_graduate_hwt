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
    class SplayTree : public BinarySearchTree<MT>
    {
    protected:
        using BST = BinarySearchTree<MT>;

    public:
        bool find(int key) {
            if (!this->root_id)
                return false;

            this->splay(key);
            return (this->nodes[this->root_id.value()]->key == key);
        }

        void splay(int key) {
            if (!this->root_id)
                return;

            id_opt_t right_min = nullopt;
            id_opt_t right_head = nullopt;
            id_opt_t left_max = nullopt;
            id_opt_t left_head = nullopt;

            while (true)
            {
                id_t curr_root_id = this->root_id.value();
                auto left_id = this->get_left_id(curr_root_id);
                auto right_id = this->get_right_id(curr_root_id);
                auto root_key = this->nodes[curr_root_id]->key;

                if (key < root_key)
                {
                    if (!left_id)
                        break;
                    if (key < this->nodes[left_id.value()]->key)
                    {
                        this->set_root(this->right_rotate(curr_root_id));
                        curr_root_id = this->root_id.value();

                        if (!this->get_left_id(curr_root_id))
                            break;
                    }
                    if (!right_min)
                        right_head = this->root_id;
                    else
                        this->link_left(right_min.value(), this->root_id);

                    right_min = this->root_id;
                    this->set_root(this->get_left_id(curr_root_id).value());
                    curr_root_id = this->root_id.value();

                    if (right_min)
                        this->link_left(right_min.value(), nullopt);
                }
                else if (key > root_key)
                {
                    if (!right_id)
                        break;
                    if (key > this->nodes[right_id.value()]->key)
                    {
                        this->set_root(this->left_rotate(curr_root_id));
                        curr_root_id = this->root_id.value();

                        if (!this->get_right_id(curr_root_id))
                            break;
                    }

                    if (!left_max)
                        left_head = this->root_id;
                    else
                        this->link_right(left_max.value(), this->root_id);

                    left_max = this->root_id;
                    this->set_root(this->get_right_id(curr_root_id).value());
                    curr_root_id = this->root_id.value();

                    if (left_max)
                        this->link_right(left_max.value(), nullopt);
                }
                else
                    break;
            }
            if (!left_max)
                left_head = this->get_left_id(this->root_id.value());
            else
                this->link_right(left_max.value(), this->get_left_id(this->root_id.value()));

            if (!right_min)
                right_head = this->get_right_id(this->root_id.value());
            else
                this->link_left(right_min.value(), this->get_right_id(this->root_id.value()));

            this->link_left(this->root_id.value(), left_head);
            this->link_right(this->root_id.value(), right_head);
        }

    public:
        using BST::insert;
    };
}