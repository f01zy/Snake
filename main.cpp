#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

const int COLS = 30;
const int ROWS = 30;
char map[ROWS][COLS];

std::vector<int> head{6, 6};
std::vector<int> tail{0, 0};
std::vector<std::vector<int>> nodes = {{0, 6}};

enum class SIDE { LEFT, RIGHT, TOP, BOTTOM };
SIDE direction = SIDE::BOTTOM;

const float SPEED = 0.2f;

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

void prepareLine(std::vector<int> a, std::vector<int> b) {
  int xLength = a[1] - b[1];
  int yLength = a[0] - b[0];

  if (xLength) {
    int xMax = xLength < 0 ? b[1] : a[1];
    int xMin = xLength < 0 ? a[1] : b[1];

    for (int x = xMin; x <= xMax; x++) {
      map[a[0]][x] = '#';
    }
  }

  else if (yLength) {
    int yMax = yLength < 0 ? b[0] : a[0];
    int yMin = yLength < 0 ? a[0] : b[0];

    for (int y = yMin; y <= yMax; y++) {
      map[y][a[1]] = '#';
    }
  }
}

void prepareMap() {
  std::vector<std::vector<int>> prepareNodes;

  prepareNodes.push_back(tail);
  for (std::vector<int> node : nodes) {
    prepareNodes.push_back(node);
  }
  prepareNodes.push_back(head);

  for (int i = 1; i < prepareNodes.size(); i++) {
    prepareLine(prepareNodes[i], prepareNodes[i - 1]);
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

void moveNodeRelativeDirection(std::vector<int> &node, SIDE nodeDirection) {
  switch (nodeDirection) {
  case SIDE::LEFT:
    node[1]--;
    break;
  case SIDE::RIGHT:
    node[1]++;
    break;
  case SIDE::TOP:
    node[0]--;
    break;
  case SIDE::BOTTOM:
    node[0]++;
    break;
  }
}

void move() {
  if (!checkMoveAvailability()) {
    lose();
  }

  if (!nodes.empty() && nodes[0][0] == tail[0] && nodes[0][1] == tail[1]) {
    nodes.erase(nodes.begin());
  }

  if (nodes.empty()) {
    moveNodeRelativeDirection(tail, direction);
  }

  else {
    std::vector<int> node = nodes[0];

    SIDE tailDirection;

    if (node[0] == tail[0] && node[1] > tail[1]) {
      tailDirection = SIDE::RIGHT;
    }

    else if (node[0] == tail[0] && node[1] < tail[1]) {
      tailDirection = SIDE::LEFT;
    }

    else if (node[1] == tail[1] && node[0] > tail[0]) {
      tailDirection = SIDE::BOTTOM;
    }

    else if (node[1] == tail[1] && node[0] < tail[0]) {
      tailDirection = SIDE::TOP;
    }

    moveNodeRelativeDirection(tail, tailDirection);
  }

  moveNodeRelativeDirection(head, direction);
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
