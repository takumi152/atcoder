#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <bitset>

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 998244353;

pair<Pii, Pii> move_dir(char dir, int a, int c) {
       if (dir == 'U') return pair<Pii, Pii>(Pii(a, c), Pii(a, c+1));
  else if (dir == 'D') return pair<Pii, Pii>(Pii(a, c+1), Pii(a, c));
  else if (dir == 'L') return pair<Pii, Pii>(Pii(a+1, c), Pii(a, c));
  else if (dir == 'R') return pair<Pii, Pii>(Pii(a, c), Pii(a+1, c));
  else                 return pair<Pii, Pii>(Pii(998244353, 998244353), Pii(998244353, 998244353)); // ???
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int sx, sy;
  cin >> sx >> sy;
  int tx, ty;
  cin >> tx >> ty;
  int a, b, c, d;
  cin >> a >> b >> c >> d;

  if ((sx - tx) % 2 != 0 || (sy - ty) % 2 != 0) {
    cout << "No" << endl;
    return 0;
  }

  bool can_move_horizontal = a < b;
  bool can_move_vertical = c < d;

  if (!can_move_horizontal && !can_move_vertical) {
    int dx = sx + (a - sx) * 2;
    int dy = sy + (c - sy) * 2;
    if (sx == tx && sy == ty) {
      cout << "Yes" << endl;
      return 0;
    }
    else if (dx == tx && dy == ty) {
      cout << "Yes" << endl;
      cout << a << " " << c << endl;
      return 0;
    }
    else {
      cout << "No" << endl;
      return 0;
    }
  }
  else if (!can_move_horizontal) {
    int dx = sx + (a - sx) * 2;
    if (sx != tx && dx != tx) {
      cout << "No" << endl;
      return 0;
    }
    cout << "Yes" << endl;
    if (sx != tx) {
      int dy = sy + (c - sy) * 2;
      cout << a << " " << c << endl;
      sx = dx;
      sy = dy;
    }
  }
  else if (!can_move_vertical) {
    int dy = sy + (c - sy) * 2;
    if (sy != ty && dy != ty) {
      cout << "No" << endl;
      return 0;
    }
    cout << "Yes" << endl;
    if (sy != ty) {
      int dx = sx + (a - sx) * 2;
      cout << a << " " << c << endl;
      sx = dx;
      sy = dy;
    }
  }
  else {
    cout << "Yes" << endl;
  }

  while (sx < tx) {
    auto [command1, command2] = move_dir('R', a, c);
    cout << command1.first << " " << command1.second << endl;
    cout << command2.first << " " << command2.second << endl;
    sx += 2;
  }
  while (sx > tx) {
    auto [command1, command2] = move_dir('L', a, c);
    cout << command1.first << " " << command1.second << endl;
    cout << command2.first << " " << command2.second << endl;
    sx -= 2;
  }
  while (sy < ty) {
    auto [command1, command2] = move_dir('U', a, c);
    cout << command1.first << " " << command1.second << endl;
    cout << command2.first << " " << command2.second << endl;
    sy += 2;
  }
  while (sy > ty) {
    auto [command1, command2] = move_dir('D', a, c);
    cout << command1.first << " " << command1.second << endl;
    cout << command2.first << " " << command2.second << endl;
    sy -= 2;
  }

  return 0;
}
