// Math Quest - a standalone C++17 console game.
// Compile: g++ -std=c++17 -O2 -Wall -Wextra -pedantic math_game.cpp -o math_game
// Run on Windows: math_game.exe    Run on macOS/Linux: ./math_game
// Use a modern terminal for the dashboard, or pass --plain for simple output.
// MSVC: cl /std:c++17 /EHsc /utf-8 math_game.cpp
#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <io.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

struct Question {
    std::string text;
    int answer;
    std::string explanation;
};

constexpr int questionsPerRound = 10;
constexpr int wordProblemTypes = 10;

int randomInt(std::mt19937& rng, int low, int high) {
    return std::uniform_int_distribution<int>(low, high)(rng);
}

std::string number(int value) {
    return std::to_string(value);
}

// Read a whole line so invalid text cannot leave the input stream stuck.
// Return false when the player types Q or closes the input stream.
bool readNumber(const std::string& prompt, double& value) {
    for (;;) {
        std::cout << prompt << std::flush;
        std::string line;
        if (!std::getline(std::cin, line)) {
            return false;
        }
        std::string trimmed;
        std::istringstream words(line);
        words >> trimmed >> std::ws;
        if (words.eof() && (trimmed == "q" || trimmed == "Q")) {
            return false;
        }

        std::istringstream input(line);
        double candidate;
        if (input >> candidate) {
            input >> std::ws;
            if (input.eof() && std::isfinite(candidate)) {
                value = candidate;
                return true;
            }
        }
        std::cout << "Please enter a number only, or Q to quit.\n";
    }
}

Question makeArithmeticQuestion(int difficulty, std::mt19937& rng) {
    int a = randomInt(rng, 1, 20);
    int b = randomInt(rng, 1, 20);
    std::string expression;
    int answer = 0;

    if (difficulty == 1) {
        if (randomInt(rng, 0, 1) == 0) {
            expression = number(a) + " + " + number(b);
            answer = a + b;
        } else {
            if (a < b) {
                std::swap(a, b);
            }
            expression = number(a) + " - " + number(b);
            answer = a - b;
        }
    } else {
        a = randomInt(rng, 3, 12);
        b = randomInt(rng, 3, 12);
        const int c = randomInt(rng, 2, 15);
        switch (randomInt(rng, 0, 3)) {
            case 0:
                expression = number(a) + " * " + number(b);
                answer = a * b;
                break;
            case 1:
                // Build the dividend from the answer for exact division.
                expression = number(a * b) + " / " + number(b);
                answer = a;
                break;
            case 2:
                expression = number(a) + " + " + number(b) + " * " + number(c);
                answer = a + b * c;
                break;
            default:
                expression = "(" + number(a) + " + " + number(b) + ") * " + number(c);
                answer = (a + b) * c;
                break;
        }
    }
    return {expression + " = ?", answer, expression + " = " + number(answer)};
}

