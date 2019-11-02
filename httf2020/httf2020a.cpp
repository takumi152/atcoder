#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#pragma GCC target ("avx")

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <functional>
#include <numeric>
#include <random>
#include <chrono>
#include <cstring>
#include <climits>
#include <cassert>

using namespace std;

typedef long long int ll;

struct xorshift64 {
  unsigned long long int x = 88172645463325252ULL;
  inline unsigned int nextInt() {
    x = x ^ (x << 7);
    return x = x ^ (x >> 9);
  }
  inline unsigned int nextIntMod(unsigned long long int mod) {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return ((x & 0x00000000ffffffff) * mod) >> 32;
  }
  inline double nextDouble() {
    x = x ^ (x << 7);
    x = x ^ (x >> 9);
    return (double)x * 5.42101086242752217e-20;
  }
};

#define DEBUG1
//#define DEBUG2

auto timeBegin = chrono::steady_clock::now();
auto timeLimit = chrono::microseconds(2995 * 1000);
auto timeElapsed = chrono::microseconds(0);

xorshift64 theRandom;

constexpr int N = 40;
constexpr int M = 100;
constexpr int B = 300;

char board[N][N];
char bestBoard[N][N];
short finishX, finishY;
short initRobotX[100];
short initRobotY[100];
char initRobotDirection[100];
char direction[5] = {'U', 'D', 'L', 'R', '.'};

bool passed[4][N][N];
bool allPassed[N][N];
bool allDirPassed[4][N][N];
unsigned long long passedLL[4][N];
unsigned long long allPassedLL[N];
unsigned long long allDirPassedLL[4][N];

inline int codeOfDir(char x) {
  if (x == 'U') return 0;
  else if (x == 'D') return 1;
  else if (x == 'L') return 2;
  else if (x == 'R') return 3;
  else return 0;
}

int calcScore() {
  int finished = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      allPassed[i][j] = false;
    }
  }
  for (int k = 0; k < M; k++) {
    int dir = codeOfDir(initRobotDirection[k]);
    int nowY = initRobotY[k], nowX = initRobotX[k];
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    while (true) {
      if (board[nowY][nowX] == '#') break;
      if (board[nowY][nowX] == 'G') {
        finished++;
        allPassed[nowY][nowX] = true;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (passed[dir][nowY][nowX]) break;
      passed[dir][nowY][nowX] = true;
      allPassed[nowY][nowX] = true;
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          if (nextX < 0) nextX = N-1;
          break;
        case 3: // R
          nextX++;
          if (nextX >= N) nextX = 0;
          break;
      }
      nowX = nextX, nowY = nextY;
    }
    dir = codeOfDir(initRobotDirection[k]);
    nowY = initRobotY[k], nowX = initRobotX[k];
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    while (true) {
      if (board[nowY][nowX] == '#') break;
      if (board[nowY][nowX] == 'G') {
        passed[dir][nowY][nowX] = false;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (!passed[dir][nowY][nowX]) break;
      passed[dir][nowY][nowX] = false;
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          if (nextX < 0) nextX = N-1;
          break;
        case 3: // R
          nextX++;
          if (nextX >= N) nextX = 0;
          break;
      }
      nowX = nextX, nowY = nextY;
    }
  }
  int score = finished * 1000;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (allPassed[i][j]) score++;
      if (!(board[i][j] == '.' || board[i][j] == '#' || board[i][j] == 'G')) score -= 10;
    }
  }
  return score;
}

int calcScoreFast() {
  int finished = 0;
  for (int d = 0; d < 4; d++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        allDirPassed[d][i][j] = false;
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      allPassed[i][j] = false;
    }
  }
  for (int k = 0; k < M; k++) {
    int dir = codeOfDir(initRobotDirection[k]);
    int nowY = initRobotY[k], nowX = initRobotX[k];
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    bool didFinish = false;
    while (true) {
      if (board[nowY][nowX] == '#') break;
      if (board[nowY][nowX] == 'G') {
        finished++;
        allPassed[nowY][nowX] = true;
        didFinish = true;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (passed[dir][nowY][nowX]) break;
      passed[dir][nowY][nowX] = true;
      allPassed[nowY][nowX] = true;
      if (allDirPassed[dir][nowY][nowX]) {
        finished++;
        didFinish = true;
        break;
      }
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          if (nextX < 0) nextX = N-1;
          break;
        case 3: // R
          nextX++;
          if (nextX >= N) nextX = 0;
          break;
      }
      nowX = nextX, nowY = nextY;
    }
    dir = codeOfDir(initRobotDirection[k]);
    nowY = initRobotY[k], nowX = initRobotX[k];
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    while (true) {
      if (board[nowY][nowX] == '#') break;
      if (board[nowY][nowX] == 'G') {
        passed[dir][nowY][nowX] = false;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (!passed[dir][nowY][nowX]) break;
      passed[dir][nowY][nowX] = false;
      if (didFinish) allDirPassed[dir][nowY][nowX] = true;
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          if (nextX < 0) nextX = N-1;
          break;
        case 3: // R
          nextX++;
          if (nextX >= N) nextX = 0;
          break;
      }
      nowX = nextX, nowY = nextY;
    }
  }
  int score = finished * 1000;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (allPassed[i][j]) score++;
      if (!(board[i][j] == '.' || board[i][j] == '#' || board[i][j] == 'G')) score -= 10;
    }
  }
  return score;
}

