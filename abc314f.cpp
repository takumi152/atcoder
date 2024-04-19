#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <memory>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

struct Node {
  int id;
  int size;
  int parent_p, parent_q;
  int lowest_child;
};

ll modinv(ll x, ll m = mod) {
  ll b = m;
  ll u = 1;
  ll v = 0;
  ll tmp;
  while (b) {
    ll t = x / b;
    x -= t * b;
    tmp = x;
    x = b;
    b = tmp;
    u -= t * v;
    tmp = u;
    u = v;
    v = tmp;
  }
  u %= m;
  if (u < 0) u += m;
  return u;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<Pii> pq(n - 1);
  for (auto &[p, q]: pq) cin >> p >> q;

  vector<Node> nodes;
  for (int i = 0; i < n; i++) {
    Node node;
    node.id = i;
    node.size = 1;
    node.parent_p = -1;
    node.parent_q = -1;
    node.lowest_child = i;
    nodes.push_back(node);
  }
  for (int i = 0; i < n - 1; i++) {
    auto &[p, q] = pq[i];

    int id_p = p - 1;
    while (nodes[id_p].id != nodes[id_p].lowest_child) {
      int next = nodes[nodes[id_p].lowest_child].lowest_child;
      nodes[id_p].lowest_child = next;
      id_p = next;
    }
    int id_q = q - 1;
    while (nodes[id_q].id != nodes[id_q].lowest_child) {
      int next = nodes[nodes[id_q].lowest_child].lowest_child;
      nodes[id_q].lowest_child = next;
      id_q = next;
    }

    Node &node_p = nodes[id_p];
    Node &node_q = nodes[id_q];
    node_p.lowest_child = n + i;
    node_q.lowest_child = n + i;

    Node next_node;
    next_node.id = n + i;
    next_node.size = node_p.size + node_q.size;
    next_node.parent_p = id_p;
    next_node.parent_q = id_q;
    next_node.lowest_child = n + i;
    nodes.push_back(next_node);
  }

  auto euler_tour = [&](auto self, vector<pair<ll, ll>> &ans, pair<ll, ll> now_expected, int now_id) {
    if (false) return;

    auto &now_node = nodes[now_id];
    if (now_node.parent_p == -1 && now_node.parent_q == -1) {
      ans[now_id] = now_expected;
    }
    else {
      auto &node_p = nodes[now_node.parent_p];
      auto &node_q = nodes[now_node.parent_q];
      int size_total = node_p.size + node_q.size;
      // a / b + c / d == (ad + cb) / bd
      pair<ll, ll> expected_p = make_pair((((now_expected.first * size_total) % mod) + ((node_p.size * now_expected.second) % mod)) % mod, (now_expected.second * size_total) % mod);
      pair<ll, ll> expected_q = make_pair((((now_expected.first * size_total) % mod) + ((node_q.size * now_expected.second) % mod)) % mod, (now_expected.second * size_total) % mod);
      self(self, ans, expected_p, node_p.id);
      self(self, ans, expected_q, node_q.id);
    }
  };

  vector<pair<ll, ll>> ans(n);
  pair<ll, ll> now_expected = make_pair(0, 1);
  int now_id = nodes.size() - 1;
  euler_tour(euler_tour, ans, now_expected, now_id);

  for (auto &[y, x]: ans) cout << (y * modinv(x)) % mod << " ";
  cout << endl;

  return 0;
}