// Randomize both the story details and the numbers. Every scenario has a
// whole-number answer and explains how to calculate it.
Question makeWordProblem(int type, std::mt19937& rng) {
    static const std::array<std::string, 8> names = {
        "Alex", "Sam", "Mia", "Leo", "Jamie", "Nina", "Kai", "Ava"
    };
    const std::string name = names[randomInt(rng, 0, 7)];

    switch (type) {
        case 0: {
            const int notebooks = randomInt(rng, 3, 8);
            const int notebookPrice = randomInt(rng, 15, 60);
            const int pens = randomInt(rng, 2, 6);
            const int penPrice = randomInt(rng, 5, 20);
            const int total = notebooks * notebookPrice + pens * penPrice;
            const int paid = ((total / 100) + 1) * 100;
            return {
                name + " buys " + number(notebooks) + " notebooks at " + number(notebookPrice)
                    + " coins each and " + number(pens) + " pens at " + number(penPrice)
                    + " coins each. If " + name + " pays " + number(paid)
                    + " coins, how many coins of change should they receive?",
                paid - total,
                number(paid) + " - (" + number(notebooks) + " * " + number(notebookPrice)
                    + " + " + number(pens) + " * " + number(penPrice) + ") = " + number(paid - total)
            };
        }
        case 1: {
            const int price = 20 * randomInt(rng, 10, 80);
            const int discount = 5 * randomInt(rng, 1, 7);
            const int answer = price * (100 - discount) / 100;
            return {
                "A backpack costs " + number(price) + " coins. A store offers a "
                    + number(discount) + "% discount. What is the sale price in coins?",
                answer,
                number(price) + " * (100 - " + number(discount) + ") / 100 = " + number(answer)
            };
        }
        case 2: {
            const int speed = randomInt(rng, 40, 90);
            const int hours = randomInt(rng, 2, 5);
            const int remaining = randomInt(rng, 30, 150);
            const int distance = speed * hours + remaining;
            return {
                "A bus must travel " + number(distance) + " km. It travels at a constant "
                    + number(speed) + " km/h for " + number(hours)
                    + " hours. How many kilometers remain?",
                remaining,
                number(distance) + " - " + number(speed) + " * " + number(hours)
                    + " = " + number(remaining)
            };
        }
        case 3: {
            const int length = randomInt(rng, 12, 35);
            const int width = randomInt(rng, 5, 11);
            const int gate = randomInt(rng, 1, 4);
            const int answer = 2 * (length + width) - gate;
            return {
                "A rectangular garden is " + number(length) + " meters long and "
                    + number(width) + " meters wide. A " + number(gate)
                    + "-meter gate needs no fencing. How many meters of fencing are needed?",
                answer,
                "2 * (" + number(length) + " + " + number(width) + ") - "
                    + number(gate) + " = " + number(answer)
            };
        }
        case 4: {
            const int students = randomInt(rng, 2, 8);
            const int boxes = randomInt(rng, 3, 7);
            const int perBox = students * randomInt(rng, 3, 8);
            const int donated = students * randomInt(rng, 1, 4);
            const int answer = (boxes * perBox - donated) / students;
            return {
                "A club buys " + number(boxes) + " boxes containing " + number(perBox)
                    + " pencils each. It donates " + number(donated)
                    + " pencils, then shares the rest equally among " + number(students)
                    + " students. How many pencils does each student get?",
                answer,
                "(" + number(boxes) + " * " + number(perBox) + " - " + number(donated)
                    + ") / " + number(students) + " = " + number(answer)
            };
        }
        case 5: {
            const int adults = randomInt(rng, 2, 8);
            const int adultPrice = randomInt(rng, 80, 150);
            const int children = randomInt(rng, 2, 9);
            const int childPrice = randomInt(rng, 40, 75);
            const int answer = adults * adultPrice + children * childPrice;
            return {
                "A group buys " + number(adults) + " adult tickets at " + number(adultPrice)
                    + " coins each and " + number(children) + " child tickets at "
                    + number(childPrice) + " coins each. How many coins do they pay in total?",
                answer,
                number(adults) + " * " + number(adultPrice) + " + " + number(children)
                    + " * " + number(childPrice) + " = " + number(answer)
            };
        }
        case 6: {
            const int initial = randomInt(rng, 20, 90);
            const int rate = randomInt(rng, 3, 12);
            const int minutes = randomInt(rng, 5, 20);
            const int drained = randomInt(rng, 10, initial);
            const int answer = initial + rate * minutes - drained;
            return {
                "A tank starts with " + number(initial) + " liters of water. A tap adds "
                    + number(rate) + " liters per minute for " + number(minutes)
                    + " minutes. Then " + number(drained)
                    + " liters are drained. Assuming no overflow, how many liters remain?",
                answer,
                number(initial) + " + " + number(rate) + " * " + number(minutes)
                    + " - " + number(drained) + " = " + number(answer)
            };
        }
        case 7: {
            const int target = randomInt(rng, 70, 95);
            const int first = target - 8;
            const int second = target + 4;
            const int third = target + 1;
            const int answer = 4 * target - first - second - third;
            return {
                name + " scores " + number(first) + ", " + number(second) + ", and "
                    + number(third) + " on three tests. What score is needed on a fourth test"
                    + " to have an average of " + number(target) + " across all four tests?",
                answer,
                "4 * " + number(target) + " - (" + number(first) + " + " + number(second)
                    + " + " + number(third) + ") = " + number(answer)
            };
        }
        case 8: {
            const int total = 12 * randomInt(rng, 4, 16);
            const int answer = total - total / 3 - total / 4;
            return {
                "A bakery makes " + number(total)
                    + " rolls. It sells one third of the ORIGINAL batch in the morning and"
                    + " one quarter of the ORIGINAL batch in the afternoon. How many rolls remain?",
                answer,
                number(total) + " - " + number(total) + " / 3 - " + number(total)
                    + " / 4 = " + number(answer)
            };
        }
        default: {
            const int hours = randomInt(rng, 3, 8);
            const int days = randomInt(rng, 2, 5);
            const int rate = 10 * randomInt(rng, 6, 15);
            const int earnings = hours * days * rate;
            const int spent = (earnings / 10) * randomInt(rng, 1, 8);
            const int answer = earnings - spent;
            return {
                name + " works " + number(hours) + " hours per day for " + number(days)
                    + " days and earns " + number(rate) + " coins per hour. After spending "
                    + number(spent) + " coins, how many of those earned coins remain?",
                answer,
                number(hours) + " * " + number(days) + " * " + number(rate) + " - "
                    + number(spent) + " = " + number(answer)
            };
        }
    }
}