int calcScoreFaster() {
  int finished = 0;
  for (int d = 0; d < 4; d++) {
    for (int i = 0; i < N; i++) {
      allDirPassedLL[d][i] = 0ULL;
    }
  }
  for (int i = 0; i < N; i++) {
    allPassedLL[i] = 0ULL;
  }
  for (int k = 0; k < M; k++) {
    int dir = codeOfDir(initRobotDirection[k]);
    int nowY = initRobotY[k], nowX = initRobotX[k];
    unsigned long long nowXmask = 1ULL << nowX;
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    bool didFinish = false;
    while (true) {
      if (board[nowY][nowX] == '#') break;
      if (board[nowY][nowX] == 'G') {
        finished++;
        allPassedLL[nowY] |= nowXmask;
        didFinish = true;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (passedLL[dir][nowY] & nowXmask) break;
      passedLL[dir][nowY] |= nowXmask;
      allPassedLL[nowY] |= nowXmask;
      if (allDirPassedLL[dir][nowY] & nowXmask) {
        finished++;
        didFinish = true;
        break;
      }
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          nowXmask >>= 1;
          if (nextX < 0) {
            nextX = N-1;
            nowXmask = 1ULL << (N-1);
          }
          break;
        case 3: // R
          nextX++;
          nowXmask <<= 1;
          if (nextX >= N) {
            nextX = 0;
            nowXmask = 1ULL;
          }
          break;
      }
      nowX = nextX, nowY = nextY;
    }
    dir = codeOfDir(initRobotDirection[k]);
    nowY = initRobotY[k], nowX = initRobotX[k];
    nowXmask = 1ULL << nowX;
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    while (true) {
      if (board[nowY][nowX] == '#') break;
      if (board[nowY][nowX] == 'G') {
        passedLL[dir][nowY] &= ~nowXmask;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (!(passedLL[dir][nowY] & nowXmask)) break;
      passedLL[dir][nowY] &= ~nowXmask;
      if (didFinish) allDirPassedLL[dir][nowY] |= nowXmask;
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          nowXmask >>= 1;
          if (nextX < 0) {
            nextX = N-1;
            nowXmask = 1ULL << (N-1);
          }
          break;
        case 3: // R
          nextX++;
          nowXmask <<= 1;
          if (nextX >= N) {
            nextX = 0;
            nowXmask = 1ULL;
          }
          break;
      }
      nowX = nextX, nowY = nextY;
    }
  }
  int score = finished * 1000;
  for (int i = 0; i < N; i++) {
    score += __builtin_popcountll(allPassedLL[i]);
    for (int j = 0; j < N; j++) {
      if (!(board[i][j] == '.' || board[i][j] == '#' || board[i][j] == 'G')) score -= 10;
    }
  }
  return score;
}

