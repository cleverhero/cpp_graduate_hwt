#include <optional>
#include <memory>
#include <vector>


using std::vector;
using std::unique_ptr, std::make_unique;
using std::optional, std::nullopt;



namespace hwt {
    using id_t = size_t;
    using id_opt_t = optional<id_t>;

    namespace metadata {
        struct Empty {
            typedef std::integral_constant<int, 1> base_trait;

            static Empty AfterLeaf() { return Empty{}; }
            void update(const Empty left, const Empty right) {};
        };

        struct Avl_h {
            typedef std::integral_constant<int, 1> base_trait;

            typedef std::integral_constant<int, 2> avl_trait;
            int avl_h = 1;

            static Avl_h AfterLeaf() { return Avl_h{0}; }

            void update(const Avl_h& left, const Avl_h& right) {
                avl_h = (left.avl_h > right.avl_h ? left.avl_h : right.avl_h) + 1;
            };
        };

        struct Size {
            typedef std::integral_constant<int, 1> base_trait;

            typedef std::integral_constant<int, 2> size_trait;
            int subtree_size = 1;

            static Size AfterLeaf() { return Size{0}; }

            void update(const Size& left, const Size& right) {
                subtree_size = left.subtree_size + right.subtree_size + 1;
            };
        };

        struct Full {
            typedef std::integral_constant<int, 1> base_trait;

            typedef std::integral_constant<int, 2> avl_trait;
            int avl_h = 1;

            typedef std::integral_constant<int, 3> size_trait;
            int subtree_size = 1;

            static Full AfterLeaf() { return Full{0, 0}; }

            void update(const Full& left, const Full& right) {
                avl_h = (left.avl_h > right.avl_h ? left.avl_h : right.avl_h) + 1;
                subtree_size = left.subtree_size + right.subtree_size + 1;
            };
        };
    }


    template<typename MetadataType = metadata::Empty>
    struct Node_ {
        using MT = MetadataType;
        using NodeType = Node_<MT>;

        id_t id, tree_id;
        id_opt_t parent_id = nullopt;
        id_opt_t left_id = nullopt;
        id_opt_t right_id = nullopt;

        MT metadata;
        int key;

        Node_(id_t tree_id, id_t id, int key_):
            id(id),
            tree_id(tree_id),

            key(key_)
        {}

        Node_(id_t new_tree_id, const NodeType& rhs):
            NodeType(rhs)
        {
            tree_id = new_tree_id;
        }

        ~Node_() = default;

    #ifdef TEST_MEMORY
        static void* operator new(std::size_t n);
        static void operator delete(void* p);
    #endif
    };


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


    template<
        template<typename MT>
        typename TreeType = AVLTree,
        typename MT = metadata::Full,
        typename = typename MT::size_trait
    >
    class OrderStatisticTree: public TreeType<MT> {
    public:
        using TreeType<MT>::insert;

        optional<int> select(int k) const {
            if (k < 1 || k > this->nodes.size())
                return nullopt;

            auto curr_node_id = this->root_id;
            while (curr_node_id) {
                auto node_id = curr_node_id.value();
                auto right_id = this->get_right_id(node_id);

                auto subtree_size = this->get_metadata(node_id).subtree_size;
                auto rsubtree_size = this->get_metadata(right_id).subtree_size;
                int extra_k = k - subtree_size + rsubtree_size;

                if (extra_k > 0) {
                    curr_node_id = right_id;
                    k = extra_k;
                }
                else if (extra_k < 0)
                    curr_node_id = this->get_left_id(node_id);
                else
                    return this->nodes[node_id]->key;
            }

            return nullopt;
        }

        int rank(const int k) const {
            if (!this->root_id)
                return 0;

            auto curr_node_id = this->root_id;
            int result = 0;
            while (curr_node_id) {
                auto node_id = curr_node_id.value();
                auto right_id = this->get_right_id(node_id);

                auto subtree_size = this->get_metadata(node_id).subtree_size;
                auto rsubtree_size = this->get_metadata(right_id).subtree_size;

                if (k > this->nodes[node_id]->key) {
                    result += subtree_size - rsubtree_size;
                    curr_node_id = right_id;
                }
                else if (k < this->nodes[node_id]->key) {
                    curr_node_id = this->get_left_id(node_id);
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