void showSummary(int correct, int answered, int points) {
    std::cout << "\n----- ROUND RESULTS -----\n"
              << "Correct: " << correct << " / " << answered << " answered\n"
              << "Score: " << points << " points\n";
    if (answered == questionsPerRound) {
        if (correct == questionsPerRound) {
            std::cout << "Perfect! You are a Math Quest champion!\n";
        } else if (correct >= 7) {
            std::cout << "Great work! Keep it up.\n";
        } else {
            std::cout << "Keep practicing. Every question helps you improve!\n";
        }
    }
}

bool playRound(int difficulty, std::mt19937& rng) {
    const std::array<std::string, 3> labels = {"Easy", "Medium", "Hard"};
    const int pointsPerCorrect = 10 * difficulty;
    std::array<int, wordProblemTypes> stories{};
    std::iota(stories.begin(), stories.end(), 0);
    std::shuffle(stories.begin(), stories.end(), rng);

    int correct = 0;
    int points = 0;
    std::cout << "\n" << labels[difficulty - 1] << " mode: " << questionsPerRound
              << " questions, " << pointsPerCorrect << " points per correct answer.\n"
              << "All answers are whole numbers. Enter numbers only, or Q to quit.\n"
              << "For arithmetic, do parentheses first, then * and /, then + and -.\n";

    for (int i = 0; i < questionsPerRound; ++i) {
        const Question question = difficulty == 3
            ? makeWordProblem(stories[i % wordProblemTypes], rng)
            : makeArithmeticQuestion(difficulty, rng);

        std::cout << "\nQuestion " << i + 1 << " / " << questionsPerRound << "\n"
                  << question.text << "\n";
        double answer = 0;
        if (!readNumber("Your answer: ", answer)) {
            showSummary(correct, i, points);
            return false;
        }
        if (std::fabs(answer - question.answer) < 0.000001) {
            ++correct;
            points += pointsPerCorrect;
            std::cout << "Correct! +" << pointsPerCorrect << " points\n";
        } else {
            std::cout << "Incorrect. The correct answer is " << question.answer << ".\n";
        }
        std::cout << "Solution: " << question.explanation << "\n"
                  << "Score: " << points << " points\n";
    }
    showSummary(correct, questionsPerRound, points);
    return true;
}

int plainGame() {
    std::mt19937 rng(std::random_device{}());
    std::cout << "========================================\n"
              << "             MATH QUEST\n"
              << "========================================\n";

    for (;;) {
        std::cout << "\nChoose your difficulty:\n"
                  << "1. Easy   - addition and subtraction\n"
                  << "2. Medium - multiplication, division, and mixed operations\n"
                  << "3. Hard   - randomized word problems\n"
                  << "0. Exit\n";
        double choice = 0;
        if (!readNumber("Choose 1, 2, or 3 (0 or Q to quit): ", choice) || choice == 0) {
            break;
        }
        if (choice < 1 || choice > 3 || std::floor(choice) != choice) {
            std::cout << "Please select 1, 2, 3, or 0.\n";
            continue;
        }
        if (!playRound(static_cast<int>(choice), rng)) {
            break;
        }
    }
    std::cout << "\nThanks for playing Math Quest!\n";
    return 0;
}

