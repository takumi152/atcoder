#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m, q;
  cin >> n >> m >> q;
  vector<vector<vector<int> > > bus_time(n);
  for (int i = 0; i < m; i++) {
    int a, b, s, t;
    cin >> a >> b >> s >> t;
    bus_time[a-1].push_back(vector<int>({s*2+1, t*2+1, b-1, i})); // start, end, target, id
  }
  vector<vector<int> > query(q, vector<int>(3));
  for (auto &x: query) {
    cin >> x[0] >> x[1] >> x[2];
    x[0] *= 2;
    x[2] *= 2;
  }

  vector<vector<int> > bus_id(m);
  for (auto &x: bus_time) {
    for (auto &y: x) bus_id[y[3]] = vector<int>({x+1, y[2]+1});
  }

  for (int i = 0; i < m; i++) sort(bus[i].begin(), bus[i].end());

  vector<vector<vector<int> > > city(n);
  vector<vector<int> > bus(m);
  for (int i = 0; i < q; i++) {
    int now = query[i][1];
    int t = query[i][0];
    while (true) {
      auto next_bus = lower_bound(bus_time[now].begin(), bus_time[now].end(), vector<int>({t}));
      if (next_bus != bus_time.end()) {
        city[i].push_back(vector<int>({t, *(next_bus)[3]}));
        if (!bus[*(next_bus)[3]].empty()) break; // bus already used
        bus[*(next_bus)[3]] = vector<int>({*(next_bus)[0], *(next_bus)[2]});
        now = *(next_bus)[2];
        t = *(next_bus)[1];
      }
      else { // no bus found
        city[i].push_back(vector<int>({t, -1}));
        break;
      }
    }
  }

  for (int i = 0; i < n; i++) sort(city[i].begin(), city[i].end());

  // doubling
  vector<vector<vector<vector<int> > > > city_doubling(n);
  vector<vector<vector<int> > > bus_doubling(m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < (int) city[i].size(); j++) city_doubling[i].push_back(vector<vector<int> >(20));
  }

  return 0;
}
