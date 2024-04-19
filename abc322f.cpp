#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

#include <atcoder/lazysegtree>

using namespace std;
using namespace atcoder;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

struct S {
  int size;
  int left_num, right_num;
  bool is_left_one, is_right_one;
  int zero_max, one_max;
};

// flip flag
using F = bool;

S op (S l, S r) {
  int next_left_num = l.left_num;
  if (l.size == l.left_num && l.is_left_one == r.is_left_one) {
    next_left_num = l.left_num + r.left_num;
  }
  int next_right_num = r.right_num;
  if (r.size == r.right_num && l.is_right_one == r.is_right_one) {
    next_right_num = l.right_num + r.right_num;
  }

  int next_zero_max = max(l.zero_max, r.zero_max);
  int next_one_max = max(l.one_max, r.one_max);
  if (l.is_right_one == r.is_left_one) {
    if (l.is_right_one) next_one_max = max(next_one_max, l.right_num + r.left_num);
    else next_zero_max = max(next_zero_max, l.right_num + r.left_num);
  }

  return S {
    l.size + r.size,
    next_left_num, next_right_num,
    l.is_left_one, r.is_right_one,
    next_zero_max, next_one_max
  };
}

S e() {
  return S {
    0,
    0, 0,
    false, false,
    0, 0
  };
}

S mapping(F l, S r) {
  if (!l) return r;
  // flip
  return S {
    r.size,
    r.left_num, r.right_num,
    !r.is_left_one, !r.is_right_one,
    r.one_max, r.zero_max
  };
}

F composition(F l, F r) {
  return (l && !r) || (!l && r);
}

F id() {
  return false;
}

struct Query {
  int type;
  int l, r;
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<S> s(n);
  for (int i = 0; i < n; i++) {
    char c;
    cin >> c;
    if (c == '0') {
      s[i] = S {
        1,
        1, 1,
        false, false,
        1, 0
      };
    }
    else {
      s[i] = S {
        1,
        1, 1,
        true, true,
        0, 1
      };
    }
  }
  vector<Query> queries(q);
  for (auto &x: queries) cin >> x.type >> x.l >> x.r;

  lazy_segtree<S, op, e, F, mapping, composition, id> lst(s);
  vector<int> ans;
  for (auto &query: queries) {
    if (query.type == 1) {
      lst.apply(query.l - 1, query.r, true);
    }
    else {
      auto res = lst.prod(query.l - 1, query.r);
      ans.push_back(res.one_max);
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
