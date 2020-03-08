#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <cmath>

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned short nextUShort() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUShortMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x000000000000ffff) * mod) >> 48;
  }
  inline unsigned int nextUInt() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextUIntMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x00000000ffffffff) * mod) >> 32;
  }
  inline unsigned long long int nextULL() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline double nextDouble() {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return (double)x * 5.42101086242752217e-20;
  }
};

struct timer {
  double t = 0.0;
  timer() {
    restart();
  }
  inline void restart() {
    t = now();
  }
  inline double time() {
    return now() - t;
  }
  inline double now() {
    unsigned long long l, h;
    __asm__ ("rdtsc" : "=a"(l), "=d"(h));
    return (double)(l | h << 32) * 3.5714285714285715e-10; // 1 / 2.8e9
  }
};

//#define DEBUG

using namespace std;

typedef long long int ll;
typedef pair<int, int> Pii;

const ll mod = 1000000007;

timer theTimer;
xorshift64 theRandom;

vector<vector<bool> > board;

int calcScore() {
  int score = 0;
  int n = board.size();
  int m = board[0].size();
  vector<vector<int> > boardSum(n+1, vector<int>(m+1));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      boardSum[i+1][j+1] = boardSum[i][j+1] + boardSum[i+1][j] - boardSum[i][j];
      if (board[i][j]) boardSum[i+1][j+1]++;
    }
  }
  for (int i1 = 0; i1 < n; i1++) {
    for (int i2 = i1+1; i2 <= n; i2++) {
      for (int j1 = 0; j1 < m; j1++) {
        for (int j2 = j1+1; j2 <= m; j2++) {
          if ((boardSum[i2][j2] - boardSum[i1][j2] - boardSum[i2][j1] + boardSum[i1][j1]) % 2 == 1) score++;
        }
      }
    }
  }
  return score;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int n, m;
  cin >> n >> m;

  board = vector<vector<bool> >((1 << n) - 1, vector<bool>((1 << m) - 1, true));

  for (int i = 0; i < (1 << n) - 1; i++) {
    for (int j = 0; j < (1 << m) - 1; j++) {
      if ((i % 8 == 3) || (j % 8 == 3)) board[i][j] = true;
      else {
        bool xo = false;
        int num = 2;
        while (true) {
          if (num == 4) {
            num *= 2;
          }
          if ((i % num == num-1 && j % num == num-1)) {
            xo = !xo;
            num *= 2;
          }
          else break;
        }
        board[i][j] = xo;
      }
    }
  }

  vector<vector<bool> > bestBoard = board;

  if (n <= 2 && m <= 2) {
    int score = calcScore();
    int lastScore = score;
    int bestScore = score;

    double baseTemperature = 1e1;
    double temperature = baseTemperature;
    double decayRate = 1e-6;
    double timeLimit = 1.000;
    int iterCount = 0;

    while (theTimer.time() < timeLimit) {
      int p1 = theRandom.nextUIntMod((1 << n) - 1);
      int p2 = theRandom.nextUIntMod((1 << m) - 1);

      board[p1][p2] = !board[p1][p2];

      score = calcScore();

      #ifdef DEBUG
      if (iterCount % 1000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << bestScore << " " << temperature << " " << endl;
      #endif

      if (score >= lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          bestBoard = board;
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) { // accept
        lastScore = score;
      }
      else { // rollback
        board[p1][p2] = !board[p1][p2];
      }

      iterCount++;
      temperature *= 1.0 - decayRate;
    }

    board = bestBoard;

    cerr << "iterCount   = " << iterCount << endl;
    cerr << "temperature = " << temperature << endl;
    cerr << "bestScore   = " << bestScore << endl;
  }

  for (int i = 0; i < (1 << n) - 1; i++) {
    for (int j = 0; j < (1 << m) - 1; j++) {
      if (board[i][j]) cout << 1;
      else cout << 0;
    }
    cout << endl;
  }

  return 0;
}
