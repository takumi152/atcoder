#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N, K;
  cin >> N >> K;
  vector<Pii> xy;
  for (int i = 0; i < N; i++) {
    int x, y;
    cin >> x >> y;
    xy.push_back(Pii(x, y));
  }

  vector<Pii> xyX = xy;
  sort(xyX.begin(), xyX.end());

  ll smallest = (ll) 9e18;
  for (int i = 0; i < N; i++) {
    vector<Pii> xyXT;
    for (int j = i; j < min(i + K - 1, N); j++) xyXT.push_back(xyX[j]);
    ll down = xyXT[0].first, up = xyXT[xyXT.size()-1].first;
    for (int j = i + K - 1; j < N; j++) {
      xyXT.push_back(xyX[j]);
      up = xyXT[xyXT.size()-1].first;
      sort(xyXT.begin(), xyXT.end(), [](Pii a, Pii b){return a.second < b.second;});
      for (int k = 0; k < xyXT.size(); k++) {
        for (int l = k + K - 1; l < xyXT.size(); l++) {
          ll left = xyXT[k].second, right = xyXT[l].second;
          ll field = (up - down) * (right - left);
          if (field < smallest) smallest = field;
        }
      }
    }
  }

  cout << smallest << endl;

  return 0;
}
