#include <iostream>
#include <cstdio>
#include <vector>

template <typename T>
class SegmentTree {
    struct Node {
        int maxv_i, minv_i;
        T sum, maxv, minv;
        int l, r, left, right;
    };

    std::vector<T> inp;
    std::vector<Node> arr;
    size_t sz;

    void build(int l, int r, int ind) {
        Node v;
        v.l = l, v.r = r;
        if (v.r - v.l <= 1) {
            v.maxv = v.minv = inp[l];
            v.maxv_i = v.minv_i = l;
            arr[ind] = v;
        }
        else {
            v.left = 2 * ind, v.right = 2 * ind + 1;
            build(l, (l + r) / 2, 2 * ind);
            build((l + r) / 2, r, 2 * ind + 1);
            
            // max
            if (arr[2 * ind].maxv > arr[2 * ind + 1].maxv) {
                v.maxv = arr[2 * ind].maxv;
                v.maxv_i = arr[2 * ind].maxv_i;
            }
            else {
                v.maxv = arr[2 * ind + 1].maxv;
                v.maxv_i = arr[2 * ind + 1].maxv_i;
            }
            
            // min
            if (arr[2 * ind].minv < arr[2 * ind + 1].minv) {
                v.minv = arr[2 * ind].minv;
                v.minv_i = arr[2 * ind].minv_i;
            }
            else {
                v.minv = arr[2 * ind + 1].minv;
                v.minv_i = arr[2 * ind + 1].minv_i;
            }
            arr[ind] = v;
        }
    }

    int getmax(int ind, int l, int r) {
        if (arr[ind].r <= l || r <= arr[ind].l)
            return -1;
        if (l <= arr[ind].l && arr[ind].r <= r && arr[ind].l < arr[ind].r)
            return arr[ind].maxv_i;
        int ind1 = getmax(arr[ind].left, l, r);
        int ind2 = getmax(arr[ind].right, l, r);
        if (ind1 == -1)
            return ind2;
        if (ind2 == -1)
            return ind1;
        return (inp[ind1] > inp[ind2] ? ind1 : ind2);
    }
    
    int getmin(int ind, int l, int r) {
        if (arr[ind].r <= l || r <= arr[ind].l)
            return -1;
        if (l <= arr[ind].l && arr[ind].r <= r && arr[ind].l < arr[ind].r)
            return arr[ind].minv_i;
        int ind1 = getmin(arr[ind].left, l, r);
        int ind2 = getmin(arr[ind].right, l, r);
        if (ind1 == -1)
            return ind2;
        if (ind2 == -1)
            return ind1;
        return (inp[ind1] < inp[ind2] ? ind1 : ind2);
    }
    
public:
    SegmentTree() { 
        sz = 0;
        // ??
    }
    template<typename Iter> SegmentTree(Iter from, Iter to) { 
        sz = (to - from) + 1;
        arr.resize(5 * sz + 1);
        inp = std::vector<T>(from, to);
        build(0, sz, 1);
    }
    T getMaxInd(int l = 1, int r = -1) {
        if (r == -1)
            r = sz;
        return getmax(1, l - 1, r);
    }
    T getMinInd(int l = 1, int r = -1) {
        if (r == -1)
            r = sz;
        return getmin(1, l - 1, r);
    }
    T getMaxValue(int l = 1, int r = -1) {
        if (r == -1)
            r = sz;
        return inp[getmax(1, l - 1, r)];
    }
    T getMinValue(int l = 1, int r = -1) {
        if (r == -1)
            r = sz;
        return inp[getmin(1, l - 1, r)];
    }
};

int main() {
    freopen("index-max.in", "r", stdin);
    freopen("index-max.out", "w", stdout);
    int n;
    std::cin >> n;
    std::vector<double> inp(n);
    for (int i = 0; i < n; i++) {
        std::cin >> inp[i];
    }
    SegmentTree<int> t(inp.begin(), inp.end());
    int k;
    std::cin >> k;
    for (int i = 0; i < k; i++) {
        int l, r;
        std::cin >> l >> r;
        std::cout << t.getMaxInd(l, r) + 1 << '\n';
    }
    
    return 0;
}
