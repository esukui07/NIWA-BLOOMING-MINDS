#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <limits>
#include <iomanip>
#include <cctype>
#include <chrono>
#include <thread>
#include "math_problem.h" // Word Problems
#include <filesystem> // Used to delete gamedata.txt
#include <conio.h> // Used for pressing any keys to continue
#include <vector>
using namespace std;

namespace fs = std::filesystem;

// ─── Function Declarations ─────────────────────────────────
// Profile management functions
void enterUsername(string& username);
void showUsername(const string& username);
void flowerListMenu();
void saveFlower(const string& flower);
bool loadFlower(string& flower);
// Title screen and main interface functions
void setColor(int fg, int bg = 0);
void showTitleScreen();
void clearScreen();
void pauseScreen();
void mainInterface();
void showFlowerList();
void showStats();
void showFlowerTells();
void showFlowerStory(const string& flower, int notesUnlocked); 
void bloomMathMenu();
void playPollenWords();
void playGrowAStory();
void jankenPonMenu();
void resetGameData();
void forceQuit();
void actionBar();
// Input Validation functions
int getIntegerInput(int minimum, int maximum);
double getMathOrActionInput();
char getYesNoInput();
char getJankenInput();
char getMainMenuInput();

// ─── Color Constants ─────────────────────────────────
const int COLOR_BG = 15;   // White background
const int COLOR_TITLE = 5; // Purple
const int COLOR_TEXT = 7;  // Light gray
const int COLOR_GOOD = 10; // Light green
const int COLOR_BAD = 12;  // Light red
const int COLOR_INFO = 6; // Yellow

// ─── Game Data Structure ─────────────────────────────────
struct GameData{
    string username = "";
    string selectedFlower = "";
    int points = 0;
    int timePlayed = 0;
    int questionsAnswered = 0;
    int bestStreak = 0;
    int notesUnlocked = 0;
};
void saveGameData(const GameData& data);
bool loadGameData(GameData& data);
GameData player;

