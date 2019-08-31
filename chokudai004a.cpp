#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx")

#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <climits>
#include <cassert>

class xorshift128 {
public:
    using result_type = unsigned int;
private:
    unsigned int x;
    unsigned int y = 362436069;
    unsigned int z = 521288629;
    unsigned int w = 88675123;
    unsigned int t;
public:
    xorshift128(unsigned int seed) {x = seed;}
    static constexpr result_type min() {return 0u;}
    static constexpr result_type max() {return UINT_MAX;}
    unsigned int nextRand();
    result_type operator()() {return nextRand();}
};
xorshift128::result_type xorshift128::nextRand() {
    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

using namespace std;

typedef long long int ll;

int N, B1, B2, B3;
char board[30][30];
char bestBoard[30][30];
char l[30][30];
char r[30][30];

bool x1b[31][31][31];
bool x2b[31][31][31];
bool x3b[31][31][31];
bool y1b[31][31][31];
bool y2b[31][31][31];
bool y3b[31][31][31];

short x1c = 0;
short x2c = 0;
short x3c = 0;
short y1c = 0;
short y2c = 0;
short y3c = 0;

auto timeBegin = chrono::steady_clock::now();
auto timeLimit = chrono::microseconds(2995 * 1000);
auto timeElapsed = chrono::microseconds(0);

int calcScoreAll() {
  x1c = 0;
  x2c = 0;
  x3c = 0;
  y1c = 0;
  y2c = 0;
  y3c = 0;
  for (short i = 0; i < N; i++) {
    for (short j = 0; j < N; j++) {
      for (short k = 0; k < N; k++) {
        x1b[i][j][k] = false;
        x2b[i][j][k] = false;
        x3b[i][j][k] = false;
        y1b[i][j][k] = false;
        y2b[i][j][k] = false;
        y3b[i][j][k] = false;
      }
    }
  }
  // vertical
  for (short i = 0; i < N; i++) {
    for (short j = 0; j < N; j++) {
      short s = 0;
      for (short k = j; k < N; k++) {
        s += board[i][k];
        if (s == B1) {
          x1c++;
          x1b[i][j][k] = true;
        }
        else if (s == B2) {
          x2c++;
          x2b[i][j][k] = true;
        }
        else if (s == B3) {
          x3c++;
          x3b[i][j][k] = true;
        }
        else if (s > B3) break;
      }
    }
  }
  // horizontal
  for (short j = 0; j < N; j++) {
    for (short i = 0; i < N; i++) {
      char s = 0;
      for (short k = i; k < N; k++) {
        s += board[k][j];
        if (s == B1) {
          y1c++;
          y1b[i][j][k] = true;
        }
        else if (s == B2) {
          y2c++;
          y2b[i][j][k] = true;
        }
        else if (s == B3) {
          y3c++;
          y3b[i][j][k] = true;
        }
        else if (s > B3) break;
      }
    }
  }
  int score = (x1c + y1c) * B1 + (x2c + y2c) * B2 + (x3c + y3c) * B3;
  return score;
}

void updateScorePart(short x, short y) {
  char s;
  char t;
  // vertical
  t = -board[x][y];
  for (short j = y; j >= 0; j--) {
    t += board[x][j];
    if (t > B3+9) break;
    s = t;
    for (short k = y; k < N; k++) {
      s += board[x][k];
      if (s == B1 && !x1b[x][j][k]) {
        x1c++;
        x1b[x][j][k] = true;
      }
      else if (s != B1 && x1b[x][j][k]) {
        x1c--;
        x1b[x][j][k] = false;
      }
      if (s == B2 && !x2b[x][j][k]) {
        x2c++;
        x2b[x][j][k] = true;
      }
      else if (s != B2 && x2b[x][j][k]) {
        x2c--;
        x2b[x][j][k] = false;
      }
      if (s == B3 && !x3b[x][j][k]) {
        x3c++;
        x3b[x][j][k] = true;
      }
      else if (s != B3 && x3b[x][j][k]) {
        x3c--;
        x3b[x][j][k] = false;
      }
      if (s > B3+9) break;
    }
  }
  // horizontal
  t = -board[x][y];
  for (short i = x; i >= 0; i--) {
    t += board[i][y];
    if (t > B3+9) break;
    s = t;
    for (short k = x; k < N; k++) {
      s += board[k][y];
      if (s == B1 && !y1b[i][y][k]) {
        y1c++;
        y1b[i][y][k] = true;
      }
      else if (s != B1 && y1b[i][y][k]) {
        y1c--;
        y1b[i][y][k] = false;
      }
      if (s == B2 && !y2b[i][y][k]) {
        y2c++;
        y2b[i][y][k] = true;
      }
      else if (s != B2 && y2b[i][y][k]) {
        y2c--;
        y2b[i][y][k] = false;
      }
      if (s == B3 && !y3b[i][y][k]) {
        y3c++;
        y3b[i][y][k] = true;
      }
      else if (s != B3 && y3b[i][y][k]) {
        y3c--;
        y3b[i][y][k] = false;
      }
      if (s > B3+9) break;
    }
  }
}

int calcScorePart(short x, short y) {
  updateScorePart(x, y);
  int score = (x1c + y1c) * B1 + (x2c + y2c) * B2 + (x3c + y3c) * B3;
  return score;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  cin >> N >> B1 >> B2 >> B3;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      short buf;
      cin >> buf;
      l[i][j] = buf;
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      short buf;
      cin >> buf;
      r[i][j] = buf;
    }
  }

  unsigned int seed = random_device()();
  xorshift128 theRandom(seed);

  uniform_real_distribution<> distr(0.0, 1.0);
  uniform_int_distribution<> distp(0, 29);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      board[i][j] = l[i][j];
      bestBoard[i][j] = l[i][j];
    }
  }

  double temperature = 3e1;
  double decayRate = 6e-7;

  int score = calcScoreAll();
  int lastScore = score;
  int bestScore = score;

  int iterCount = 0;
  int nextCheck = 0;

  while (true) {
    iterCount++;
    if (iterCount >= nextCheck) {
      if ((timeElapsed = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - timeBegin)) >= timeLimit) break;
      nextCheck += (timeLimit - timeElapsed).count() >> 1;
    }
    short x1 = theRandom.nextRand() % 30;
    short y1 = theRandom.nextRand() % 30;
    short lastNum11 = board[x1][y1];
    board[x1][y1] = theRandom.nextRand() % (r[x1][y1] - l[x1][y1] + 1) + l[x1][y1];
    if (board[x1][y1] == lastNum11) continue;
    score = calcScorePart(x1, y1);
    if (score > bestScore) {
      bestScore = score;
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          bestBoard[i][j] = board[i][j];
        }
      }
    }
    if (score > lastScore) {
      lastScore = score;
    }
    else if (distr(theRandom) > exp((score - lastScore) / temperature)) {
      board[x1][y1] = lastNum11;
      updateScorePart(x1, y1);
    }
    else {
      lastScore = score;
    }
    temperature *= 1.0 - decayRate;
    //if ((iterCount & 65535) == 0) cerr << lastScore << " " << bestScore << " " << temperature << " " << exp((score - lastScore) / temperature) << endl;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cout << (short)bestBoard[i][j];
      if (j < N-1) cout << " ";
      else cout << endl;
    }
  }

  cerr << "iterCount   = " << iterCount << endl;
  cerr << "bestScore   = " << bestScore << endl;
  cerr << "temperature = " << temperature << endl;

  return 0;
}
