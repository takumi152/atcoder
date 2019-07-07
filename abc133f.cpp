// WIP

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include <stack>

using namespace std;

typedef long long int ll;
typedef pair<int, pair<int, int> > PiPii;
typedef pair<pair<int, int>, pair<int, int> > dPii;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, Q;
  cin >> N >> Q;
  vector<set<PiPii> > edge;
  for (int i = 0; i < N; i++) edge.push_back(set<PiPii>());
  for (int i = 0; i < N-1; i++) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    edge[a-1].insert(b-1, Pii(c, d)); // zero index
    edge[b-1].insert(a-1, Pii(c, d)); // zero index
  }
  vector<dPii> query;
  for (int i = 0; i < Q; i++) {
    int x, y, u, v;
    cin >> x >> y >> u >> v;
    query.push_back(Pii(x, y), Pii(u, v));
  }



  return 0;
}