int getIntegerInput(int minimum, int maximum){
    int value;
    while(true){
        cin >> value;
        if(!cin.fail() && value >= minimum && value <= maximum){
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        setColor(COLOR_BAD);
        cout << "Invalid input. Enter a number from " << minimum
             << " to " << maximum << ": ";
    }
}

double getMathOrActionInput(char& action){
    double value;
    action = '\0';

    while(true){
        if(cin >> value){
            if(cin.peek() == '\n'){
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
        cin.clear();

        string command;
        cin >> command;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(command.length() == 1){
            char c = toupper(command[0]);
            if (c == 'H' || c == 'B' || c == 'Q'){
                action - c;
                return 0;
            }
        }

        setColor(COLOR_BAD);
        cout << "Invalid input. Enter a valid number: ";
        setColor(COLOR_TEXT);
    }
}

char getYesNoInput(){
    char input;
    while(true){
        cin >> input;
        input = toupper(input);
        if(input == 'Y' || input == 'y' || input == 'N' || input == 'n'){
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        setColor(COLOR_BAD);
        cout << "Invalid input. Enter Y or N: ";
    }
}

char getJankenInput() {
    char choice;

    while (true) {
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 'R' || choice == 'r' ||
            choice == 'P' || choice == 'p' ||
            choice == 'S' || choice == 's' ||
            choice == 'Q' || choice == 'q') {
            return choice;
        }

        setColor(COLOR_BAD);
        cout << "Invalid choice. Enter R, P, S, or Q: ";
    }
}

char getMainMenuInput() {
    char choice;

    while (true) {
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        choice = toupper(choice);

        if ((choice >= '1' && choice <= '4') ||
            choice == 'S' || choice == 'N' || choice == 'R' || choice == 'Q') {
            return choice;
        }

        setColor(COLOR_BAD);
        cout << "Invalid choice. Enter 1-4, S, N, R, or Q: ";
    }
}

// ─── Main Function ─────────────────────────────────
int main() {
    srand(time(0)); // Seed the random number generator with the current time

    SetConsoleOutputCP(CP_UTF8); // Allows the terminal to print multi-byte Unicode symbols such as the ones used for the progress bar

    if (!loadGameData(player)) {
        cout << "No saved progress found.\n";
        enterUsername(player.username);
        saveGameData(player);
    } else {
        showUsername(player.username);
    }
    showTitleScreen(); // Show the title screen
    int choice;

    if (player.selectedFlower.empty()) { // If no flower is selected, show the flower list
        showFlowerList();
    } else {
        cout << "Welcome back, " << player.username << "!\n";
        cout << "Your flower is: " << player.selectedFlower << '\n';
        cout << "Your points are: " << player.points << '\n';
    }

    do{
        mainInterface(); // Call the main interface function to start the program
        char choice = getMainMenuInput();
        switch (choice) {
            case '1': bloomMathMenu(); break;
            case '2': playPollenWords(); break;
            case '3': playGrowAStory(); break;
            case '4': jankenPonMenu(); break;
            case 'S': showStats(); break;
            case 'N': showFlowerTells(); break;
            case 'R': resetGameData(); break;
            case 'Q':
                saveGameData(player);
                return 0;
        }
    }while (choice != 0);

    return 0;
}

void setColor(int fg, int bg) { //fg means foreground(text) color, bg means background color
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, fg | (bg << 4));
    /*This Windows function changes the console’s text and background colors.*/

}

void clearScreen() {
    system("cls");
}

void pauseScreen() {
    setColor(COLOR_INFO);
    cout << "\nPress ENTER to continue...";
    cin.get();
}
void showTitleScreen() {
    clearScreen();
    setColor(COLOR_TITLE);
    cout << "NIWA MIND\n";
    setColor(COLOR_TEXT);
    cout << "Welcome to the NIWA MIND game!\n";
    pauseScreen();
}

void enterUsername(string& username) {
     do {
        clearScreen();
        setColor(COLOR_TITLE);
        cout << "Enter your username: ";
        getline(cin, username);
 
        if (username.empty()) {
            cout << "Username cannot be empty.\n";
        }

    } while (username.empty());
}

void showUsername(const string& username) {
    setColor(COLOR_INFO);
    cout << "Welcome, " << username << "!\n";
}

void saveFlower(const string& flower) {
    ofstream file("flower.txt");
    file << flower;
    file.close();
}

bool loadFlower(string& flower) {
    ifstream file("flower.txt");
    if (getline(file, flower) && !flower.empty()) {
        file.close();
        return true;
    }
    file.close();
    return false;
}

void mainInterface() {
    clearScreen();
    setColor(COLOR_TITLE);
    cout << "NIWA MIND\n";
    showUsername(player.username); // Show the username
    setColor(COLOR_TEXT);
    cout << "Flower: " << (player.selectedFlower.empty() ? "None selected" : player.selectedFlower) << '\n'
         << "Points: " << player.points
         << "   Time played: " << player.timePlayed << " seconds"
         << "   Flower Tells: " << player.notesUnlocked << " / 3\n\n";
    cout << "1. Bloom Math\n"
         << "2. Pollen Words\n"
         << "3. Grow A Story\n"
         << "4. Janken Pon\n\n"
         << "[1-4] Play   [S] Stats   [N] Flower Tells   [R] Reset Game Data   [Q] Quit\n"
         << "\nChoose an option: ";
}

void saveGameData(const GameData& data) {
    ofstream file("gamedata.txt");
    file << data.selectedFlower << '\n'
         << data.points << '\n'
         << data.timePlayed << '\n'
         << data.questionsAnswered << '\n'
         << data.bestStreak << '\n'
         << data.notesUnlocked;
    file.close();
}

bool loadGameData(GameData& data) {
    ifstream file("gamedata.txt");

    if (!file) {
        return false;
    }

    getline(file, data.selectedFlower);
    file >> data.points
         >> data.timePlayed
         >> data.questionsAnswered
         >> data.bestStreak
         >> data.notesUnlocked;

    file.close();
    return true;
}

void showStats() {
    clearScreen();
    setColor(COLOR_TITLE);
    cout << "--- GAME STATS ---\n\n";
    setColor(COLOR_TEXT);
    cout << "Flower: " << (player.selectedFlower.empty() ? "None selected" : player.selectedFlower) << '\n';
    cout << "Total points: " << player.points << '\n';
    cout << "Time played: " << player.timePlayed << " seconds\n";
    cout << "Questions answered: " << player.questionsAnswered << '\n';
    cout << "Best streak: " << player.bestStreak << '\n';
    cout << "Flower Tells unlocked: " << player.notesUnlocked << " / 3\n";
    pauseScreen();
}

void showFlowerTells() {
    clearScreen();
    setColor(COLOR_TITLE);
    cout << "--- FLOWER TELLS ---\n\n";
    setColor(COLOR_TEXT);

    if (player.selectedFlower.empty()) {
        cout << "Choose a flower first.\n";
        pauseScreen();
        return;
    }

    cout << "Flower: " << player.selectedFlower << "\n";
    cout << "Notes unlocked: " << player.notesUnlocked << " / 3\n\n";

    showFlowerStory(player.selectedFlower, player.notesUnlocked);

    pauseScreen();
}

void showFlowerStory(const string& flower, int notesUnlocked) {
    ifstream file("flowertells.txt");

    if (!file) {
        cout << "The flowertells.txt file could not be found.\n";
        cout << "Place it in the same folder as the program.\n";
        return;
    }

    if (notesUnlocked == 0) {
        cout << "No Flower Tells have been unlocked yet.\n";
        return;
    }

    string line;
    string flowerHeader = "[" + flower + "]";
    bool foundFlower = false;
    int noteNumber = 0;

    while (getline(file, line)) {
        if (line == flowerHeader) {
            foundFlower = true;
            continue;
        }

        if (foundFlower && !line.empty() && line.front() == '[') {
            break;
        }

        if (foundFlower && !line.empty() && noteNumber < notesUnlocked) {
            noteNumber++;
            cout << "Note " << noteNumber << ": " << line << "\n\n";
        }
    }

    if (!foundFlower) {
        cout << "No story was found for the selected flower.\n";
    }

    file.close();
}

void updateFlowerTells() {
    int oldNotes = player.notesUnlocked;

    if (player.points >= 50) {
        player.notesUnlocked = 1;
    }
    if (player.points >= 100) {
        player.notesUnlocked = 2;
    }
    if (player.points >= 200) {
        player.notesUnlocked = 3;
    }

    if (player.notesUnlocked > oldNotes) {
        setColor(COLOR_GOOD);
        cout << "\nYou unlocked a new Flower Tell!\n";
    }
}

void progressBar(int completed, int totalQuestions, int difficulty, const vector<int>& results){
    int barWidth = 0;

    if(difficulty == 1){
        barWidth = 10;
    }else if(difficulty == 2){
        barWidth = 15;
    }else{
        barWidth = 20;
    }

    int filledBlocks = (completed * barWidth) / totalQuestions;

    setColor(COLOR_TEXT);
    cout << "Progress: ";

    for(int i = 0; i < barWidth; i++){
        int qIndex = (i * totalQuestions) / barWidth;

        if(qIndex < results.size() && results[qIndex] == 1){
            setColor(COLOR_GOOD);
            cout << "▮";
        }else if(qIndex < results.size() && results[qIndex] == 2){
            setColor(COLOR_BAD);
            cout << "▮";
        }else{
            cout << "▯";
        }
    }
    setColor(COLOR_TEXT);
}

void actionBar(){
    setColor(COLOR_TEXT);
    cout << "[H] Help   [B] Menu   [Q] Quit\n";
}

double generateNumber(int difficulty){
    if(difficulty == 1){
        return rand() % 9 + 1;
    }else if(difficulty == 2){ 
        return rand() % 49 + 1;
    }else{ 
        return rand() % 99 + 1;
    }
}

char generateOperator(int difficulty){
    char op[] = {'+', '-', '*', '/'};
    int index;

     if(difficulty == 1){
        index = rand() % 2;
    }else if(difficulty == 2){ 
        index = rand() % 3;
    }else if(difficulty == 3){ 
        index = rand() % 4;
    }
    return op[index];
}
    
void playBloomMath(int difficulty){
    WordProblem questions[20];
    randomProblem(questions);

    int totalQuestions = 0;
    int score = 0;
    double correctAnswer = 0;
    double ans;
    auto startTime = chrono::steady_clock::now();
    char action = '\0';

    if(difficulty == 1){
        totalQuestions = 10;
    }else if(difficulty == 2){
        totalQuestions = 15;
    }else{
        totalQuestions = 20;
    }

    vector<int> results(totalQuestions, 0);

    for(int i = 0; i < totalQuestions; i++){
        int n1 = generateNumber(difficulty);
        int n2 = generateNumber(difficulty);
        char op = generateOperator(difficulty);

        if(difficulty == 1 && op == '-' && n1 < n2){
            swap(n1 ,n2);
        }

        if(difficulty == 2 && op == '*'){
            n1 = rand() % 14 + 1;
            n2 = rand() % 14 + 1;
        }

        if(difficulty == 3 && op == '/' && n1 < n2){
            swap(n1, n2);
        }

        switch(op){
            case '+': correctAnswer = n1 + n2; break;
            case '-': correctAnswer = n1 - n2; break;
            case '*': correctAnswer = n1 * n2; break;
            case '/': correctAnswer = n1 / n2; break;
        }

        setColor(COLOR_TEXT);
        progressBar(i + 1, totalQuestions, difficulty, results);
        
        if(difficulty == 1 || difficulty == 2){
            setColor(COLOR_TEXT);
            cout << "\n\nQuestion #" << i + 1 << '\n'
                 << n1 << " " << op << " " << n2 << " = ";
            ans = getMathOrActionInput(action);
            
            if(ans == correctAnswer){
                results[i] = 1;
                setColor(COLOR_GOOD);
                cout << "Correct answer!\n";
                score++;
            }else if(op == '/' && ans != correctAnswer){
                results[i] = 2;
                setColor(COLOR_BAD);
                cout << fixed << setprecision(2) 
                 << "Incorrect! The correct answer was " << correctAnswer << ".\n";
            }else{
                results[i] = 2;
                setColor(COLOR_BAD);
                cout << fixed << setprecision(0) 
                     << "Incorrect! The correct answer was " << correctAnswer << ".\n";
            }
            pauseScreen();
            clearScreen();
        }else{
            setColor(COLOR_TEXT);
            cout << "\n\nQuestion #" << i + 1 << '\n' 
                 << questions[i].question << "\nAnswer: ";
            ans = getMathOrActionInput(action);

            if(ans == questions[i].answer){
                results[i] = 1;
                setColor(COLOR_GOOD);
                cout << "Correct!\n";
                score++;
                pauseScreen();
            }else{
                setColor(COLOR_BAD);
                if(questions[i].answer > 999){
                    results[i] = 2;
                    cout << "Incorrect! The correct answer is " << commaFormat(questions[i].answer) << ".\n";
                    setColor(COLOR_TEXT);
                    cout << "\nExplanation: " << questions[i].explanation << ".\n";
                    pauseScreen();
                }else{
                    results[i] = 2;
                    cout << "Incorrect! The correct answer is " << questions[i].answer << ".\n";
                    setColor(COLOR_TEXT);
                    cout << "\nExplanation: " << questions[i].explanation << ".\n";
                    pauseScreen();
                }
            }    
        }
        clearScreen();
    }
    setColor(COLOR_TEXT);
    cout << "\nYour final score is: " << score << " / " << totalQuestions << '\n';
    pauseScreen();

    int pointsPerQuestion;

    if (difficulty == 1) {
            pointsPerQuestion = 2;
    } else if (difficulty == 2) {
        pointsPerQuestion = 5;
    } else {
        pointsPerQuestion = 10;
    }

    int pointsEarned = score * pointsPerQuestion;
    auto endTime = chrono::steady_clock::now();
    int secondsPlayed = chrono::duration_cast<chrono::seconds>(
        endTime - startTime
    ).count();

    player.points += pointsEarned;
    player.timePlayed += secondsPlayed;
    player.questionsAnswered += totalQuestions;

    cout << "Points earned: " << pointsEarned << '\n';
    cout << "Total points: " << player.points << '\n';
    cout << "Total time played: " << player.timePlayed << " seconds\n";

    updateFlowerTells();
    saveGameData(player);
    pauseScreen();
}

void bloomMathMenu() {
    clearScreen();
    int choice;

    setColor(COLOR_GOOD);
    cout << "1. Easy\n";
    setColor(COLOR_TEXT);
    cout << "Addition / Subtraction\n";
    setColor(COLOR_GOOD);
    cout << "10 QUESTIONS   +2 / CORRECT\n\n";
 
    setColor(COLOR_INFO);
    cout << "2. Medium\n";
    setColor(COLOR_TEXT);
    cout << "Mixed / Products / Divison\n";
    setColor(COLOR_INFO);
    cout << "15 QUESTIONS   +5 / CORRECT\n\n";

    setColor(COLOR_BAD);
    cout << "3. Hard\n";
    setColor(COLOR_TEXT);
    cout << "Word Problems / Algebra / Percentages\n";
    setColor(COLOR_BAD);
    cout << "20 QUESTIONS   +10 / CORRECT\n\n";

    setColor(COLOR_TEXT);
    cout << "0. Back\n"
         << "Choose a difficulty: ";
    
    choice = getIntegerInput(0, 3);

    clearScreen();

    switch(choice){
        case 1:
            setColor(COLOR_GOOD);
            cout << "You have selected easy mode!\n";
            pauseScreen();
            clearScreen();
            playBloomMath(choice);
            break;
        case 2:
            setColor(COLOR_INFO);
            cout << "You have selected medium mode!\n";
            pauseScreen();
            clearScreen();
            playBloomMath(choice);
            break;
        case 3:
            setColor(COLOR_BAD);
            cout << "You have selected hard mode!\n";
            pauseScreen();
            clearScreen();
            playBloomMath(choice);
            break;
        case 0:
            cout << "Returning to main menu...\n";
            return;
            break;
        default:
            setColor(COLOR_BAD);
            cout << "Invalid choice!\n";
            pauseScreen();
            break;
    }
}

char computerChoice(){
    int num = rand() % 3 + 1;

    switch(num){
        case 1: return 'R';
        case 2: return 'P';
        case 3: return 'S';
    }
    return 0;
}

bool chooseWinner(char player, char computer){
    bool playerWon = false;
    switch(player){
        case 'R':
        case 'r':
            if(computer == 'R'){
                setColor(COLOR_INFO);
                cout << "\nIt's a tie!\n";
            }else if(computer == 'P'){
                playerWon = false;
                setColor(COLOR_BAD);
                cout << "\nYou lose.\n";
            }else{
                playerWon = true;
                setColor(COLOR_GOOD);
                cout << "\nYou win!\n";
            }
            break;
        case 'P':
        case 'p':
            if(computer == 'R'){
                playerWon = true;
                setColor(COLOR_GOOD);
                cout << "\nYou win!.\n";
            }else if(computer == 'P'){
                setColor(COLOR_INFO);
                cout << "\nIt's a tie!\n";
            }else{
                playerWon = false;
                setColor(COLOR_BAD);
                cout << "\nYou lose.\n";
            }
            break;
        case 'S':
        case 's':
            if(computer == 'R'){
                playerWon = false;
                setColor(COLOR_BAD);
                cout << "\nYou lose.\n";
            }else if(computer == 'P'){
                playerWon = true;
                setColor(COLOR_GOOD);
                cout << "\nYou win!\n";
            }else{
                setColor(COLOR_INFO);
                cout << "\nIt's a tie!\n";
            }
            break;
    }
    return playerWon;
}

void jankenPonCountdown(){
    setColor(COLOR_TEXT);
    cout << "Jan... ";
    cout.flush();
    this_thread::sleep_for(chrono::seconds(1));

    cout << "Ken... ";
    cout.flush();
    this_thread::sleep_for(chrono::seconds(1));

    cout << "PON!\n";
    cout.flush();
    this_thread::sleep_for(chrono::seconds(1));
}

void jankenPonMenu(){
    int winningStreak = 0;
    while(true){
        clearScreen();
        char player;
        
        setColor(COLOR_INFO);
        cout << "Current streak: " << winningStreak << "\n\n";

        cout << "R - Rock (Guu)\n"
             << "P - Paper (Pa)\n"
             << "S - Scissiors (Choki)\n"
             << "Q - Back\n"
             << "\nChoose: ";

        player = getJankenInput();

        if (player == 'Q' || player == 'q') {
            cout << "\nReturning to main menu...\n";
            return;
        }

        char computer = computerChoice();
        clearScreen();

        if (player == 'R' || player == 'r') {
            cout << "You chose Rock!\n";
        } else if (player == 'P' || player == 'p') {
            cout << "You chose Paper!\n";
        } else {
            cout << "You chose Scissors!\n";
        }

        pauseScreen();
        clearScreen();
        jankenPonCountdown();

        cout << "\nYour choice: " << char(toupper(player)) << '\n'
             << "Computer's choice: " << computer << '\n';

        bool playerWon = chooseWinner(player, computer);

        if (playerWon) {
            winningStreak++;
            setColor(COLOR_GOOD);
            cout << "Winning streak: " << winningStreak << "\n";
        } else if (toupper(player) != toupper(computer)) {
            winningStreak = 0;
            setColor(COLOR_INFO);
            cout << "Winning streak reset to 0.\n";
        } else {
            setColor(COLOR_INFO);
            cout << "Winning streak remains at " << winningStreak << ".\n";
        }

        pauseScreen();
    }
}

void playPollenWords() {
    cout << "Pollen Words coming soon.";
    pauseScreen();
}
void playGrowAStory() {
    cout << "Grow A Story coming soon.";
    pauseScreen();
}

void showFlowerList() {
    int choice;
    char confirm;
    string chosenFlower;

    clearScreen();
    setColor(COLOR_TITLE);
    cout << "--- FLOWER GARDEN ---\n\n";
    setColor(COLOR_TEXT);
    cout << "1. Sunflower\n"
         << "   Meaning: Hope and happiness\n"
         << "   Theme: Finding light during difficult times\n\n"
         << "2. Rose\n"
         << "   Meaning: Love and courage\n"
         << "   Theme: Caring for others despite challenges\n\n"
         << "3. Lotus\n"
         << "   Meaning: Growth and overcoming struggles\n"
         << "   Theme: Rising above a difficult beginning\n\n"
         << "4. Cherry Blossom\n"
         << "   Meaning: Change and memories\n"
         << "   Theme: Accepting the temporary nature of life\n\n"
         << "5. Lavender\n"
         << "   Meaning: Peace and healing\n"
         << "   Theme: Recovering from sadness\n\n"
         << "6. Orchid\n"
         << "   Meaning: Uniqueness and patience\n"
         << "   Theme: Becoming confident in your differences\n\n"
         << "0. Back\n\n"
         << "Choose a flower: ";

    choice = getIntegerInput(0, 6);

    switch (choice) {
        case 1: chosenFlower = "Sunflower"; break;
        case 2: chosenFlower = "Rose"; break;
        case 3: chosenFlower = "Lotus"; break;
        case 4: chosenFlower = "Cherry Blossom"; break;
        case 5: chosenFlower = "Lavender"; break;
        case 6: chosenFlower = "Orchid"; break;
        case 0: return;
        default:
            cout << "Invalid choice!\n";
            pauseScreen();
            return;
    }

    cout << "\nYou selected: " << chosenFlower << "\n";
    cout << "Do you want to grow this flower? (Y/N): ";
    confirm = getYesNoInput();

    if (confirm == 'Y' || confirm == 'y') {
        // Keep the current points and notes when changing flowers.
        player.selectedFlower = chosenFlower;
        saveGameData(player);

        setColor(COLOR_GOOD);
        cout << "\nYour " << player.selectedFlower << " has been selected!\n";
        cout << "Your current progress has been saved.\n";
    } else {
        cout << "\nYour flower selection was cancelled.\n";
    }

    pauseScreen();
}

void forceQuit(){
    setColor(COLOR_INFO);
    cout << "\nPress any key to quit the program...";
    _getch();
    exit(0);
}

void resetGameData(){
    char confirm, reset;

    setColor(COLOR_INFO);
    cout << "Do you want to reset game data? (Y/N): ";
    confirm = getYesNoInput();

    if(confirm == 'Y' || confirm == 'y'){
        setColor(COLOR_BAD);
        cout << "Are you sure? All your saved data will be deleted. You can't undo this action. (Y/N): ";
        reset = getYesNoInput();

        if(reset == 'Y' || reset == 'y'){
            clearScreen();

            fs::path saveFile = fs::current_path() / "gamedata.txt";
            error_code ec;

            if(fs::remove(saveFile, ec)){
                setColor(COLOR_GOOD);
                cout << "Game data successfully deleted.\n";
                forceQuit();
            }else{
                setColor(COLOR_BAD);
                cout << "No save file found or could not delete gamedata.txt.\n";
            }
        }
    }
}