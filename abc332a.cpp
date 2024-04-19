#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, s, k;
  cin >> n >> s >> k;
  vector<Pii> pq(n);
  for (auto &[p, q]: pq) cin >> p >> q;

  int base_total = 0;
  for (auto &[p, q]: pq) {
    base_total += p * q;
  }
  
  int total = base_total;
  if (base_total < s) total += k;

  cout << total << endl;

  return 0;
}
