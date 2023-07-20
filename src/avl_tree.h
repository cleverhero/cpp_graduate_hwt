#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "nodes.h"
#include "binary_search_tree.h"

using std::vector;
using std::unique_ptr, std::make_unique;
using std::optional, std::nullopt;


namespace hwt {
    template<
        typename MT = metadata::Avl_h,
        typename = typename MT::avl_trait
    >
    class AVLTree: public BinarySearchTree<MT> {
    protected:
        using BST = BinarySearchTree<MT>;
    
    private:
        int get_subtree_bfactor(id_opt_t node_id) const {
            if (!node_id)
                return 0;

            auto right_id = this->get_right_id(node_id.value());
            auto left_id = this->get_left_id(node_id.value());
            return this->get_metadata(right_id).avl_h - this->get_metadata(left_id).avl_h;
        }

        id_t balance_node(id_t node_id) {
            auto left_id = this->get_left_id(node_id);
            auto right_id = this->get_right_id(node_id);
            auto bfactor = get_subtree_bfactor(node_id);

            if (bfactor == 2) {
                if (get_subtree_bfactor(right_id) < 0)
                    this->link_right(node_id, this->right_rotate(right_id.value()) );
                return this->left_rotate(node_id);
            }
            if (bfactor == -2) {
                if (get_subtree_bfactor(left_id) > 0)
                    this->link_left(node_id, this->left_rotate(left_id.value()) );
                return this->right_rotate(node_id);
            }
            
            return node_id;
        }
    
    protected:
        id_t insert(const id_opt_t curr_node_id, const int key) override {
            auto node_id = BST::insert(curr_node_id, key);
            return balance_node(node_id);
        };

    public:
        using BST::insert;
    };
}