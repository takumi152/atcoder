// WIP

#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <utility>

using namespace std;

int main() {
  int H, W, N;
  cin >> H >> W >> N;
  int sr, sc;
  cin >> sr >> sc;
  string S, T;
  cin >> S;
  cin >> T;
  int SL = 0, SR = 0, SU = 0, SD = 0;
  int TL = 0, TR = 0, TU = 0, TD = 0;
  for (int i = 0; i < N; i++) {
    if (S[i] == 'L') SL++;
    else if (S[i] == 'R') SR++;
    else if (S[i] == 'U') SU++;
    else if (S[i] == 'D') SD++;
    if (T[i] == 'L') TL++;
    else if (T[i] == 'R') TR++;
    else if (T[i] == 'U') TU++;
    else if (T[i] == 'D') TD++;
  }
  // left / right
  int SScL = sc, SScR = sc, TScL = sc, TScR = sc;
  for (int i = 0; i < N; i++) {
    if (S[i] == 'L') SScL--;
    else if (S[i] == 'R') SR++;
    if (T[i] == 'L') TL++;
    else if (T[i] == 'R') TR++;
  }
}
