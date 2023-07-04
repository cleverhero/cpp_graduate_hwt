#include <optional>


using std::optional, std::nullopt;


namespace hwt {
	static int node_count = 0;

	struct Node_ {
		Node_ *parent, *left, *right;
		
		int avl_h, lsubtree_size, rsubtree_size;
		int key;

		Node_(int key_):
			parent(nullptr),
			left(nullptr),
			right(nullptr),
			avl_h(1),
			lsubtree_size(0),
			rsubtree_size(0),
			key(key_)
		{}

		static Node_* shallow_clone(const Node_& rhs) {
			node_count++;
			Node_* node = new Node_{rhs.key};
			node->avl_h = rhs.avl_h;
			node->lsubtree_size = rhs.lsubtree_size;
			node->rsubtree_size = rhs.rsubtree_size;

			return node;
		}

		static Node_* deep_clone(const Node_* rhs) {
			const Node_* rhs_node = rhs;
			if (!rhs_node)
				return nullptr;

			Node_* this_node = Node_::shallow_clone(*rhs_node);
			Node_* result = this_node;
			while (rhs_node) {
				if (rhs_node->left && !this_node->left) {
					this_node->link_left( Node_::shallow_clone(*rhs_node->left) );
					this_node = this_node->left;
					rhs_node = rhs_node->left;
				}
				else if (rhs_node->right && !this_node->right) {
					this_node->link_right( Node_::shallow_clone(*rhs_node->right) );
					this_node = this_node->right;
					rhs_node = rhs_node->right;
				}
				else {
					this_node = this_node->parent;
					rhs_node = rhs_node->parent;
				}
			}

			return result;
		}

		static void deep_delete(Node_* rhs) {
			Node_* rhs_node = rhs;
			if (!rhs_node)
				return;

			while (rhs_node) {
				if (rhs_node->left) {
					rhs_node = rhs_node->left;
				}
				else if (rhs_node->right) {
					rhs_node = rhs_node->right;
				}
				else {
					Node_* parent = rhs_node->parent;

					node_count--;
					delete rhs_node;
					rhs_node = parent;

					if (parent && parent->left)
						parent->left = nullptr;
					else if (parent && parent->right)
						parent->right = nullptr;
				}
			}
		}

		void link_left(Node_* rhs) {
			left = rhs;
			if (rhs) rhs->parent = this;
		}

		void link_right(Node_* rhs) {
			right = rhs;
			if (rhs) rhs->parent = this;
		}

		void unlink() {
			this->parent = nullptr;
		}

		int bfactor() const {
			auto r_avl_h = right ? right->avl_h : 0;
			auto l_avl_h = left ? left->avl_h : 0;

			return r_avl_h - l_avl_h;
		}

		void recalc() {
			auto r_avl_h = right ? right->avl_h : 0;
			auto l_avl_h = left ? left->avl_h : 0;
			avl_h = (l_avl_h > r_avl_h ? l_avl_h : r_avl_h) + 1;

			lsubtree_size = 0;
			if (left)
				lsubtree_size = left->lsubtree_size + left->rsubtree_size + 1;

			rsubtree_size = 0;
			if (right)
				rsubtree_size = right->lsubtree_size + right->rsubtree_size + 1;
		}
	};

	Node_* right_rotate(Node_* root) {
		Node_* new_root = root->left;
		new_root->unlink();
		root->link_left(new_root->right);
		new_root->link_right(root);

		root->recalc();
		new_root->recalc();
		return new_root;
	}

	Node_* left_rotate(Node_* root) {
		Node_* new_root = root->right;
		new_root->unlink();
		root->link_right(new_root->left);
		new_root->link_left(root);
		
		root->recalc();
		new_root->recalc();
		return new_root;
	}

	Node_* balance(Node_* root) {
		if (root->bfactor() == 2) {
			if (root->right->bfactor() < 0)
				root->link_right( right_rotate(root->right) );
			return left_rotate(root);
		}
		if (root->bfactor() == -2) {
			if (root->left->bfactor() > 0)
				root->link_left( left_rotate(root->left) );
			return right_rotate(root);
		}

		return root;
	}

	Node_* insert(Node_* curr_node, const int key) {
		if (!curr_node) {
			node_count++;
			return new Node_{ key };
		}

		if (key > curr_node->key)
			curr_node->link_right(insert(curr_node->right, key));
		else
			curr_node->link_left(insert(curr_node->left, key));

		curr_node->recalc();
		curr_node->unlink();
		return balance(curr_node);
	}

	class OrderStatisticTree {
		Node_* root_;

	public:
		OrderStatisticTree(): root_() {}

		~OrderStatisticTree() {
			Node_::deep_delete(this->root_);
		}

		OrderStatisticTree(const OrderStatisticTree& rhs):
			root_(Node_::deep_clone(rhs.root_)) 
		{}

		OrderStatisticTree& operator=(const OrderStatisticTree& rhs) {
			Node_::deep_delete(this->root_);
			this->root_ = Node_::deep_clone(rhs.root_);
			return *this;
		}

		void insert(const int key) {
			root_ = hwt::insert(root_, key);
		}

		optional<int> select(int k) const {
			if (k < 1)
				return nullopt;

			auto curr_node = root_;
			while (curr_node) {
				int extra_k = k - curr_node->lsubtree_size;

				if (extra_k > 1) {
					curr_node = curr_node->right;
					k = extra_k - 1;
				}
				else if (extra_k < 1)
					curr_node = curr_node->left;
				else
					return curr_node->key;
			}

			return nullopt;
		}

		int rank(const int k) const {
			if (!root_)
				return 0;

			auto curr_node = root_;
			int result = 0;
			while (curr_node) {
				if (k > curr_node->key) {
					result += curr_node->lsubtree_size + 1;
					curr_node = curr_node->right; 
				}
				else if (k < curr_node->key) {
					curr_node = curr_node->left;
				}
				else {
					result += curr_node->lsubtree_size;
					break;
				}
			}

			return result;
		}
	};
}