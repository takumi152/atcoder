// WIP

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  for (auto &x: a) x = (x - 1) % k;

  // 長さk-1以下の連続部分列で、総和(mod k)が0になるものの数は？


  for (auto &x: a) cout << x << " ";
  cout << endl;

  return 0;
}