// The rich interface is deliberately separate from the question generators.
// Redirected output and older consoles retain the simple, accessible interface.
namespace dashboard {

enum class Tone { Text, Muted, Accent, Success, Warning, Danger, Border };
struct Span { std::string text; Tone tone = Tone::Text; };
using Line = std::vector<Span>;
using Screen = std::vector<Line>;

int widthOf(const std::string& text) {
    int width = 0;
    for (unsigned char ch : text) {
        if ((ch & 0xC0) != 0x80) ++width;
    }
    return width;
}

std::string clipped(const std::string& text, int width) {
    int count = 0;
    std::size_t end = 0;
    while (end < text.size()) {
        const unsigned char ch = static_cast<unsigned char>(text[end]);
        if ((ch & 0xC0) != 0x80 && count++ >= width) break;
        ++end;
    }
    return text.substr(0, end);
}

std::string repeated(const std::string& text, int count) {
    std::string result;
    for (int i = 0; i < count; ++i) result += text;
    return result;
}

int widthOf(const Line& line) {
    int width = 0;
    for (const auto& span : line) width += widthOf(span.text);
    return width;
}

Line fitted(const Line& line, int width) {
    Line result;
    int remaining = width;
    for (const auto& span : line) {
        if (remaining <= 0) break;
        const std::string text = clipped(span.text, remaining);
        result.push_back({text, span.tone});
        remaining -= widthOf(text);
    }
    if (remaining > 0) result.push_back({std::string(static_cast<std::size_t>(remaining), ' ')});
    return result;
}

Line text(const std::string& value, Tone tone = Tone::Text) { return {{value, tone}}; }

Line aligned(const std::string& left, const std::string& right, int width) {
    const int gap = width - widthOf(left) - widthOf(right);
    if (gap < 1) return fitted(text(left + "  " + right), width);
    return {{left, Tone::Text}, {std::string(static_cast<std::size_t>(gap), ' ')}, {right, Tone::Accent}};
}

Screen wrapped(const std::string& value, int width, Tone tone = Tone::Text) {
    Screen lines;
    std::istringstream input(value);
    std::string word, current;
    while (input >> word) {
        if (!current.empty() && widthOf(current) + 1 + widthOf(word) > width) {
            lines.push_back(text(current, tone));
            current.clear();
        }
        while (widthOf(word) > width) {
            if (!current.empty()) {
                lines.push_back(text(current, tone));
                current.clear();
            }
            const std::string chunk = clipped(word, width);
            lines.push_back(text(chunk, tone));
            word.erase(0, chunk.size());
        }
        if (!word.empty()) {
            if (!current.empty()) current += ' ';
            current += word;
        }
    }
    if (!current.empty()) lines.push_back(text(current, tone));
    return lines;
}

void append(Screen& target, const Screen& source) {
    target.insert(target.end(), source.begin(), source.end());
}

Screen panel(int width, const std::string& title, Screen body, Tone border = Tone::Border) {
    const std::string label = title.empty() ? "" : " " + clipped(title, width - 8) + " ";
    Screen result = {text("╭─" + label + repeated("─", width - 3 - widthOf(label)) + "╮", border)};
    for (const auto& row : body) {
        Line line = {{"│ ", border}};
        const auto content = fitted(row, width - 4);
        line.insert(line.end(), content.begin(), content.end());
        line.push_back({" │", border});
        result.push_back(line);
    }
    result.push_back(text("╰" + repeated("─", width - 2) + "╯", border));
    return result;
}

Screen beside(const Screen& left, int leftWidth, const Screen& right, int rightWidth, int gap = 2) {
    Screen result;
    const std::size_t count = std::max(left.size(), right.size());
    for (std::size_t i = 0; i < count; ++i) {
        Line line = fitted(i < left.size() ? left[i] : Line{}, leftWidth);
        line.push_back({std::string(static_cast<std::size_t>(gap), ' ')});
        const Line other = fitted(i < right.size() ? right[i] : Line{}, rightWidth);
        line.insert(line.end(), other.begin(), other.end());
        result.push_back(line);
    }
    return result;
}

std::string percentage(int correct, int answered) {
    return answered ? number(static_cast<int>(100.0 * correct / answered + 0.5)) + "%" : "--";
}

std::string trimmed(std::string value) {
    const auto begin = value.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(begin, end - begin + 1);
}

std::string lower(std::string value) {
    for (char& ch : value) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return value;
}

bool numeric(const std::string& input, double& answer) {
    std::istringstream stream(input);
    if (!(stream >> answer) || !std::isfinite(answer)) return false;
    stream >> std::ws;
    return stream.eof();
}

std::string displayNumber(double value) {
    std::ostringstream out;
    out << std::setprecision(10) << value;
    return out.str();
}

class Terminal {
    bool active = false;
#ifdef _WIN32
    DWORD oldMode = 0;
    UINT oldCodepage = 0;
    HANDLE output = INVALID_HANDLE_VALUE;
#endif
public:
    int width = 104;
    int height = 30;
    int theme = 0;
    bool available = false;

