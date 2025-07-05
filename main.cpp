#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

const int BLOCK_SIZE = 15;
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
      map[i][j] = ' ';
    }
  }
}

void renderMap(sf::RenderWindow &window) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (map[i][j] == '#') {
        float x = static_cast<float>(j * BLOCK_SIZE);
        float y = static_cast<float>(i * BLOCK_SIZE);

        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        block.setFillColor(sf::Color::Green);
        block.setPosition({x, y});

        window.draw(block);
      }
    }
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

bool isLose() {
  switch (direction) {
  case SIDE::LEFT:
    if (head[1] == 0) {
      return 1;
    }
    break;
  case SIDE::RIGHT:
    if (head[1] == COLS) {
      return 1;
    }
    break;
  case SIDE::TOP:
    if (head[0] == 0) {
      return 1;
    }
    break;
  case SIDE::BOTTOM:
    if (head[0] == ROWS) {
      return 1;
    }
    break;
  }

  return 0;
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
  if (isLose()) {
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

void inputHandle(sf::RenderWindow &window) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
    window.close();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    if (direction == SIDE::RIGHT)
      return;
    direction = SIDE::LEFT;
    nodes.push_back(head);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    if (direction == SIDE::LEFT)
      return;
    direction = SIDE::RIGHT;
    nodes.push_back(head);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
    if (direction == SIDE::BOTTOM)
      return;
    direction = SIDE::TOP;
    nodes.push_back(head);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    if (direction == SIDE::TOP)
      return;
    direction = SIDE::BOTTOM;
    nodes.push_back(head);
  }
}

int main() {
  const int width = BLOCK_SIZE * COLS;
  const int height = BLOCK_SIZE * ROWS;

  sf::RenderWindow window(sf::VideoMode({width, height}), "Snake",
                          sf::Style::Titlebar | sf::Style::Close);
  window.setFramerateLimit(60);

  sf::Clock clock;
  float moveInterval = 0.2f;
  float timeSinceLastMove = 0.0f;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    float dt = clock.restart().asSeconds();
    timeSinceLastMove += dt;

    if (timeSinceLastMove >= moveInterval) {
      clearMap();
      prepareMap();
      move();
      inputHandle(window);
      timeSinceLastMove = 0.0f;
    }

    window.clear();
    renderMap(window);
    window.display();
  }
}
