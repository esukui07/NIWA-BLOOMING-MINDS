#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <limits>
#include <iomanip>
#include <cctype>
#include <chrono>
#include <thread>
using namespace std;

void showTitleScreen();
void clearScreen();
void pauseScreen();
void mainInterface();
void showFlowerList();
void showStats();
void showFlowerTells();
void bloomMathMenu();
void playPollenWords();
void playGrowAStory();
void jankenPonMenu();

const int COLOR_BG = 15;   // White background
const int COLOR_TITLE = 11; // Light cyan
const int COLOR_TEXT = 7;  // Light gray
const int COLOR_GOOD = 10; // Light green
const int COLOR_BAD = 12;  // Light red
const int COLOR_INFO = 14; // Yellow

// ─── Main Function ─────────────────────────────────
int main() {
    srand(time(0)); // Seed the random number generator with the current time
    
    showTitleScreen(); // Show the title screen
    int choice;
    
    do{
        mainInterface(); // Call the main interface function to start the program
        cin >> choice;
    
        switch (choice) {
            case 1: bloomMathMenu(); break;
            case 2: playPollenWords(); break;
            case 3: playGrowAStory(); break;
            case 4: jankenPonMenu(); break;
            case 5: showFlowerTells(); break;
            case 0: return 0;
            default:
                cout << "Invalid choice!\n";
                pauseScreen();
                break;
        }
    }while (choice != 0);

    return 0;
}

void setColor(int fg, int bg = 0) { //fg means foreground(text) color, bg means background color
    /*HANDLE is a Windows data type.
    It represents a connection to something managed by Windows. In this case, it represents the console screen
    GetStdHandle
    This Windows function gets a handle to the standard output.
    STD_OUTPUT_HANDLE means:
    Get the console where text is printed.
    The result is stored in: h ,, You can think of h as an address or reference to the console screen.*/ 
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, fg | (bg << 4));
    /*This Windows function changes the console’s text and background colors.
    It needs two things:
    1. The console handle: h
    2. The color setting
    fg | (bg << 4)
    This combines the foreground color and background color into one value.
    bg << 4
    The << here is a bit-shift operator.
    It moves the background color four binary positions to the left.

    Windows stores colors like this:
    Background color | Text color
    
    so kapag setColor(12, 0);
        cout << "This is light red text.\n";
    The text becomes light red with a black background.*/
    
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
void mainInterface() {
    clearScreen();
    setColor(COLOR_TITLE);
    cout << "NIWA MIND\n";
    setColor(COLOR_TEXT);
    cout << "1. Bloom Math\n"
         << "2. Pollen Words\n"
         << "3. Grow A Story\n"
         << "4. Janken Pon\n"
         << "5. Show Flower Tells \n"
         << "0. Exit\n"
         << "\nChoose an option: ";
    
}

void showStats() {
    cout << "Stats coming soon.";
    pauseScreen();
}

void showFlowerTells() {
    cout << "Flower Tells coming soon.";
    pauseScreen();
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
    int totalQuestions = 0;
    int score = 0;
    int lives = 3;
    double correctAnswer = 0;
    double ans;

    if(difficulty == 1){
        totalQuestions = 10;
    }else if(difficulty == 2){
        totalQuestions = 15;
    }else{
        totalQuestions = 20;
    }

    for(int i = 1; i <= totalQuestions; ++i){
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
        cout << "\nQuestion #" << i << '\n'
             << n1 << " " << op << " " << n2 << " = ";
        cin >> ans;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(ans == correctAnswer){
            setColor(COLOR_GOOD);
            cout << "Correct answer!\n";
            score++;
        }else if(op == '/' && ans != correctAnswer){
            setColor(COLOR_BAD);
            cout << fixed << setprecision(2) 
                 << "Incorrect! The correct answer was " << correctAnswer << ".\n";
        }else{
            setColor(COLOR_BAD);
            cout << fixed << setprecision(0) 
                 << "Incorrect! The correct answer was " << correctAnswer << ".\n";
        }

    }
    setColor(COLOR_TEXT);
    cout << "\nYour final score is: " << score << " / " << totalQuestions << '\n';
    pauseScreen();
}

