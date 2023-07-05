#include <optional>
#include <memory>
#include <vector>


using std::vector;
using std::unique_ptr, std::make_unique;
using std::optional, std::nullopt;


namespace hwt {
	// TODO
	static int node_count = 0;
	static int id_counter = 0;

	struct Node_ {
		size_t id, tree_id;
		optional<size_t> parent_id = nullopt;
		optional<size_t> left_id = nullopt;
		optional<size_t> right_id = nullopt;
		
		int avl_h = 1, lsubtree_size = 0, rsubtree_size = 0;
		int key;

		Node_(size_t tree_id, size_t id, int key_):
			id(id),
			tree_id(tree_id),

			key(key_)
		{}

		Node_(size_t tree_id, const Node_& rhs):
			Node_(rhs)
		{
			tree_id = tree_id;
		}
	};

	using unique_ptr_node = unique_ptr<Node_>;
	using id_t = size_t;
	using id_opt_t = optional<id_t>;

	class OrderStatisticTree {
		id_t id;
		id_opt_t root_id;
		vector<unique_ptr_node> nodes;

	public:
		OrderStatisticTree():
			id(id_counter++),
			root_id(nullopt),
			nodes()			
		{}

		~OrderStatisticTree() {
			node_count -= nodes.size();
		};

		OrderStatisticTree(const OrderStatisticTree& rhs):
			id(id_counter++),
			root_id(rhs.root_id),
			nodes(rhs.nodes.size())
		{
			for (int i = 0; i < rhs.nodes.size(); i++) {
				nodes[i] = make_unique<Node_>(id, *rhs.nodes[i]);
				node_count++;
			}
		}

		OrderStatisticTree& operator=(const OrderStatisticTree& rhs) {
			// TODO
			node_count -= nodes.size();
			nodes = vector<unique_ptr_node>(rhs.nodes.size());

			root_id = rhs.root_id;
			for (int i = 0; i < rhs.nodes.size(); i++) {
				nodes[i] = make_unique<Node_>(id, *rhs.nodes[i]);
				node_count++;
			}
			return *this;
		}

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

		id_opt_t get_left_id(id_t id) const {
			return nodes[id]->left_id;
		}

		id_opt_t get_parent_id(id_t id) const {
			return nodes[id]->parent_id;
		}

		id_opt_t get_right_id(id_t id) const {
			return nodes[id]->right_id;
		}

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

		void recalc_node(id_t node_id) {
			auto left_id = get_left_id(node_id);
			auto right_id = get_right_id(node_id);

			auto l_avl_h = get_subtree_avl_h(left_id);
			auto r_avl_h = get_subtree_avl_h(right_id);
			
			nodes[node_id]->avl_h = (l_avl_h > r_avl_h ? l_avl_h : r_avl_h) + 1;
			nodes[node_id]->lsubtree_size = get_subtree_size(left_id);
			nodes[node_id]->rsubtree_size = get_subtree_size(right_id);
		}

		id_t right_rotate(id_t topnode_id) {
			id_t new_topnode_id = get_left_id(topnode_id).value();

			unlink_node(new_topnode_id);
			link_left(topnode_id, get_right_id(new_topnode_id));
			link_right(new_topnode_id, topnode_id);
			
			recalc_node(topnode_id);
			recalc_node(new_topnode_id);
			return new_topnode_id;
		}

		id_t left_rotate(id_t topnode_id) {
			id_t new_topnode_id = get_right_id(topnode_id).value();

			unlink_node(new_topnode_id);
			link_right(topnode_id, get_left_id(new_topnode_id));
			link_left(new_topnode_id, topnode_id);
			
			recalc_node(topnode_id);
			recalc_node(new_topnode_id);
			return new_topnode_id;
		}

		id_t balance_node(id_t node_id) {
			auto left_id = get_left_id(node_id);
			auto right_id = get_right_id(node_id);
			auto bfactor = get_subtree_bfactor(node_id);

			if (bfactor == 2) {
				if (get_subtree_bfactor(right_id) < 0)
					link_right(node_id, right_rotate(right_id.value()) );
				return left_rotate(node_id);
			}
			if (bfactor == -2) {
				if (get_subtree_bfactor(left_id) > 0)
					link_left(node_id, left_rotate(left_id.value()) );
				return right_rotate(node_id);
			}
			
			return node_id;
		}

		id_t insert(const id_opt_t curr_node_id, const int key) {
			if (!curr_node_id) {
				nodes.push_back(
					make_unique<Node_>(id, nodes.size(), key)
				);
				node_count++;
				return nodes.size() - 1;
			}

			auto node_id = curr_node_id.value();
			auto left_id = get_left_id(node_id);
			auto right_id = get_right_id(node_id);

			if (key > nodes[node_id]->key)
				link_right(node_id, insert(right_id, key));
			else
				link_left(node_id, insert(left_id, key));

			recalc_node(node_id);
			unlink_node(node_id);
			return balance_node(node_id);
		}

		void insert(const int key) {
			// NON-RECURSIVE?
			root_id = insert(root_id, key);
		}

		optional<int> select(int k) const {
			if (k < 1 || k > nodes.size())
				return nullopt;

			auto curr_node_id = root_id;
			while (curr_node_id) {
				auto node_id = curr_node_id.value();
				int extra_k = k - nodes[node_id]->lsubtree_size;

				if (extra_k > 1) {
					curr_node_id = get_right_id(node_id);
					k = extra_k - 1;
				}
				else if (extra_k < 1)
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
				if (k > nodes[node_id]->key) {
					result += nodes[node_id]->lsubtree_size + 1;
					curr_node_id = get_right_id(node_id);
				}
				else if (k < nodes[node_id]->key) {
					curr_node_id = get_left_id(node_id);
				}
				else {
					result += nodes[node_id]->lsubtree_size;
					break;
				}
			}

			return result;
		}
	};
}