    Terminal() {
#ifdef _WIN32
        output = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO info{};
        available = _isatty(_fileno(stdin)) && GetConsoleMode(output, &oldMode);
        if (available) {
            available = SetConsoleMode(output, oldMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
            if (GetConsoleScreenBufferInfo(output, &info)) {
                width = info.srWindow.Right - info.srWindow.Left + 1;
                height = info.srWindow.Bottom - info.srWindow.Top + 1;
            }
        }
#else
        available = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
        const char* term = std::getenv("TERM");
        if (!term || std::string(term) == "dumb") available = false;
        winsize size{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0) {
            if (size.ws_col) width = size.ws_col;
            if (size.ws_row) height = size.ws_row;
        }
#endif
        width = std::min(112, width - 4);
        if (width < 76 || height < 24) available = false;
    }

    void begin() {
#ifdef _WIN32
        oldCodepage = GetConsoleOutputCP();
        SetConsoleOutputCP(CP_UTF8);
#endif
        active = true;
        std::cout << "\033[?1049h";
    }

    ~Terminal() {
        if (active) std::cout << "\033[0m\033[?25h\033[?1049l" << std::flush;
#ifdef _WIN32
        if (output != INVALID_HANDLE_VALUE && oldMode) SetConsoleMode(output, oldMode);
        if (oldCodepage) SetConsoleOutputCP(oldCodepage);
#endif
    }

    std::string color(Tone tone) const {
        switch (tone) {
            case Tone::Text: return "\033[38;2;230;238;249m";
            case Tone::Muted: return "\033[38;2;141;158;184m";
            case Tone::Accent:
                return theme == 0 ? "\033[38;2;78;218;233m" :
                       theme == 1 ? "\033[38;2;112;231;166m" : "\033[38;2;192;163;255m";
            case Tone::Success: return "\033[38;2;112;231;166m";
            case Tone::Warning: return "\033[38;2;255;205;114m";
            case Tone::Danger: return "\033[38;2;255;134;151m";
            case Tone::Border: return "\033[38;2;65;86;116m";
        }
        return "";
    }

    std::string themeName() const {
        return theme == 0 ? "MIDNIGHT" : theme == 1 ? "FOREST" : "AMETHYST";
    }

    void draw(const Screen& screen) const {
        std::cout << "\033[?25l\033[48;2;11;18;32m\033[2J\033[H";
        for (const auto& row : screen) {
            std::cout << "  ";
            for (const auto& span : fitted(row, width)) std::cout << color(span.tone) << span.text;
            std::cout << "\033[K\n";
        }
        std::cout << color(Tone::Accent) << "\n  > " << color(Tone::Text) << "\033[?25h" << std::flush;
    }

    bool read(std::string& input) const {
        if (!std::getline(std::cin, input)) return false;
        input = trimmed(input);
        return true;
    }
};

struct Attempt { Question question; double answer; bool correct; };
struct Round {
    int difficulty = 1;
    std::vector<Attempt> attempts;
    int bestStreak = 0;
    long long seconds = 0;
    int correct() const {
        return static_cast<int>(std::count_if(attempts.begin(), attempts.end(),
            [](const Attempt& attempt) { return attempt.correct; }));
    }
    int points() const { return correct() * difficulty * 10; }
    int answered() const { return static_cast<int>(attempts.size()); }
    bool complete() const { return answered() == questionsPerRound; }
};

const std::array<std::string, 3> levels = {"EASY", "MEDIUM", "HARD"};
const std::array<Tone, 3> levelTones = {Tone::Success, Tone::Warning, Tone::Danger};
const std::array<std::string, wordProblemTypes> topics = {
    "SHOPPING & CHANGE", "PERCENTAGE DISCOUNTS", "SPEED & DISTANCE", "PERIMETER & FENCING",
    "EQUAL SHARING", "TICKET TOTALS", "VOLUME & FLOW", "TEST AVERAGES", "FRACTIONS", "EARNINGS & SAVINGS"
};

class Game {
    Terminal& terminal;
    std::mt19937 rng{std::random_device{}()};
    std::vector<Round> history;
    bool running = true;

    Screen header(const std::string& page, bool subtitle = false) const {
        Screen body = {aligned("M A T H   Q U E S T", page, terminal.width - 4)};
        if (subtitle) body.push_back(text("THE NUMBER ARCADE  /  Think clearly. Solve boldly.", Tone::Muted));
        return panel(terminal.width, "", body, Tone::Accent);
    }

    void footer(Screen& screen, const std::string& controls, const std::string& notice = "") const {
        screen.push_back({});
        append(screen, wrapped(controls, terminal.width, Tone::Muted));
        if (!notice.empty()) append(screen, wrapped(notice, terminal.width, Tone::Warning));
    }

    void waitForMenu(const Screen& content) {
        terminal.draw(content);
        std::string command;
        if (!terminal.read(command) || lower(command) == "q") running = false;
    }

    void help() {
        Screen screen = header("FIELD GUIDE");
        screen.push_back({});
        append(screen, panel(terminal.width, "HOW TO PLAY", {
            text("01  Choose Easy, Medium, or Hard from the mission deck.", Tone::Accent),
            text("02  Solve ten questions. Submit each answer with Enter."),
            text("03  Review the solution, then press Enter to continue."),
            text("04  Check your round report or review every answer."), text(""),
            text("Easy: +10 points. Medium: +20 points. Hard: +30 points."),
            text("Every answer is a whole number. Numbers only; omit units."),
            text("Order: parentheses first, then * and /, then + and -."),
            text("Hard mode shuffles ten stories with fresh numbers."), text(""),
            text("During a question: H help | T theme | B end round | Q quit", Tone::Muted),
            text("Answered questions stay in this session's log.", Tone::Muted),
            text("Session statistics reset when you close the game.", Tone::Muted)
        }));
        footer(screen, "[Enter] Return     [Q] Quit");
        waitForMenu(screen);
    }

