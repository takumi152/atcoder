#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int t;
  cin >> t;
  vector<ll> d(t), k(t), x(t);
  for (int i = 0; i < t; i++) cin >> d[i] >> k[i] >> x[i];

  vector<ll> ans(t);
  for (int i = 0; i < t; i++) {
    vector<ll> layer_node(d[i]+1);
    layer_node[0] = 1;
    for (int j = 1; j <= d[i]; j++) layer_node[j] = layer_node[j-1] * k[i];

    vector<ll> layer_sum(d[i]+1);
    layer_sum[0] = 1;
    for (int j = 1; j <= d[i]; j++) layer_sum[j] = layer_sum[j-1] + layer_node[j];

    ans[i] = 0;
    ll current_layer = d[i];
    if (x[i] <= layer_sum[current_layer-1]) {
      ans[i] += 1;
      while (current_layer > 0 && x[i] <= layer_sum[current_layer-1]) current_layer--;
    }
    ll current_node = layer_sum[current_layer];
    while (current_layer > 0) {
      ll edge_to_cut = (current_node - x[i]) / layer_sum[current_layer-1];
      ans[i] += edge_to_cut;
      current_node -= layer_sum[current_layer-1] * edge_to_cut;
      current_layer--;
    }
  }

  for (auto &x: ans) cout << x << endl;

  return 0;
}
