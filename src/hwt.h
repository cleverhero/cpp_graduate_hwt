#include <optional>
#include <memory>
#include <vector>


using std::vector;
using std::unique_ptr, std::make_unique;
using std::optional, std::nullopt;


namespace hwt {
    using id_t = size_t;
    using id_opt_t = optional<id_t>;

    struct Node_ {
        id_t id, tree_id;
        id_opt_t parent_id = nullopt;
        id_opt_t left_id = nullopt;
        id_opt_t right_id = nullopt;
        int avl_h = 1, lsubtree_size = 0, rsubtree_size = 0;
        int key;

        Node_(id_t tree_id, id_t id, int key_):
            id(id),
            tree_id(tree_id),

            key(key_)
        {}

        Node_(id_t new_tree_id, const Node_& rhs):
            Node_(rhs)
        {
            tree_id = new_tree_id;
        }

        ~Node_() = default;

    #ifdef TEST_MEMORY
        static void* operator new(std::size_t n);
        static void operator delete(void* p);
    #endif
    };

    using unique_ptr_node = unique_ptr<Node_>;

    class OrderStatisticTree {
    public:
        id_t id;
        id_opt_t root_id;
        vector<unique_ptr_node> nodes;

        OrderStatisticTree();
        ~OrderStatisticTree() = default;

        OrderStatisticTree(const OrderStatisticTree& rhs);

        OrderStatisticTree& operator=(const OrderStatisticTree& rhs) {
            OrderStatisticTree tmp(rhs);

            std::swap(*this, tmp);
            return *this;
        }

        OrderStatisticTree(OrderStatisticTree&& rhs) noexcept:
            id(rhs.id),
            root_id(rhs.root_id)
        {
            nodes.swap(rhs.nodes);
        }

        OrderStatisticTree& operator=(OrderStatisticTree&& rhs) noexcept {
            this->nodes.swap(rhs.nodes);

            id = rhs.id;
            root_id = rhs.root_id;
            return *this;
        }

        void insert(const int key) {
            root_id = insert(root_id, key); 
        }
        optional<int> select(int k) const;
        int rank(const int k) const;

    private:
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

        int get_subtree_avl_h(id_opt_t node_id) const {
            if (!node_id)
                return 0;

            return nodes[node_id.value()]->avl_h;
        }

        int get_subtree_size(id_opt_t node_id) const {
            if (!node_id)
                return 0;

            return nodes[node_id.value()]->lsubtree_size + nodes[node_id.value()]->rsubtree_size + 1;
        }

        int get_subtree_bfactor(id_opt_t node_id) const {
            if (!node_id)
                return 0;

            auto right_id = get_right_id(node_id.value());
            auto left_id = get_left_id(node_id.value());
            return get_subtree_avl_h(right_id) - get_subtree_avl_h(left_id);
        }

        void recalc_node(id_t node_id);
        id_t right_rotate(id_t topnode_id);
        id_t left_rotate(id_t topnode_id);
        id_t balance_node(id_t node_id);
        id_t insert(const id_opt_t curr_node_id, const int key);
    };
}