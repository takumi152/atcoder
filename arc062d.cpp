#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  string s;
  cin >> s;

  int N = s.size();
  int pLeft = 0;
  int score = 0;
  for (int i = 0; i < N; i++) {
    if (s[i] == 'g') {
      if (pLeft > 0) {
        score++;
        pLeft--;
      }
      else {
        pLeft++;
      }
    }
    else { // s[i] == 'p'
      if (pLeft > 0) {
        pLeft--;
      }
      else {
        pLeft++;
        score--;
      }
    }
  }

  cout << score << endl;
  return 0;
}
