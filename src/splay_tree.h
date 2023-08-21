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
        typename = std::enable_if_t<
            std::is_base_of_v<metadata::BaseTrait, typename MT::metadata_trait>
        >
    >
    class SplayTree : public BinarySearchTree<MT> {
    protected:
        using BST = BinarySearchTree<MT>;

        using BST::nodes;
        using BST::root_id;

        using BST::update_node;

        using BST::get_right_id;
        using BST::get_left_id;
        using BST::get_metadata;

        using BST::link_right;
        using BST::link_left;

        using BST::right_rotate;
        using BST::left_rotate;


    private:
        struct TmpTree {
            id_opt_t head;
            id_opt_t tail;
        };

        void to_right_tree(id_opt_t node_id, TmpTree& right) noexcept {
            if (!right.head)
                right.head = node_id;
            else {
                nodes[right.tail.value()]->left_id = node_id;
                if (node_id)
                    nodes[node_id.value()]->parent_id = right.tail;

                update_node(right.tail.value());
            }
            right.tail = node_id;
        };

        void to_left_tree(id_opt_t node_id, TmpTree& left) noexcept {
            if (!left.head)
                left.head = node_id;
            else {
                nodes[left.tail.value()]->right_id = node_id;
                if (node_id)
                    nodes[node_id.value()]->parent_id = left.tail;

                update_node(left.tail.value());
            }
            left.tail = node_id;
        };

        id_t break_left(id_t topnode_id, TmpTree& left) noexcept {
            id_t new_topnode_id = get_right_id(topnode_id).value();
            nodes[topnode_id]->right_id = nullopt;

            to_left_tree(topnode_id, left);

            update_node(topnode_id);
            return new_topnode_id;
        };

        id_t break_right(id_t topnode_id, TmpTree& right) noexcept {
            id_t new_topnode_id = get_left_id(topnode_id).value();
            nodes[topnode_id]->left_id = nullopt;

            to_right_tree(topnode_id, right);

            update_node(topnode_id);
            return new_topnode_id;
        };

        void assemble(TmpTree& left, TmpTree& right) noexcept {
            auto left_id = get_left_id(root_id.value());
            to_left_tree(left_id, left);

            auto right_id = get_right_id(root_id.value());
            to_right_tree(right_id, right);

            nodes[root_id.value()]->left_id = left.head;
            nodes[root_id.value()]->right_id = right.head;
            this->update_node(root_id.value());
        }


    public:
        bool find(int key) override {
            if (!root_id)
                return false;

            splay(key);
            return (nodes[root_id.value()]->key == key);
        }

        void splay(const int key) noexcept {
            TmpTree tmp_left, tmp_right;

            while(true) {
                id_t curr_root_id = root_id.value();
                const int root_key = nodes[curr_root_id]->key;

                if (key < root_key) {
                    auto left_id = get_left_id(curr_root_id);
                    if (!left_id)
                        break;

                    if (key < nodes[left_id.value()]->key)
                        curr_root_id = right_rotate(curr_root_id);
                    curr_root_id = break_right(curr_root_id, tmp_right);
                }
                else if (key > root_key) {
                    auto right_id = get_right_id(curr_root_id);
                    if (!right_id)
                        break;

                    if (key > nodes[right_id.value()]->key)
                        curr_root_id = left_rotate(curr_root_id);
                    curr_root_id = break_left(curr_root_id, tmp_left);
                }
                else
                    break;

                root_id = curr_root_id;
                nodes[curr_root_id]->parent_id = nullopt;
            }

            assemble(tmp_left, tmp_right);
        }

    public:
        using BST::insert;
    };
}