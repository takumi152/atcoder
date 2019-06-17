// C++14 implemention of abc128e.py
// still TLE

#include <iostream>
#include <algorithm>
#include <tuple>

using namespace std;

typedef tuple<int, int, int> Ti3;

int N, Q;
Ti3 XST[200010];
int D[200010];
int dist[200010];

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  cin >> N >> Q;
  for (int i = 0; i < N; i++) {
    int S, T, X;
    cin >> S >> T >> X;
    XST[i] = make_tuple(X, S - X, T - X);
  }
  for (int i = 0; i < Q; i++) {
    cin >> D[i];
    dist[i] = -1;
  }
  sort(XST, XST+N);
  for (int i = 0; i < N; i++) {
    int point_begin = distance(D, lower_bound(D, D+Q, get<1>(XST[i])));
    int point_end = distance(D, lower_bound(D, D+Q, get<2>(XST[i])));
    for (int point = point_begin; point < point_end; point++) {
      if (dist[point] == -1) dist[point] = get<0>(XST[i]);
    }
  }
  for (int i = 0; i < Q; i++) {
    cout << dist[i] << endl;
  }
  return 0;
}
