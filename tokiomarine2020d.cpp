#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>
#include <unordered_map>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

int ilog2(int x) {
  int a = 0;
  while (x > 0) {
    x >>= 1;
    a++;
  }
  return a;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n;
  cin >> n;
  vector<int> v(262144), w(262144);
  for (int i = 0; i < n; i++) cin >> v[i] >> w[i];
  int q;
  cin >> q;
  vector<Pii> vl(n);
  for (int i = 0; i < n; i++) {
    cin >> vl[i].first >> vl[i].second;
    vl[i].first--;
  }

  vector<vector<Pii> > roottree(4096);
  queue<unordered_map<int, int> > que;
  unordered_map<int, int> first;
  first[0] = 0;
  que.push(first);
  for (int i = 0; i < 4096; i++) {
    auto now = que.front();
    que.pop();
    auto next = now;
    for (auto &x: now) {
      if (next[x.first+w[i]] < x.second+v[i]) next[x.first+w[i]] = x.second+v[i];
    }
    for (auto &x: now) {
      roottree[i].emplace_back(x.first, x.second);
    }
    sort(roottree[i].begin(), roottree[i].end());
    if (i < 2048) {
      que.push(next);
      que.push(next);
    }
  }

  vector<vector<vector<Pii> > > lowertree(4096, vector<vector<Pii> >(32));
  for (int j = 4096; j < 8192; j++) {
    queue<unordered_map<int, int> > que;
    unordered_map<int, int> first;
    first[0] = 0;
    que.push(first);
    for (int i = 0; i < 32; i++) {
      auto now = que.front();
      que.pop();
      auto next = now;
      for (auto &x: now) {
        if (next[x.first+w[(j<<ilog2(i))+i]] < x.second+v[(j<<ilog2(i))+i]) next[x.first+w[(j<<ilog2(i))+i]] = x.second+v[(j<<ilog2(i))+i];
      }
      for (auto &x: now) {
        lowertree[j-4096][i].emplace_back(x.first, x.second);
      }
      sort(lowertree[j-4096][i].begin(), lowertree[j-4096][i].end());
      if (i < 16) {
        que.push(next);
        que.push(next);
      }
    }
  }

  vector<int> ans(q);
  for (int i = 0; i < q; i++) {
    if (vl[i].first < 4096) {
      for (auto &x: roottree[vl[i].first]) cerr << x.first << " " << x.second << ", ";
      cerr << endl;
      ans[q] = (*(lower_bound(roottree[vl[i].first].begin(), roottree[vl[i].first].end(), Pii(vl[i].second, -1)))).second;
    }
    else {
      for (auto &x: lowertree[(vl[i].first>>(ilog2(vl[i].first)-13))-4096][vl[i].first&~(8191<<(ilog2(vl[i].first)-13))]) {
        if (x.first > vl[i].second) continue;
        ans[q] = max(ans[q], x.second + (*(lower_bound(roottree[(vl[i].first>>(ilog2(vl[i].first)-12))].begin(), roottree[(vl[i].first>>(ilog2(vl[i].first)-12))].end(), Pii(vl[i].second - x.first, -1)))).second);
      }
    }
  }

  for (auto &x: ans) {
    cout << x << endl;
  }

  return 0;
}