void bloomMathMenu() {
    clearScreen();
    int choice;

    cout << "1. Easy\n"
         << "2. Medium\n"
         << "3. Hard\n"
         << "0. Back\n"
         << "Choose a difficulty: ";
    
    cin >> choice;
    cin.ignore();

    clearScreen();

    switch(choice){
        case 1:
            setColor(COLOR_GOOD);
            cout << "You have selected easy mode!\n";
            pauseScreen();
            playBloomMath(choice);
            break;
        case 2:
            setColor(COLOR_INFO);
            cout << "You have selected medium mode!\n";
            pauseScreen();
            playBloomMath(choice);
            break;
        case 3:
            setColor(COLOR_BAD);
            cout << "You have selected hard mode!\n";
            pauseScreen();
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

void chooseWinner(char player, char computer){
    switch(player){
        case 'R':
        case 'r':
            if(computer == 'R'){
                setColor(COLOR_INFO);
                cout << "\nIt's a tie!\n";
            }else if(computer == 'P'){
                setColor(COLOR_BAD);
                cout << "\nYou lose.\n";
            }else{
                setColor(COLOR_GOOD);
                cout << "\nYou win!\n";
            }
            break;
        case 'P':
        case 'p':
            if(computer == 'R'){
                setColor(COLOR_GOOD);
                cout << "\nYou win!.\n";
            }else if(computer == 'P'){
                setColor(COLOR_INFO);
                cout << "\nIt's a tie!\n";
            }else{
                setColor(COLOR_BAD);
                cout << "\nYou lose.\n";
            }
            break;
        case 'S':
        case 's':
            if(computer == 'R'){
                setColor(COLOR_BAD);
                cout << "\nYou lose.\n";
            }else if(computer == 'P'){
                setColor(COLOR_GOOD);
                cout << "\nYou win!\n";
            }else{
                setColor(COLOR_INFO);
                cout << "\nIt's a tie!\n";
            }
            break;
    }
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
    clearScreen();
    char player;
    char computer = computerChoice();

    cout << "R - Rock (Guu)\n"
         << "P - Paper (Pa)\n"
         << "S - Scissiors (Choki)\n"
         << "Q - Back\n"
         << "\nChoose: ";

    cin >> player;
    cin.ignore();

    clearScreen();

    switch(player){
        case 'R':
        case 'r':
            cout << "You chose Rock!\n";
            pauseScreen();
            clearScreen();
            jankenPonCountdown();
            cout << "\nYour choice: " << char(toupper(player)) << '\n'
                 << "Computer's choice: " << computer << '\n';
            chooseWinner(player, computer);
            pauseScreen();
            break;
        case 'P':
        case 'p':
            cout << "You chose paper!\n";
            pauseScreen();
            clearScreen();
            jankenPonCountdown();
            cout << "\nYour choice: " << char(toupper(player)) << '\n'
                 << "Computer's choice: " << computer << '\n';
            chooseWinner(player, computer);
            pauseScreen();
            break;
        case 'S':
        case 's':
            cout << "You chose scissors!\n";
            pauseScreen();
            clearScreen();
            jankenPonCountdown();
            cout << "\nYour choice: " << char(toupper(player)) << '\n'
                 << "Computer's choice: " << computer << '\n';
            chooseWinner(player, computer);
            pauseScreen();
            break;
        case 'Q':
        case 'q':
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

void playPollenWords() {
    cout << "Pollen Words coming soon.";
    pauseScreen();
}
void playGrowAStory() {
    cout << "Grow A Story coming soon.";
    pauseScreen();
}

void showFlowerList() {
    cout << "Flower List coming soon.";
    pauseScreen();
}