int moveStuckRobot() {
  vector<vector<int> > stuckRobot;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      allPassed[i][j] = false;
    }
  }
  for (int k = 0; k < M; k++) {
    int dir = codeOfDir(initRobotDirection[k]);
    int nowY = initRobotY[k], nowX = initRobotX[k];
    int lastY = nowY, lastX = nowX;
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    while (true) {
      if (board[nowY][nowX] == '#') {
        vector<int> newStuckRobot = {initRobotY[k], initRobotX[k], dir};
        stuckRobot.push_back(newStuckRobot);
        break;
      }
      if (board[nowY][nowX] == 'G') {
        allPassed[nowY][nowX] = true;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (passed[dir][nowY][nowX]) break;
      passed[dir][nowY][nowX] = true;
      allPassed[nowY][nowX] = true;
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          if (nextX < 0) nextX = N-1;
          break;
        case 3: // R
          nextX++;
          if (nextX >= N) nextX = 0;
          break;
      }
      lastX = nowX, lastY = nowY;
      nowX = nextX, nowY = nextY;
    }
    dir = codeOfDir(initRobotDirection[k]);
    nowY = initRobotY[k], nowX = initRobotX[k];
    if (board[initRobotY[k]][initRobotX[k]] != '.') dir = codeOfDir(board[initRobotY[k]][initRobotX[k]]);
    while (true) {
      if (board[nowY][nowX] == '#') break;
      if (board[nowY][nowX] == 'G') {
        passed[dir][nowY][nowX] = false;
        break;
      }
      if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
      if (!passed[dir][nowY][nowX]) break;
      passed[dir][nowY][nowX] = false;
      int nextX = nowX, nextY = nowY;
      switch (dir) {
        case 0: // U
          nextY--;
          if (nextY < 0) nextY = N-1;
          break;
        case 1: // D
          nextY++;
          if (nextY >= N) nextY = 0;
          break;
        case 2: // L
          nextX--;
          if (nextX < 0) nextX = N-1;
          break;
        case 3: // R
          nextX++;
          if (nextX >= N) nextX = 0;
          break;
      }
      nowX = nextX, nowY = nextY;
    }
  }
  int updated = 0;
  for (auto &robot: stuckRobot) {
    int fromY = robot[0], fromX = robot[1], fromD = robot[2];
    priority_queue<pair<int, vector<vector<int> > > > que;
    que.emplace(-10, vector<vector<int> >({{fromY, fromX, 0}}));
    que.emplace(-10, vector<vector<int> >({{fromY, fromX, 1}}));
    que.emplace(-10, vector<vector<int> >({{fromY, fromX, 2}}));
    que.emplace(-10, vector<vector<int> >({{fromY, fromX, 3}}));
    priority_queue<pair<int, vector<vector<int> > > > candidate;
    while (!que.empty()) {
      auto now = que.top();
      auto history = now.second;
      int point = now.first, nowY = history[history.size()-1][0], nowX = history[history.size()-1][1], nowdir = history[history.size()-1][2];
      que.pop();
      for (int k = 0; k < 40; k++) {
        int nextY = nowY, nextX = nowX;
        switch (nowdir) {
          case 0: // U
            nextY--;
            if (nextY < 0) nextY = N-1;
            break;
          case 1: // D
            nextY++;
            if (nextY >= N) nextY = 0;
            break;
          case 2: // L
            nextX--;
            if (nextX < 0) nextX = N-1;
            break;
          case 3: // R
            nextX++;
            if (nextX >= N) nextX = 0;
            break;
        }
        nowY = nextY, nowX = nextX;
        if (board[nowY][nowX] == '#') break;
        else if (board[nowY][nowX] != '.') {
          candidate.emplace(point, history);
          break;
        }
        else if (!allPassed[nowY][nowX]) {
          point++;
          if (history.size() < 4) {
            if (nowdir == 0 || nowdir == 1) {
              auto newHistory = history;
              newHistory.push_back(vector<int>({nowY, nowX, 2}));
              que.emplace(point-10, newHistory);
              newHistory[newHistory.size()-1][2] = 3;
              que.emplace(point-10, newHistory);
            }
            else {
              auto newHistory = history;
              newHistory.push_back(vector<int>({nowY, nowX, 0}));
              que.emplace(point-10, newHistory);
              newHistory[newHistory.size()-1][2] = 1;
              que.emplace(point-10, newHistory);
            }
          }
        }
      }
    }
    if (!candidate.empty()) {
      auto now = candidate.top();
      auto history = now.second;
      for (auto &p: history) {
        board[p[0]][p[1]] = direction[p[2]];
      }
      if (board[history[0][0]][history[0][1]] == robot[2]) board[history[0][0]][history[0][1]] = '.';
      int nowY = history[0][0], nowX = history[0][1], dir = history[0][2];
      int endY = history[history.size()-1][0], endX = history[history.size()-1][1];
      while (true) {
        if (board[nowY][nowX] == '#') break;
        if (board[nowY][nowX] == 'G') break;
        if (nowY == endY && nowX == endX) break;
        if (board[nowY][nowX] != '.') dir = codeOfDir(board[nowY][nowX]);
        allPassed[nowY][nowX] = true;
        int nextX = nowX, nextY = nowY;
        switch (dir) {
          case 0: // U
            nextY--;
            if (nextY < 0) nextY = N-1;
            break;
          case 1: // D
            nextY++;
            if (nextY >= N) nextY = 0;
            break;
          case 2: // L
            nextX--;
            if (nextX < 0) nextX = N-1;
            break;
          case 3: // R
            nextX++;
            if (nextX >= N) nextX = 0;
            break;
        }
        nowX = nextX, nowY = nextY;
      }
      updated++;
    }
  }
  return updated;
}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int _n, _m, _b;
  cin >> _n >> _m >> _b;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      board[i][j] = '.';
    }
  }
  cin >> finishY >> finishX;
  board[finishY][finishX] = 'G';
  for (int i = 0; i < M; i++) {
    cin >> initRobotY[i] >> initRobotX[i] >> initRobotDirection[i];
  }
  for (int i = 0; i < B; i++) {
    int _by, _bx;
    cin >> _by >> _bx;
    board[_by][_bx] = '#';
  }

  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        passed[k][i][j] = false;
      }
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      bestBoard[i][j] = board[i][j];
    }
  }

  for (int d = 0; d < 4; d++) {
    for (int i = 0; i < N; i++) {
      passedLL[d][i] = 0ULL;
    }
  }

  theRandom.x = 1234567890ULL;

  int score = calcScore();
  int lastScore = score;
  int bestScore = score;

  int iterCount = 0;
  int nextCheck = 0;
  double temperature = 5e0;
  double decayRate = 6e-6;

  while (true) {
    if (iterCount >= nextCheck) {
      timeElapsed = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - timeBegin);
      if (timeElapsed > timeLimit) break;
      nextCheck += (timeLimit - timeElapsed).count() >> 5;
    }
    if (theRandom.nextDouble() < 0.9) { // single
      int ry = theRandom.nextIntMod(N);
      int rx = theRandom.nextIntMod(N);
      int rd = theRandom.nextIntMod(5);
      if (board[ry][rx] == '#' || board[ry][rx] == 'G' || board[ry][rx] == direction[rd]) continue;
      if (iterCount % 10000 == 0) {
        int updated;
        for (int i = 0; i < 10; i++) {
          updated = moveStuckRobot();
          if (updated == 0) break;
        }
        score = calcScoreFaster();
        lastScore = score;
      }
      char lastDirection = board[ry][rx];
      board[ry][rx] = direction[rd];

      score = calcScoreFaster();

      #ifdef DEBUG2
      if (iterCount % 5000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << temperature << " " << exp((score - lastScore) / temperature) << endl;
      #endif

      if (score > lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
              bestBoard[i][j] = board[i][j];
            }
          }
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) {
        lastScore = score;
      }
      else {
        board[ry][rx] = lastDirection;
      }
    }
    else { // double
      int ry1 = theRandom.nextIntMod(N);
      int rx1 = theRandom.nextIntMod(N);
      int rd1 = theRandom.nextIntMod(5);
      if (board[ry1][rx1] == '#' || board[ry1][rx1] == 'G' || board[ry1][rx1] == direction[rd1]) continue;
      int ry2 = theRandom.nextIntMod(N);
      int rx2 = theRandom.nextIntMod(N);
      int rd2 = theRandom.nextIntMod(5);
      if (board[ry2][rx2] == '#' || board[ry2][rx2] == 'G' || board[ry2][rx2] == direction[rd2]) continue;
      if (iterCount % 10000 == 0) {
        int updated;
        for (int i = 0; i < 10; i++) {
          updated = moveStuckRobot();
          if (updated == 0) break;
        }
        score = calcScoreFaster();
        lastScore = score;
      }
      char lastDirection1 = board[ry1][rx1];
      char lastDirection2 = board[ry2][rx2];
      board[ry1][rx1] = direction[rd1];
      board[ry2][rx2] = direction[rd2];

      score = calcScoreFaster();

      #ifdef DEBUG2
      if (iterCount % 1000 == 0) cerr << iterCount << " " << score << " " << lastScore << " " << temperature << " " << exp((score - lastScore) / temperature) << endl;
      #endif

      if (score > lastScore) {
        lastScore = score;
        if (score > bestScore) {
          bestScore = score;
          for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
              bestBoard[i][j] = board[i][j];
            }
          }
        }
      }
      else if (theRandom.nextDouble() < exp(double(score - lastScore) / temperature)) {
        lastScore = score;
      }
      else {
        board[ry1][rx1] = lastDirection1;
        board[ry2][rx2] = lastDirection2;
      }
    }

    iterCount++;
    temperature *= 1.0 - decayRate;
  }

  #ifdef DEBUG1
    cerr << "iterCount   = " << iterCount << endl;
    cerr << "temperature = " << temperature << endl;
    cerr << "bestScore   = " << bestScore << endl;
  #endif

  vector<short> signY, signX;
  vector<char> signD;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (!(bestBoard[i][j] == '.' || bestBoard[i][j] == '#' || bestBoard[i][j] == 'G')) {
        signY.push_back(i);
        signX.push_back(j);
        signD.push_back(bestBoard[i][j]);
      }
    }
  }

  cout << signD.size() << endl;
  for (int i = 0; i < signD.size(); i++) {
    cout << signY[i] << " " << signX[i] << " " << signD[i] << endl;
  }

  return 0;
}