    void statistics() {
        Screen screen = header("SESSION LOG");
        screen.push_back({});
        Screen rows;
        rows.push_back(text("ROUND   DIFFICULTY   CORRECT   POINTS   STATUS", Tone::Accent));
        if (history.empty()) {
            rows.push_back(text("Your story starts here. Complete a round to fill this log.", Tone::Muted));
        } else {
            const std::size_t first = history.size() > 8 ? history.size() - 8 : 0;
            for (std::size_t i = first; i < history.size(); ++i) {
                const auto& round = history[i];
                std::ostringstream row;
                row << std::left << std::setw(8) << i + 1 << std::setw(13) << levels[round.difficulty - 1]
                    << std::setw(10) << number(round.correct()) + "/" + number(round.answered())
                    << std::setw(9) << round.points() << (round.complete() ? "COMPLETE" : "ENDED EARLY");
                rows.push_back(text(row.str()));
            }
            rows.push_back(text("Latest eight rounds shown. Totals include every round.", Tone::Muted));
        }
        append(screen, panel(terminal.width, "ROUND HISTORY", rows));
        screen.push_back({});
        Line records;
        for (int difficulty = 1; difficulty <= 3; ++difficulty) {
            int best = 0;
            for (const auto& round : history) if (round.difficulty == difficulty) best = std::max(best, round.points());
            records.push_back({levels[difficulty - 1] + ": " + number(best) + " pts    ", levelTones[difficulty - 1]});
        }
        append(screen, panel(terminal.width, "SESSION RECORDS", {records}));
        footer(screen, "[Enter] Main menu     [Q] Quit");
        waitForMenu(screen);
    }

    int home() {
        std::string notice;
        while (running) {
            Screen screen = header("MISSION CONTROL", true);
            int points = 0, correct = 0, answered = 0, streak = 0;
            for (const auto& round : history) {
                points += round.points(); correct += round.correct(); answered += round.answered();
                streak = std::max(streak, round.bestStreak);
            }
            screen.push_back({});
            append(screen, wrapped("SESSION  " + number(points) + " points    /    ACCURACY  "
                + percentage(correct, answered) + "    /    BEST STREAK  " + number(streak), terminal.width, Tone::Muted));
            screen.push_back({});
            const std::array<std::string, 3> subtitles = {"BUILD YOUR FOUNDATION", "SHARPEN YOUR INSTINCT", "ENTER THE STORY LAB"};
            const std::array<std::string, 3> details = {"Addition + subtraction", "Products / division / mixed", "10 shuffled word problems"};
            const bool wide = terminal.width >= 92;
            const int cardWidth = wide ? (terminal.width - 4) / 3 : terminal.width;
            Screen deck;
            for (int i = 0; i < 3; ++i) {
                Screen body = {text(number(i + 1) + "  /  " + levels[i], levelTones[i])};
                if (wide) {
                    body.push_back(text(subtitles[i], Tone::Muted)); body.push_back(text(""));
                    body.push_back(text(details[i]));
                    body.push_back(text("10 QUESTIONS   +" + number((i + 1) * 10) + " / CORRECT", levelTones[i]));
                    body.push_back(text("Enter " + number(i + 1) + " to launch", Tone::Muted));
                } else {
                    body = {text(details[i], Tone::Muted)};
                }
                const std::string title = wide ? "MISSION 0" + number(i + 1)
                    : number(i + 1) + " / " + levels[i] + "   +" + number((i + 1) * 10) + " POINTS";
                auto card = panel(cardWidth, title, body, levelTones[i]);
                if (wide && i) deck = beside(deck, i * cardWidth + (i - 1) * 2, card, cardWidth);
                else append(deck, card);
            }
            append(screen, deck);
            footer(screen, "[1-3] Play   [S] Stats   [T] Theme   [H] Help   [Q] Quit", notice);
            screen.push_back(text("THEME: " + terminal.themeName() + "    /    Session statistics", Tone::Muted));
            terminal.draw(screen);
            std::string command;
            if (!terminal.read(command)) { running = false; break; }
            command = lower(command);
            notice.clear();
            if (command == "1" || command == "2" || command == "3") return command[0] - '0';
            if (command == "q" || command == "0") running = false;
            else if (command == "t") terminal.theme = (terminal.theme + 1) % 3;
            else if (command == "s") statistics();
            else if (command == "h") help();
            else notice = "Choose 1, 2, or 3 to play, or use a shortcut shown above.";
        }
        return 0;
    }

