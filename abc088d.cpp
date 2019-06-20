#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int H, W;
  cin >> H >> W;
  vector<string> s;
  for (int i = 0; i < H; i++) {
    string buf;
    cin >> buf;
    s.push_back(buf);
  }

  int white_count = 0;
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      if (s[i][j] == '.') white_count += 1;
    }
  }

  const int unreachable = 1000000007;
  vector<vector<int> > dist(H, vector<int>(W, unreachable));
  dist[0][0] = 0;
  bool updated = true;
  while (updated) {
    updated = false;
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
        if (dist[i][j] != unreachable) {
          if (i > 0) {
            if (s[i-1][j] == '.' && dist[i-1][j] > dist[i][j] + 1) {
              dist[i-1][j] = dist[i][j] + 1;
              updated = true;
            }
          }
          if (j > 0) {
            if (s[i][j-1] == '.' && dist[i][j-1] > dist[i][j] + 1) {
              dist[i][j-1] = dist[i][j] + 1;
              updated = true;
            }
          }
          if (i < H-1) {
            if (s[i+1][j] == '.' && dist[i+1][j] > dist[i][j] + 1) {
              dist[i+1][j] = dist[i][j] + 1;
              updated = true;
            }
          }
          if (j < W-1) {
            if (s[i][j+1] == '.' && dist[i][j+1] > dist[i][j] + 1) {
              dist[i][j+1] = dist[i][j] + 1;
              updated = true;
            }
          }
        }
      }
    }
  }
  if (dist[H-1][W-1] == unreachable) {
    cout << -1 << endl;
  }
  else {
    int score = white_count - dist[H-1][W-1] - 1;
    cout << score << endl;
  }
  return 0;
}
