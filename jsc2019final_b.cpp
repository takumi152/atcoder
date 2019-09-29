#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll MOD = 1000000007;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int N;
  cin >> N;
  vector<string> A(N), B(N);
  for (auto &x: A) cin >> x;
  for (auto &y: B) cin >> y;

  vector<vector<bool> > xy(N, vector<bool>(N));
  vector<vector<bool> > xz(N, vector<bool>(N));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      xy[i][j] = (A[i][j] == '1');
      xz[i][j] = (B[i][j] == '1');
    }
  }

  vector<vector<bool> > yz(N, vector<bool>(N, true));
  for (int j = 0; j < N; j++) {
    for (int i = 0; i < N; i++) {
      if (xy[i][j]) {
        for (int k = 0; k < N; k++) {
          if (!xz[i][k]) {
            yz[j][k] = false;
          }
        }
      }
    }
  }

  // sanity check
  bool ok = true;
  for (int i = 0; i < N; i++) {
    for (int k = 0; k < N; k++) {
      if (xz[i][k]) {
        bool found = false;
        for (int j = 0; j < N; j++) {
          if (xy[i][j] && yz[j][k]) {
            found = true;
            break;
          }
        }
        if (!found) ok = false;
      }
      else {
        for (int j = 0; j < N; j++) {
          if (xy[i][j] && yz[j][k]) {
            ok = false;
            break;
          }
        }
      }
    }
  }

  if (!ok) {
    cout << -1 << endl;
    return 0;
  }

  for (int j = 0; j < N; j++) {
    for (int k = 0; k < N; k++) {
      if (yz[j][k]) cout << 1;
      else cout << 0;
    }
    cout << endl;
  }

  return 0;
}
