#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>
#include <cmath>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<int> a(N);
  for (auto &x: a) cin >> x;
  vector<int> b(N);
  for (auto &x: b) cin >> x;

  const int M = 100000;

  vector<int> bucketA(M+1);
  vector<int> bucketB(M+1);
  for (int i = 0; i < N; i++) bucketB[b[i]]++;
  int minRepeat = N;
  vector<int> score;
  for (int i = 0; i < N; i++) {
    bucketA[a[i]]++;
    int repeat = bucketB[a[i]] / bucketA[a[i]];
    if (repeat < minRepeat) minRepeat = repeat;
    score.push_back(minRepeat);
  }

  for (auto &x: score) cout << x << '\n';

  return 0;
}
