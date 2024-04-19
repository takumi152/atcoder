#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

#include <atcoder/lazysegtree>
#include <atcoder/modint>

using namespace std;
using namespace atcoder;

typedef long long int ll;
typedef pair<int, int> Pii;
using mint = modint998244353;

struct S {
  mint value; // 現在の期待値
};

struct F {
  mint value; // 値が変化した場合の期待値
  mint chance; // 値が変化する確率
};

S op (S l, S r) {
  return S {
    l.value
  };
}

S e() {
  return S {
    mint(0)
  };
}

S mapping(F f, S x) {
  return S {
    f.value * f.chance + x.value * (mint(1) - f.chance)
  };
}

F composition(F f, F g) {
  mint remaining_chance = mint(1) - f.chance;
  mint orig_to_value_chance = remaining_chance * g.chance;
  mint overall_chance = f.chance + orig_to_value_chance;
  if (overall_chance.val() == 0) {
    return F {
      mint(0),
      mint(0)
    };
  }

  mint next_value = (f.value * ((f.chance - f.chance * g.chance) / overall_chance)) + (g.value * ((f.chance * g.chance + orig_to_value_chance) / overall_chance));

  // cerr << f.value.val() << " " << f.chance.val() << " " << g.value.val() << " " << g.chance.val() << " " << next_value.val() << " " << (f.chance + orig_to_value_chance).val() << endl;

  return F {
    next_value,
    overall_chance
  };
}

F id() {
  return F {
    mint(0),
    mint(0)
  };
}

struct Query {
  int l, r, x;
};

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, q;
  cin >> n >> q;
  vector<S> a(n);
  for (int i = 0; i < n; i++) {
    int v;
    cin >> v;
    a[i] = S {
      mint(v)
    };
  }
  vector<Query> queries(q);
  for (auto &x: queries) cin >> x.l >> x.r >> x.x;

  lazy_segtree<S, op, e, F, mapping, composition, id> lst(a);
  for (auto &query: queries) {
    lst.apply(query.l - 1, query.r, F {
      mint(query.x),
      mint(1) / mint(query.r - query.l + 1)
    });
  }

  for (int i = 0; i < n; i++) {
    auto res = lst.get(i);
    cout << res.value.val() << " ";
  }
  cout << endl;

  return 0;
}
