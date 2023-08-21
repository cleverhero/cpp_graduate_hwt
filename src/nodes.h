#pragma once

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
        /// В данном неймспейсе введены базовые типы метаданных.
        /// Предполагается, что можно создавать дополнительные типы метаданных в зависимости от прикладной задачи.
        ///
        /// Так как структура метаданных должна быть очень легковесной, то было решено отказаться от наследования,
        /// а вместо этого, чтобы была возможность сообщать пользователю, что он не может в конкретном случае выбрать неправильный тип метаданных
        /// (например не может в AVL дереве использовать тип метаданных, который не содержит информацию о высоте поддерева)
        /// была введена система "трейтов".
        ///
        /// Так, трейт указывающий, что тип может использоваться как метадата для базового BinarySearchTree -- BaseTrait
        /// Для SplayTree -- так же достаточно BaseTrait
        /// Для AVLTree дерева -- AVLTrait, потомок BaseTrait
        /// Если необходима специфичная структура OrderStatisticTree на SplayTree, то достаточно SizeTrait,
        /// что позволяет минимизировать размер структуры для метаданных.
        ///
        /// Примеры можно найти в test/base_tests/templates_test.cpp

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