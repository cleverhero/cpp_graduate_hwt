#pragma once

#include <optional>


using std::optional, std::nullopt;


namespace hwt {
    using id_t = size_t;
    using id_opt_t = optional<id_t>;

    namespace metadata {
        /// The basic types of metadata are introduced in this namespace.
        /// Supposed that custom metadata types can be created depending on the application task.

        /// The metadata structure should be very lightweight, so was decided to abandon inheritance.
        /// In order to be able to inform the user that he cannot choose the wrong type of metadata in a particular case 
        /// (for example, he cannot use a metadata type in an AVL tree that does not contain information about the height of the subtree),
        /// was introduced a "traits" system.

        /// So, a trait indicating that the type can be used as metadata for the base BinarySearchTree -- BaseTrait
        /// For Splay Tree -- BaseTrait is also enough
        /// For AVLTree tree -- AVLTrait, descendant of BaseTrait
        /// If a specific OrderStatisticTree structure built on SplayTree is needed, 
        /// then SizeTrait is sufficient, which minimizes the size of the tructure for metadata.

        /// Examples can be found in test/base_tests/templates_test.cpp

        struct BaseTrait {};
        struct AVLTrait: BaseTrait {};
        struct SizeTrait: BaseTrait {};
        struct AVLSizeTrait: AVLTrait, SizeTrait {};

        struct Empty {
            typedef BaseTrait metadata_trait;

            static Empty AfterLeaf() { return Empty{}; }
            void update(const Empty left, const Empty right) noexcept {};
        };

        struct Avl_h {
            typedef AVLTrait metadata_trait;
            int avl_h = 1;

            static Avl_h AfterLeaf() { return Avl_h{0}; }

            void update(const Avl_h& left, const Avl_h& right) noexcept {
                avl_h = (left.avl_h > right.avl_h ? left.avl_h : right.avl_h) + 1;
            };
        };

        struct Size {
            typedef SizeTrait metadata_trait;
            int subtree_size = 1;

            static Size AfterLeaf() { return Size{0}; }

            void update(const Size& left, const Size& right) noexcept {
                subtree_size = left.subtree_size + right.subtree_size + 1;
            };
        };

        struct Full {
            typedef AVLSizeTrait metadata_trait;
            int avl_h = 1;
            int subtree_size = 1;

            static Full AfterLeaf() { return Full{0, 0}; }

            void update(const Full& left, const Full& right) noexcept {
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
}