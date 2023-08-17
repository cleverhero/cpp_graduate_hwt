#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "nodes.h"
#include "binary_search_tree.h"
#include "avl_tree.h"


using std::vector;
using std::unique_ptr, std::make_unique;
using std::optional, std::nullopt;


namespace hwt {
    template<
        template<typename MT>
        typename TreeType = AVLTree,
        typename MT = metadata::Full,
        typename = typename MT::size_trait
    >
    class OrderStatisticTree: public TreeType<MT> {
    protected:
        using BST = TreeType<MT>;

        using BST::nodes;
        using BST::root_id;

        using BST::get_right_id;
        using BST::get_left_id;
        using BST::get_metadata;

    public:
        using TreeType<MT>::insert;

        optional<int> select(int k) const {
            if (k < 1 || k > nodes.size())
                return nullopt;

            auto curr_node_id = root_id;
            while (curr_node_id) {
                auto node_id = curr_node_id.value();
                auto right_id = get_right_id(node_id);

                auto subtree_size = get_metadata(node_id).subtree_size;
                auto rsubtree_size = get_metadata(right_id).subtree_size;
                int extra_k = k - subtree_size + rsubtree_size;

                if (extra_k > 0) {
                    curr_node_id = right_id;
                    k = extra_k;
                }
                else if (extra_k < 0)
                    curr_node_id = get_left_id(node_id);
                else
                    return nodes[node_id]->key;
            }

            return nullopt;
        }

        int rank(const int k) const {
            if (!root_id)
                return 0;

            auto curr_node_id = root_id;
            int result = 0;
            while (curr_node_id) {
                auto node_id = curr_node_id.value();
                auto right_id = get_right_id(node_id);

                auto subtree_size = get_metadata(node_id).subtree_size;
                auto rsubtree_size = get_metadata(right_id).subtree_size;

                if (k > nodes[node_id]->key) {
                    result += subtree_size - rsubtree_size;
                    curr_node_id = right_id;
                }
                else if (k < nodes[node_id]->key) {
                    curr_node_id = get_left_id(node_id);
                }
                else {
                    result += subtree_size - rsubtree_size - 1;
                    break;
                }
            }

            return result;
        }
    };
}