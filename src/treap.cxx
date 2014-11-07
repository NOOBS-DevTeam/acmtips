// Vladimir Miloserdov (c) 2014
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

inline int gcd(int u, int v) {
    while (v != 0) {
        int r = u % v;
        u = v;
        v = r;
    }
    return u;
}

class Treap {
	struct Node {
		int x, y;
		int gcd_val, cnt;
		Node *l, *r;
		
		Node(const int x, const int y) {
			this->l = this->r = 0;
			this->cnt = 0;
			this->x = x;
			this->y = y;
			this->gcd_val = x;
		}
		
		void recalc() {
			this->gcd_val = gcd(gcd(x, (this->l != NULL ? this->l->gcd_val : 0)), 
				(this->r != NULL ? this->r->gcd_val : 0));
		}
	};
	Node *root;
	
	Node* merge(Node* l, Node* r) {
		if (l == NULL) {
			return r;
		}
		else if (r == NULL) {
			return l;
		}
		else if (l->y > r->y) {
			l->r = merge(l->r, r);
			l->recalc();
			return l;
		} 
		else {
			r->l = merge(l, r->l);
			r->recalc();
			return r;
		}
	}

	pair<Node*, Node*> split(Node* T, int x0) {
		if (T == NULL) {
			return make_pair((Node*)0, (Node*)0);
		}
		pair<Node*, Node*> res;
		if (T->x >= x0) {
			res = split(T->l, x0);
			T->l = res.second;
			T->recalc();
			res.second = T;
			return res;
		}
		else {
			res = split(T->r, x0);
			T->r = res.first;
			T->recalc();
			res.first = T;
			return res;
		}
	}
	
	Node* insert(Node* T, Node* t0) {
		if (!T) {
			return t0;
		} 
		else if (t0->y > T->y) {
			pair<Node*, Node*> ret = split(T, t0->x);
			t0->l = ret.first;
			t0->r = ret.second;
			t0->recalc();
			return t0;
		} 
		else if (t0->x < T->x) {
			T->l = insert(T->l, t0);
			T->recalc();
			return T;
		} 
		else {
			T->r = insert(T->r, t0);
			T->recalc();
			return T;
		}
	}
	
	Node* erase(Node* T, int x0) {
		if (T->x == x0) {
			if (T->cnt == 0) {
				return merge(T->l, T->r);
			}
			else {
				T->cnt--;
				return T;
			}
		} 
		else if (x0 < T->x) {
			T->l = erase(T->l, x0);
			T->recalc();
			return T;
		}
		else {
			T->r = erase(T->r, x0);
			T->recalc();
			return T;
		}
	}
	
	bool check(const Node* T, const int x0) {
        if (!T)
            return 0;
        else if (T->x == x0)
            return 1;
        else if (x0 < T->x)
			return check(T->l, x0);
		else
            return check(T->r, x0);
	}
	
	Node* find(Node* t, int x0) {
		Node* cur = t;
		while (cur && cur->x != x0) {
			if (cur->x > x0) {
				cur = cur->l;
			} 
			else if (cur->x < x0){
				cur = cur->r;
			}
		}
		return cur;
	}

public:
	Treap() : root((Node*)0) { }
	
	void insert(const int x0) {
		Node *cur = find(root, x0);
		if (!cur) {
			Node* t0 = new Node(x0, rand());
			root = insert(root, t0);
		}
		else {
			cur->cnt++;
		}
	}
	
	void erase(const int x0) {
		root = erase(root, x0);
	}
	
	bool check(const int x0) {
        return check(root, x0);
    }
    
    int get_gcd() {
		return (root ? root->gcd_val : 1);
	}
};

int main() {
	srand(time(0));
	Treap t;
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		char c;
		int tmp;
		scanf(" %c %d", &c, &tmp);
		if (c == '+')
			t.insert(tmp);
		else
			t.erase(tmp);
		printf("%d\n", t.get_gcd());
	}
	
	return 0;
}
