// WIP

#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

#define REP(i, n) for (int i = 0; i < n; i++)

using namespace std;

typedef pair<int, int> Pii;

int x[100010], y[100010];
char d[100010];

const Pii none = Pii(-1e9, 1e9);

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int N;
  cin >> N;
  vector<int> Ux, Uy;
  vector<int> Dx, Dy;
  vector<int> Lx, Ly;
  vector<int> Rx, Ry;
  REP(i, N) {
    cin >> x[i] >> y[i] >> d[i];
  }
  REP(i, N) {
    if (d[i] == 'U'){
      Ux.push_back(x[i]);
      Uy.push_back(y[i]);
    }
    else if (d[i] == 'D'){
      Dx.push_back(x[i]);
      Dy.push_back(y[i]);
    }
    else if (d[i] == 'L'){
      Lx.push_back(x[i]);
      Ly.push_back(y[i]);
    }
    else if (d[i] == 'R'){
      Rx.push_back(x[i]);
      Ry.push_back(y[i]);
    }
  }
  Pii Ux_range, Uy_range;
  Pii Dx_range, Dy_range;
  Pii Lx_range, Ly_range;
  Pii Rx_range, Ry_range;
  if (Ux.size() > 0) {
    Ux_range = Pii(*min_element(Ux.begin(), Ux.end()), *max_element(Ux.begin(), Ux.end()));
    Uy_range = Pii(*min_element(Uy.begin(), Uy.end()), *max_element(Uy.begin(), Uy.end()));
  } else {
    Ux_range = none;
    Uy_range = none;
  }
  cout << Ux_range.first << " " << Ux_range.second << endl;
  return 0;
}