    Screen questionScreen(const Round& round, const Question& question, int index,
                          int streak, const std::string& topic, const Attempt* feedback,
                          const std::string& notice) const {
        Screen screen = header(levels[round.difficulty - 1] + " / QUESTION " + number(index + 1) + " OF 10");
        append(screen, wrapped("SCORE  " + number(round.points()) + "    ACCURACY  "
            + percentage(round.correct(), round.answered()) + "    STREAK  " + number(streak)
            + "    REWARD  +" + number(round.difficulty * 10), terminal.width, Tone::Accent));
        Line progress = {{"PROGRESS  ", Tone::Muted}};
        for (int i = 0; i < questionsPerRound; ++i) {
            progress.push_back({i < round.answered() ? "━ " : i == index ? "◆ " : "─ ",
                i < round.answered() ? Tone::Success : i == index ? Tone::Accent : Tone::Border});
        }
        progress.push_back({" " + number(round.answered()) + " / 10 complete", Tone::Muted});
        screen.push_back(progress);
        screen.push_back({});
        const bool wide = terminal.width >= 96;
        const int mainWidth = wide ? terminal.width - 30 : terminal.width;
        Screen content = {text(topic, levelTones[round.difficulty - 1]), text("")};
        append(content, wrapped(question.text, mainWidth - 4));
        while (content.size() < (wide ? 7u : 6u)) content.push_back(text(""));
        Screen board = panel(mainWidth, "CHALLENGE", content, Tone::Accent);
        if (wide) {
            Screen side = {
                text("ROUND TARGET", Tone::Accent), text("10 questions. No timer."), text(""),
                text("CURRENT RUN", Tone::Muted),
                text(number(round.correct()) + " correct / " + number(round.answered()) + " answered"),
                text("Best streak: " + number(round.bestStreak)), text("Numbers only; omit units", Tone::Muted)
            };
            while (side.size() < content.size()) side.push_back(text(""));
            board = beside(board, mainWidth, panel(28, "MISSION BRIEF", side), 28);
        }
        append(screen, board);
        if (feedback) {
            screen.push_back({});
            const Tone tone = feedback->correct ? Tone::Success : Tone::Danger;
            Screen message = {text(feedback->correct ? "CORRECT  +" + number(round.difficulty * 10) + " POINTS"
                    : "KEEP GOING  /  Correct answer: " + number(question.answer), tone)};
            append(message, wrapped("Solution: " + question.explanation, terminal.width - 4, Tone::Muted));
            append(screen, panel(terminal.width, "ANSWER CHECK", message, tone));
        }
        // Keep the input inside a 24-row terminal, even on a narrow display.
        append(screen, wrapped(feedback ? "[Enter] Continue   [B] End round   [T] Theme   [Q] Quit"
                                      : "Number + Enter   [H] Help   [T] Theme   [B] Menu   [Q] Quit",
                              terminal.width, Tone::Muted));
        if (!notice.empty()) append(screen, wrapped(notice, terminal.width, Tone::Warning));
        return screen;
    }

    Round play(int difficulty) {
        Round round;
        round.difficulty = difficulty;
        std::array<int, wordProblemTypes> stories{};
        std::iota(stories.begin(), stories.end(), 0);
        std::shuffle(stories.begin(), stories.end(), rng);
        const auto started = std::chrono::steady_clock::now();
        int streak = 0;
        for (int i = 0; i < questionsPerRound && running; ++i) {
            const Question question = difficulty == 3 ? makeWordProblem(stories[i], rng) : makeArithmeticQuestion(difficulty, rng);
            const std::string topic = difficulty == 3 ? topics[stories[i]] : "ARITHMETIC / ORDER OF OPERATIONS";
            std::string notice;
            bool answered = false;
            bool endRound = false;
            while (running && !endRound) {
                const Attempt* feedback = answered ? &round.attempts.back() : nullptr;
                terminal.draw(questionScreen(round, question, i, streak, topic, feedback, notice));
                std::string command;
                if (!terminal.read(command)) { running = false; break; }
                const std::string shortcut = lower(command);
                notice.clear();
                if (shortcut == "q") { running = false; break; }
                if (shortcut == "b") { endRound = true; break; }
                if (shortcut == "t") { terminal.theme = (terminal.theme + 1) % 3; continue; }
                if (answered) {
                    if (command.empty()) break;
                    notice = "Enter: continue | B: end round | Q: quit";
                    continue;
                }
                if (shortcut == "h") { help(); continue; }
                double answer = 0;
                if (!numeric(command, answer)) {
                    notice = "Enter a finite number only, without units. Your question stays the same.";
                    continue;
                }
                const bool correct = std::fabs(answer - question.answer) < 0.000001;
                round.attempts.push_back({question, answer, correct});
                streak = correct ? streak + 1 : 0;
                round.bestStreak = std::max(round.bestStreak, streak);
                answered = true;
            }
            if (endRound) break;
        }
        round.seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - started).count();
        return round;
    }

