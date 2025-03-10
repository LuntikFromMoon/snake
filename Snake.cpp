// Snake.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>// rand()
#include <conio.h> // kbhit(), getch()
#include <windows.h> // Sleep()

const int FIELD_ROWS = 8;
const int FIELD_COLS = 8;
const char EMPTY_CELL = '-';
const char APPLE_CHAR = 'A';
const char SNAKE_HEAD_CHAR = '@';
const char SNAKE_BODY_CHAR = 'o';
const char UP_DIRECTION = 'w';
const char LEFT_DIRECTION = 'a';
const char DOWN_DIRECTION = 's';
const char RIGHT_DIRECTION = 'd';
const char EXIT_BUTTON = 'q';
const int SNAKE_SPEED = 400;
const char GAME_OVER_MESSAGE[] = "Game Over!";
const char EXIT_MESSAGE[] = "You exited the game.";
const char WIN_MESSAGE[] = "You win!";

struct GameField {
    std::vector<std::vector<char>> grid;

    GameField(int rows, int cols, char cell)
        : grid(rows, std::vector<char>(cols, cell)) {}
};

struct Apple {
    std::pair<int, int> position;

    Apple(int x, int y) : position(x, y) {}
};

struct Snake {
    std::deque<std::pair<int, int>> body;
};

GameField InitializeField() {
    return GameField(FIELD_ROWS, FIELD_COLS, EMPTY_CELL);
}

Snake InitializeSnake() {
    Snake snake;
    int start_x = FIELD_ROWS / 2;
    int start_y = FIELD_COLS / 2;
    snake.body.push_back({ start_x, start_y });
    snake.body.push_back({ start_x, start_y - 1 });
    return snake;
}

bool IsPositionInSnake(const Snake& snake, const std::pair<int, int>& position) {
    for (size_t i = 0; i < snake.body.size() - 1; ++i) {
        if (snake.body[i] == position) {
            return true;
        }
    }
    return false;
}

Apple GenerateApple(const GameField& field, const Snake& snake) {
    int row, col;
    do {
        row = std::rand() % FIELD_ROWS;
        col = std::rand() % FIELD_COLS;
    } while (field.grid[row][col] != EMPTY_CELL || IsPositionInSnake(snake, { row, col }));
    return Apple(row, col);
}

GameField UpdateField(const GameField& field, const Apple& apple, const Snake& snake) {
    GameField new_field = InitializeField();
    for (size_t i = 0; i < snake.body.size(); ++i) {
        const auto& segment = snake.body[i];
        new_field.grid[segment.first][segment.second] =
            (i == 0) ? SNAKE_HEAD_CHAR : SNAKE_BODY_CHAR;
    }
    new_field.grid[apple.position.first][apple.position.second] = APPLE_CHAR;
    return new_field;
}

std::string PrintField(const GameField& field) {
    std::string output;
    for (const auto& row : field.grid) {
        for (const auto& cell : row) {
            output += cell;
            output += ' ';
        }
        output += '\n';
    }
    return output;
}

bool IsPositionInWall(const std::pair<int, int>& position) {
    if (position.first < 0 || position.first >= FIELD_ROWS ||
        position.second < 0 || position.second >= FIELD_COLS) {
        return true;
    }
    return false;
}

bool IsCollision(const Snake& snake, const std::pair<int, int>& next_position) {
    return IsPositionInSnake(snake, next_position) || IsPositionInWall(next_position);
}

bool MoveSnake(Snake& snake, const std::pair<int, int>& direction, Apple& apple, bool& gameOver) {
    std::pair<int, int> next_position = {
        snake.body.front().first + direction.first,
        snake.body.front().second + direction.second };

    if (IsCollision(snake, next_position)) {
        gameOver = true;
        return false;
    }

    snake.body.push_front(next_position);

    if (next_position == apple.position) {
        apple = GenerateApple(InitializeField(), snake);
    }
    else {
        snake.body.pop_back();
    }

    return true;
}

std::pair<int, int> FilterDirection(char& input, std::pair<int, int>& prevDirection) {
    if (input == UP_DIRECTION && prevDirection != std::make_pair(1, 0)) return { -1, 0 };
    if (input == DOWN_DIRECTION && prevDirection != std::make_pair(-1, 0)) return { 1, 0 };
    if (input == LEFT_DIRECTION && prevDirection != std::make_pair(0, 1)) return { 0, -1 };
    if (input == RIGHT_DIRECTION && prevDirection != std::make_pair(0, -1)) return { 0, 1 };
    return prevDirection;
}

bool IsWin(const Snake& snake) {
    return snake.body.size() == (FIELD_ROWS * FIELD_COLS);
}

int main() {
    GameField field = InitializeField();
    Snake snake = InitializeSnake();
    Apple apple = GenerateApple(field, snake);

    field = UpdateField(field, apple, snake);
    std::pair<int, int> direction = { 0, 1 };
    std::pair<int, int> prevDirection;
    bool gameOver = false;

    while (!gameOver) {
        if (IsWin(snake)) {
            break;
        }

        system("cls");
        std::cout << PrintField(field) << std::endl;

        Sleep(SNAKE_SPEED);

        if (_kbhit()) {
            prevDirection = direction;
            char input = _getch();
            if (input == EXIT_BUTTON) {
                break;
            }
            direction = FilterDirection(input, prevDirection);
        }
        if (!MoveSnake(snake, direction, apple, gameOver)) {
            break;
        }
        field = UpdateField(field, apple, snake);
    }

    system("cls");
    if (gameOver) {
        std::cout << GAME_OVER_MESSAGE << std::endl;
    }
    else if (!IsWin(snake)) {
        std::cout << EXIT_MESSAGE << std::endl;
    }
    else {
        std::cout << WIN_MESSAGE << std::endl;
    }
    return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
