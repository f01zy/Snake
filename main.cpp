#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

const float SPEED = 0.2f;

const int COLS = 30;
const int ROWS = 30;
char map[ROWS][COLS];

enum class SIDE { LEFT, RIGHT, TOP, BOTTOM };
SIDE direction = SIDE::RIGHT;
std::vector<int> head{6, 6};
std::vector<int> tail{0, 0};

void sleep(float seconds) {
  int milliseconds = static_cast<int>(seconds * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void clearConsole() { system("clear"); }

void clearMap() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      map[i][j] = '.';
    }
  }
}

void renderMap() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      std::cout << map[i][j];
    }
    std::cout << std::endl;
  }
}

void prepareMap() {
  int x =
      direction == SIDE::LEFT || direction == SIDE::RIGHT ? tail[1] : head[1];
  int y =
      direction == SIDE::LEFT || direction == SIDE::RIGHT ? head[0] : tail[0];

  // Prepare x direction
  int xLength = head[1] - tail[1];
  int xMax = xLength < 0 ? tail[1] : head[1];
  int xMin = xLength < 0 ? head[1] : tail[1];

  for (int i = xMin; i < xMax; i++) {
    map[y][i] = '#';
  }

  // Prepare y direction
  int yLength = head[0] - tail[0];
  int yMax = yLength < 0 ? tail[0] : head[0];
  int yMin = yLength < 0 ? head[0] : tail[0];

  for (int i = yMin; i < yMax; i++) {
    map[i][x] = '#';
  }
}

void lose() {
  clearConsole();
  std::cout << "You are lose." << std::endl;
  exit(0);
}

bool checkMoveAvailability() {
  switch (direction) {
  case SIDE::LEFT:
    if (head[1] == 0) {
      return 0;
    }
    break;
  case SIDE::RIGHT:
    if (head[1] == COLS) {
      return 0;
    }
    break;
  case SIDE::TOP:
    if (head[0] == 0) {
      return 0;
    }
    break;
  case SIDE::BOTTOM:
    if (head[0] == ROWS) {
      return 0;
    }
    break;
  }

  return 1;
}

void move() {
  if (!checkMoveAvailability()) {
    lose();
  }

  switch (direction) {
  case SIDE::LEFT:
    head[1]--;
    if (tail[0] != head[0])
      head[0] - tail[0] < 0 ? tail[0]-- : tail[0]++;
    else
      tail[1]--;
    break;
  case SIDE::RIGHT:
    head[1]++;
    if (tail[0] != head[0])
      head[0] - tail[0] < 0 ? tail[0]-- : tail[0]++;
    else
      tail[1]++;
    break;
  case SIDE::TOP:
    head[0]--;
    if (tail[1] != head[1])
      head[1] - tail[1] < 0 ? tail[1]-- : tail[1]++;
    else
      tail[0]--;
    break;
  case SIDE::BOTTOM:
    head[0]++;
    if (tail[1] != head[1])
      head[1] - tail[1] < 0 ? tail[1]-- : tail[1]++;
    else
      tail[0]++;
    break;
  }
}

int main() {
  while (1) {
    clearConsole();

    clearMap();
    prepareMap();
    move();
    renderMap();

    sleep(SPEED);
  }
}
