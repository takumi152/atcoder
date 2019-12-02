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

  int n, k, q;
  cin >> n >> k >> q;
  vector<int> a(n);
  for (auto &x: a) cin >> x;

  vector<int> as = a;
  sort(as.begin(), as.end());

  int minDiff = as[q-1] - as[0];
  int p = 0;
  while (p < n) {
    int numToBan = as[p];
    while (p < n) {
      if (as[p] == numToBan) p++;
      else break;
    }
    for (int i = 0; i < n; i++) {
      if (a[i] == numToBan) a[i] = -1; // ban minimum number
    }

    vector<vector<int> > seq;
    int seqCount = 0;
    int seqLength = 0;
    int start = 0;
    int pickable = 0;
    vector<int> seqPickable;
    for (int i = 0; i < n; i++) {
      if (a[i] != -1) seqLength++;
      else {
        if (seqLength >= k) {
          seq.push_back(vector<int>());
          for (int j = start; j < i; j++) seq[seqCount].push_back(a[j]);
          pickable += seqLength - k + 1;
          seqPickable.push_back(seqLength - k + 1);
          seqCount++;
        }
        seqLength = 0;
        start = i + 1;
      }
    }
    if (seqLength >= k) {
      seq.push_back(vector<int>());
      for (int j = start; j < n; j++) seq[seqCount].push_back(a[j]);
      pickable += seqLength - k + 1;
      seqPickable.push_back(seqLength - k + 1);
      seqCount++;
    }

    if (pickable < q) break; // cannot pick number without picking banned numbers

    for (int i = 0; i < seqCount; i++) sort(seq[i].begin(), seq[i].end());

    int picked = 0;
    int firstPicked = -1;
    int lastPicked = -1;
    priority_queue<Pii, vector<Pii>, greater<Pii> > que;
    vector<int> seqPicked;
    for (int i = 0; i < seqCount; i++) {
      que.emplace(seq[i][0], i);
      seqPicked.push_back(0);
    }

    while (picked < q) {
      auto now = que.top();
      que.pop();
      int num = now.first;
      int idx = now.second;
      if (firstPicked == -1) firstPicked = num;
      lastPicked = num;
      seqPicked[idx]++;
      if (seqPicked[idx] < seqPickable[idx]) que.emplace(seq[idx][seqPicked[idx]], idx);
      picked++;
    }

    int diff = lastPicked - firstPicked;
    if (diff < minDiff) minDiff = diff;
  }

  cout << minDiff << endl;

  return 0;
}
