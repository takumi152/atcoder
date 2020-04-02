#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> v(n);
  for (auto &x: v) cin >> x;

  unordered_map<int, int> countEven, countOdd;
  for (int i = 0; i < n; i += 2) countEven[v[i]]++;
  for (int i = 1; i < n; i += 2) countOdd[v[i]]++;
  countEven[0] = 0;
  countOdd[0] = 0;

  vector<Pii> countEvenVec, countOddVec;
  for (auto &x: countEven) countEvenVec.emplace_back(n/2-x.second, x.first);
  for (auto &x: countOdd) countOddVec.emplace_back(n/2-x.second, x.first);
  sort(countEvenVec.begin(), countEvenVec.end());
  sort(countOddVec.begin(), countOddVec.end());

  int ans = countEvenVec[0].first + countOddVec[0].first;
  if (countEvenVec[0].second == countOddVec[0].second) ans = min(countEvenVec[1].first + countOddVec[0].first, countEvenVec[0].first + countOddVec[1].first);

  cout << ans << endl;

  return 0;
}
