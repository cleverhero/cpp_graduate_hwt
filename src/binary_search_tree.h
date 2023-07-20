#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "nodes.h"


using std::vector;
using std::unique_ptr, std::make_unique;
using std::optional, std::nullopt;


namespace hwt {
    template<
        typename MT = metadata::Empty,
        typename = typename MT::base_trait
    >
    class BinarySearchTree {
    private:
        using NodeType = Node_<MT>;
        using unique_ptr_node = unique_ptr<NodeType>;
        using BST = BinarySearchTree<MT>;

        MT after_leaf = MT::AfterLeaf();

    protected:
        id_t id;
        id_opt_t root_id;
        vector<unique_ptr_node> nodes;

    public:
        BinarySearchTree():
            id(0),
            root_id(nullopt),
            nodes()
        {}

        virtual ~BinarySearchTree() = default;

        BinarySearchTree(const BST& rhs):
            id(0),
            root_id(rhs.root_id),
            nodes()
        {
            vector<unique_ptr_node> tmp_nodes(rhs.nodes.size());
            for (int i = 0; i < rhs.nodes.size(); i++)
                tmp_nodes[i] = make_unique<NodeType>(id, *rhs.nodes[i]);

            this->nodes.swap(tmp_nodes);
        }

        BST& operator=(const BST& rhs) {
            BST tmp(rhs);

            std::swap(*this, tmp);
            return *this;
        }

        BinarySearchTree(BST&& rhs) noexcept:
            id(rhs.id),
            root_id(rhs.root_id)
        {
            nodes.swap(rhs.nodes);
        }

        BST& operator=(BST&& rhs) noexcept {
            this->nodes.swap(rhs.nodes);

            id = rhs.id;
            root_id = rhs.root_id;
            return *this;
        }

        void insert(const int key) { root_id = insert(root_id, key); }

        int nodes_count() {return nodes.size(); }

        // bool find(const int key) const;

    protected:
        void link_left(id_t parent_id, id_opt_t child_id) {
            nodes[parent_id]->left_id = child_id;

            if (child_id)
                nodes[child_id.value()]->parent_id = parent_id;
        }

        void link_right(id_t parent_id, id_opt_t child_id) {
            nodes[parent_id]->right_id = child_id;

            if (child_id)
                nodes[child_id.value()]->parent_id = parent_id;
        }

        void unlink_node(id_t node_id) {
            auto parent_id = get_parent_id(node_id);
            if (!parent_id)
                return;

            if (get_left_id(parent_id.value()) == node_id)
                nodes[parent_id.value()]->left_id = nullopt;
            else
                nodes[parent_id.value()]->right_id = nullopt;

            nodes[node_id]->parent_id = nullopt;
        }

        id_opt_t get_left_id(id_t id) const { return nodes[id]->left_id; }
        id_opt_t get_parent_id(id_t id) const { return nodes[id]->parent_id; }
        id_opt_t get_right_id(id_t id) const { return nodes[id]->right_id; }

        const MT& get_metadata(id_opt_t id) const {
            if (!id)
                return after_leaf;

            return nodes[id.value()]->metadata;
        }
        
        void update_node(id_t node_id) {
            auto left_id = get_left_id(node_id);
            auto right_id = get_right_id(node_id);
            nodes[node_id]->metadata.update(
                get_metadata(left_id),
                get_metadata(right_id)
            );
        };

        id_t right_rotate(id_t topnode_id) {
            id_t new_topnode_id = get_left_id(topnode_id).value();

            unlink_node(new_topnode_id);
            link_left(topnode_id, get_right_id(new_topnode_id));
            link_right(new_topnode_id, topnode_id);
            
            update_node(topnode_id);
            update_node(new_topnode_id);
            return new_topnode_id;
        };

        id_t left_rotate(id_t topnode_id) {
            id_t new_topnode_id = get_right_id(topnode_id).value();

            unlink_node(new_topnode_id);
            link_right(topnode_id, get_left_id(new_topnode_id));
            link_left(new_topnode_id, topnode_id);
            
            update_node(topnode_id);
            update_node(new_topnode_id);
            return new_topnode_id;
        };

        virtual id_t insert(const id_opt_t curr_node_id, const int key) {
            if (!curr_node_id) {
                nodes.push_back(
                    make_unique<NodeType>(id, nodes.size(), key)
                );
                return nodes.size() - 1;
            }

            auto node_id = curr_node_id.value();
            auto left_id = get_left_id(node_id);
            auto right_id = get_right_id(node_id);

            if (key > nodes[node_id]->key)
                link_right(node_id, insert(right_id, key));
            else
                link_left(node_id, insert(left_id, key));

            update_node(node_id);
            unlink_node(node_id);
            return node_id;
        };
    };
}