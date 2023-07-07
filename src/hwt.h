#include <optional>
#include <memory>
#include <vector>


using std::vector;
using std::unique_ptr, std::make_unique;
using std::optional, std::nullopt;


namespace hwt {
    using id_t = size_t;
    using id_opt_t = optional<id_t>;

    int get_node_count();

    struct Node_ {
        id_t id, tree_id;
        id_opt_t parent_id = nullopt;
        id_opt_t left_id = nullopt;
        id_opt_t right_id = nullopt;
        int avl_h = 1, lsubtree_size = 0, rsubtree_size = 0;
        int key;

        Node_(id_t tree_id, id_t id, int key_);
        Node_(id_t tree_id, const Node_& rhs);
        ~Node_() = default;

        static void* operator new(std::size_t n);
        static void operator delete(void* p);
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
        OrderStatisticTree& operator=(const OrderStatisticTree& rhs);

        OrderStatisticTree(OrderStatisticTree&& rhs);
        OrderStatisticTree& operator=(OrderStatisticTree&& rhs);

        void insert(const int key);
        optional<int> select(int k) const;
        int rank(const int k) const;

    private:
        void link_left(id_t parent_id, id_opt_t child_id);
        void link_right(id_t parent_id, id_opt_t child_id);
        void unlink_node(id_t node_id);
        id_opt_t get_left_id(id_t id) const;
        id_opt_t get_parent_id(id_t id) const;
        id_opt_t get_right_id(id_t id) const;
        int get_subtree_avl_h(id_opt_t node_id) const;
        int get_subtree_size(id_opt_t node_id) const;
        int get_subtree_bfactor(id_opt_t node_id) const;
        void recalc_node(id_t node_id);
        id_t right_rotate(id_t topnode_id);
        id_t left_rotate(id_t topnode_id);
        id_t balance_node(id_t node_id);
        id_t insert(const id_opt_t curr_node_id, const int key);
    };
}