    void review(const Round& round) {
        if (round.attempts.empty()) return;
        std::size_t index = 0;
        while (running) {
            const auto& attempt = round.attempts[index];
            Screen screen = header("ANSWER REVIEW " + number(static_cast<int>(index + 1)) + " / " + number(round.answered()));
            screen.push_back({});
            Screen body = wrapped(attempt.question.text, terminal.width - 4);
            append(screen, panel(terminal.width, "QUESTION", body, Tone::Accent));
            screen.push_back({});
            Screen answers = {
                text(std::string(attempt.correct ? "CORRECT" : "INCORRECT") + "  /  Your answer: " + displayNumber(attempt.answer),
                     attempt.correct ? Tone::Success : Tone::Danger),
                text("Correct answer: " + number(attempt.question.answer))
            };
            append(answers, wrapped("Solution: " + attempt.question.explanation, terminal.width - 4, Tone::Muted));
            append(screen, panel(terminal.width, "WORKED SOLUTION", answers));
            footer(screen, "[N / Enter] Next   [P] Previous   [B] Results   [Q] Quit");
            terminal.draw(screen);
            std::string command;
            if (!terminal.read(command) || lower(command) == "q") { running = false; break; }
            command = lower(command);
            if (command == "b") break;
            if (command == "p" && index > 0) --index;
            if (command == "n" || command.empty()) {
                if (index + 1 < round.attempts.size()) ++index;
                else break;
            }
        }
    }

    bool results(const Round& round) {
        std::string notice;
        while (running) {
            Screen screen = header(round.complete() ? "ROUND COMPLETE" : "ROUND ENDED");
            screen.push_back({});
            const std::string rank = !round.complete() ? "THE NEXT CHALLENGE IS WAITING" : round.correct() == 10 ? "FLAWLESS / MATH QUEST CHAMPION"
                : round.correct() >= 7 ? "EXCELLENT / SHARP THINKING" : "KEEP EXPLORING / EVERY STEP COUNTS";
            Screen body = {text(rank, Tone::Accent), text(""),
                aligned("TOTAL POINTS", number(round.points()) + " / " + number(round.difficulty * 100), terminal.width - 4),
                aligned("ACCURACY", percentage(round.correct(), round.answered()), terminal.width - 4),
                aligned("QUESTIONS ANSWERED", number(round.answered()) + " / 10", terminal.width - 4),
                aligned("CORRECT ANSWERS", number(round.correct()), terminal.width - 4),
                aligned("BEST STREAK", number(round.bestStreak), terminal.width - 4),
                aligned("TIME SPENT", std::to_string(round.seconds) + " seconds", terminal.width - 4)};
            append(screen, panel(terminal.width, levels[round.difficulty - 1] + " / PERFORMANCE REPORT", body, Tone::Accent));
            screen.push_back({});
            const int earned = round.correct();
            Line bar = {{"  ACCURACY  ", Tone::Muted}, {repeated("━", earned * 2), Tone::Success},
                        {repeated("─", (10 - earned) * 2), Tone::Border}, {"  " + percentage(earned, 10), Tone::Muted}};
            // This bar measures progress toward ten correct answers, even in a partial round.
            bar[0].text = "  MASTERY   ";
            screen.push_back(bar);
            footer(screen, "[Enter] Play again   [M] Menu   [R] Review   [T] Theme   [Q] Quit", notice);
            terminal.draw(screen);
            std::string command;
            if (!terminal.read(command)) { running = false; break; }
            command = lower(command);
            notice.clear();
            if (command.empty()) return true;
            if (command == "m" || command == "b") return false;
            if (command == "q") { running = false; break; }
            if (command == "t") terminal.theme = (terminal.theme + 1) % 3;
            else if (command == "r") {
                if (round.attempts.empty()) notice = "No answers to review in this round yet.";
                else review(round);
            } else notice = "Use Enter to play again, M for the menu, R to review, or Q to quit.";
        }
        return false;
    }

public:
    explicit Game(Terminal& display) : terminal(display) {}
    void run() {
        while (running) {
            const int difficulty = home();
            if (!difficulty) break;
            bool replay = true;
            while (running && replay) {
                const Round round = play(difficulty);
                history.push_back(round);
                replay = running && results(round);
            }
        }
    }
};
} // namespace dashboard

int main(int argc, char** argv) {
    bool forcePlain = false;
    bool forceUI = false;
    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];
        if (argument == "--plain") forcePlain = true;
        else if (argument == "--ui") forceUI = true;
        else if (argument == "--help") {
            std::cout << "Math Quest - C++17 math arcade\n"
                      << "Run without flags for the terminal dashboard.\n"
                      << "--plain  Simple interface for IDE consoles or redirected input\n"
                      << "--ui     Force the dashboard in an ANSI-compatible terminal\n"
                      << "         Recommended: at least 80 columns by 24 rows\n";
            return 0;
        } else {
            std::cerr << "Unknown option: " << argument << ". Use --help.\n";
            return 1;
        }
    }
    dashboard::Terminal terminal;
    if (forcePlain || (!terminal.available && !forceUI)) return plainGame();
    terminal.width = std::max(76, terminal.width);
    {
        terminal.begin();
        dashboard::Game game(terminal);
        game.run();
    }
    return 0;
}
