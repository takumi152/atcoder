#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  vector<vector<int> > c(3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int buf;
      cin >> buf;
      c[i].push_back(buf);
    }
  }

  vector<int> a(3);
  vector<int> b(3);

  for (int i = 0; i <= c[0][0]; i++) {
    a[0] = i;
    b[0] = c[0][0] - i;
    for (int j = 0; j <= c[1][1]; j++) {
      a[1] = j;
      b[1] = c[1][1] - j;
      for (int k = 0; k <= c[2][2]; k++) {
        a[2] = k;
        b[2] = c[2][2] - k;
        bool correct = true;
        for (int x = 0; x < 3; x++) {
          for (int y = 0; y < 3; y++) {
            if (a[x] + b[y] != c[x][y]) correct = false;
          }
        }
        if (correct) {
          cout << "Yes" << endl;
          return 0;
        }
      }
    }
  }
  cout << "No" << endl;
  return 0;
}
