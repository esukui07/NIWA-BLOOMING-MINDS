#ifndef MATH_PROBLEM_H
#define MATH_PROBLEM_H

#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;

struct WordProblem{
    string question;
    double answer;
    string explanation;
};

string commaFormat(int num){
    string numStr = to_string(num);
    int insertPosition = numStr.length() - 3;

    while(insertPosition > 0){
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numStr;
}

string randomName(){
    int index = rand() % 20;
    string names[20] = {"Calyx", "Oliver", "Ace", "Luis", "Keet", "Yaj", "Charles", "Brian", "Liam", "Jiro",
                        "Noelle", "Olivia", "Grace", "Louisa", "Kate", "Yana", "Charlene", "Angela", "Rosemary", "Emma" };
    return names[index];
}

string randomItem(string category){
    int index = rand() % 5;

    if(category == "school"){
        string schoolItems[5] = {"Pencils", "Books", "Notebooks", "Papers", "Markers"};
        return schoolItems[index];
    }else if(category == "fruit"){
        string fruitItems[5] = {"Apples", "Mangoes", "Bananas", "Oranges", "Strawberries"};
        return fruitItems[index];
    }else if(category == "flavor"){
        string flavorItems[5] = {"Ube", "Chocolate", "Strawberry", "Vanilla", "Cheese"};
        return flavorItems[index];
    }else if(category == "dessert"){
        string dessertItems[5] = {"Cupcakes", "Cookies", "Ice Creams", "Cakes", "Pies"};
        return dessertItems[index];
    }else if(category == "personal"){
        string personalItems[5] = {"a new pair of shoes", "Groceries", "CDs", "new T-shirts", "Deodorants"};
        return personalItems[index];
    }else if(category == "theater"){
        string theaterItems[5] = {"Drinks", "Tickets", "Popcorns", "Snacks", "Chips"};
        return theaterItems[index];
    }else if(category == "store"){
        string storeItems[5] = {"Bag", "Shirt", "Jacket", "Mini Fan", "Powerbank"};
        return storeItems[index];

    }else{
        return "";
    }
}

inline void randomProblem(WordProblem list[20]){
    srand(time(0));

    bool used[7][3] = {false};

    for(int i = 0; i < 20; i++){
        int type = rand() % 7;
        int scenario = rand() % 3;

        while(used[type][scenario] == true){
            type = rand() % 7;
            scenario = rand() % 3;
        }
        
        used[type][scenario] = true;
        
        string school, fruit, flavor, dessert, store, personal, theater;

        if(type == 0){
            if(scenario == 0){     
                string item = randomItem("school");
                string name = randomName();

                int a = rand() % 350 + 150; // 150-499
                int b = rand() % 350 + 150;
                int c = rand() % 150 + 50; // 50-199

                list[i].question = name + " collected old " + item + " for a charity drive. During the first week, they collected " + to_string(a) + " " + item +
                                   ". During the second week, they collected " + to_string(b) + " " + item + ". In the final weekend, a local school donated an extra " +
                                   to_string(c) + " " + item + ". How many total did " + name + " collect in all?";
                list[i].answer = a + b + c;
                list[i].explanation = to_string(a) + " + " + to_string(b) + " + " + to_string(c) + " = " + to_string(a + b + c) + " - Add it all together";
            }else if(scenario == 1){
                string item = randomItem("fruit");
                string item2 = randomItem("fruit");

                while(item == item2){
                    item2 = randomItem("fruit");
                }

                string item3 = randomItem("fruit");

                while(item2 == item3 || item == item3){
                    item3 = randomItem("fruit");
                }

                string name1 = randomName();
                string name2 = randomName();

                while(name1 == name2){
                    name2 = randomName();
                }

                int a = rand() % 500 + 1500;
                int b = rand() % 500 + 2000;
                int c = rand() % 500 + 1500;

                list[i].question = name1 + " and " + name2 + " owns a small farm. During harvest season, they collected " + commaFormat(a) + " grams of " + item + ", " + commaFormat(b) + " grams of " + item2 + ", and " +
                                   commaFormat(c) + " grams of " + item3 + ". How many fruits did " + name1 + " and " + name2 + " harvest altogether?";
                list[i].answer = a + b + c;
                list[i].explanation = to_string(a) + " + " + to_string(b) + " + " + to_string(c) + " = " + to_string(a + b + c) + " - Add all three together";
            }else{
                string item = randomItem("flavor");
                string item2 = randomItem("flavor");

                while(item == item2){
                    item2 = randomItem("flavor");
                }

                string item3 = randomItem("flavor");

                while(item2 == item3 || item == item3){
                    item3 = randomItem("flavor");
                }

                string item4 = randomItem("dessert");
                string name = randomName();

                int a = rand() % 25 + 25;
                int b = rand() % 25 + 25;
                int c = rand() % 25 + 25;
                int d = rand() % 25 + 25;

                list[i].question = name + " is preparing for their birthday party. They oredered " + to_string(a) + " " + item + " " + item4 + ", " + to_string(b) + " " + item2 + " " + item4 + ", and " + to_string(c) + " " + item3 + " " + item4 + ". " +
                                  "Their aunt also bought " + to_string(d) + " additional " + item4 + ". How many " + item4 + " will there be at " + name + "'s birthday party?";
                list[i].answer = a + b + c + d;
                list[i].explanation = to_string(a) + " + " + to_string(b) + " + " + to_string(c) + " + " + to_string(d) + " = " + to_string(a + b + c + d) + " - Add it all together";
            }
        }else if(type == 1){
            if(scenario == 0){
                string item = randomItem("school");
                string item2 = randomItem("school");

                while(item == item2){
                    item2 = randomItem("school");
                }
    
                string name = randomName();

                int a = rand() % 200 + 300;
                int b = rand() % 350 + 500;
                int c = rand() % 200 + 300;
                int d = rand() % 200 + 50;
                int e = rand() % 200 + 300;
                int f = rand() % 300 + 350;

                list[i].question = name + " owns a school supply store. They initially had " + to_string(b) + " " + item + " and " + to_string(a) + " " + item2 + ". On Monday, they sold " + to_string(c) + " " + item + " and " + to_string(d) + " " + item2 + 
                                  ". On Tuesday, he received " + to_string(f) + " more " + item + " and " + to_string(e) + " " + item2 + ". How many school supplies does " + name + " have now?";
                list[i].answer = (b + a - (c + d)) + e + f;
                list[i].explanation = "\n1. " + to_string(a) + " - " + to_string(c) + " + " + to_string(e) + " = " + to_string(a - c + e) + " - Calculate remaining " + item + ".\n"
                                    + "2. " + to_string(b) + " - " + to_string(d) + " + " + to_string(f) + " = " + to_string(b - d + f) + " - Calculate remaining " + item2 + ".\n"
                                    + "3. " + to_string(a - c + e) + " + " + to_string(b - d + f) + " = " + to_string((a - c + e) + (b - d + f)) + " - Add both totals together";
            }else if(scenario == 1){
                string item = randomItem("personal");
                string name1 = randomName();
                string name2 = randomName();

                 while(name1 == name2){
                    name2 = randomName();
                }

                int a = rand() % 1500 + 3500;
                int b = rand() % 500 + 1500;
                int c = rand() % 1000 + 1500;
                int d = rand() % 500 + 500;

                list[i].question = name1 + " has P" + commaFormat(a) + " in savings, while " + name2 + " has P" + commaFormat(b) + ". " + name1 + " deposits another P" + commaFormat(c) + " into their savings, " + 
                                "while " + name2 + " withdraws P" + commaFormat(d) + " to buy " + item + ". How much money do they have combined?";
                list[i].answer = (a + c) + (b - d);
                list[i].explanation = "\n1. " + to_string(a) + " + " + to_string(c) + " = " + to_string(a + c) + " - Find " + name1 + " and " + name2 + "'s new savings.\n"
                                    + "   " + to_string(b) + " - " + to_string(d) + " = " + to_string(b - d) + '\n'
                                    + "2. " + to_string(a + c) + " + " + to_string(b - d) + " = " + to_string((a + c) + (b - d)) + " - Add it together";
            }else{
                string name1 = randomName();
                string name2 = randomName();

                 while(name1 == name2){
                    name2 = randomName();
                }

                int a = rand() % 150 + 150;
                int b = rand() % 50 + 180;
                int c = rand() % 50 + 100;
                int d = rand() % 50 + 150;
                int e = rand() % 50 + 50;

                list[i].question = name1 + " and " + name2 + " went on a road trip. On the first day, " + name1 + " traveled " + to_string(a) + " kilometers, while " + name2 + " traveled " + to_string(b) + ". " +
                                 "On the second day, " + name1 + " traveled another " + to_string(c) + " kilometers, while " + name2 + " traveled " + to_string(d) + " kilometers. However, they discovered that " +
                                 to_string(e) + " kilometers of their recorded distance had been counted twice. What was their actual combined distance traveled?";
                list[i].answer = (a + b) + (c + d) - e;
                list[i].explanation = "\n1. " + to_string(a) + " + " + to_string(b) + " + " + to_string(c) + " + " + to_string(d) + " = " + to_string(a + b + c + d) + " - Add all recorded distances together.\n"
                                    + "2. " + to_string(a + b + c + d) + " - " + to_string(e) + " = " + to_string((a + b + c + d) - e) + " - Subtract the double counted distance";
            }
        }else if(type == 2){
            if(scenario == 0){
                string item = randomItem("dessert");
                string name1 = randomName();
                string name2 = randomName();

                 while(name1 == name2){
                    name2 = randomName();
                }

                int a = rand() % 15 + 20;
                int b = rand() % 10 + 20;
                int c = rand() % 10 + 15;
                int d = rand() % 10 + 20;

                list[i].question = name1 + " and " + name2 + " own a bakery. " + name1 + " bakes " + to_string(a) + " boxes of " + item + " every day, with " + to_string(b) + " " + item + " in each box. " + name2 +
                                " bakes " + to_string(c) + " boxes of " + item + " every day, with " + to_string(d) + " " + item + " in each box. How many " + item + " do they bake altogether in one day?";
                list[i].answer = (a * b) + (c * d);
                list[i].explanation = "\n1. (" + to_string(a) + " * " + to_string(b) + ") + (" + to_string(c) + " * " + to_string(d) + ")" + " - Multiply both boxes with the amount of " + item + " in each box.\n"
                                      + "2. (" + to_string(a * b) + ") + (" + to_string(c * d) + ") = " + to_string((a * b) + (c * d)) + " - Add the products together";
            }else if(scenario == 1){
                string item = randomItem("theater");
                string name = randomName();

                int a = rand() % 60 + 40;
                int b = rand() % 50 + 100;
                int c = rand() % 40 + 80;
                int d = rand() % 50 + 150;
                int e = rand() % 13 + 10;
                int f = rand() % 5 + 5;

                list[i].question = name + " manages a movie theater. During weekdays, They sell " + to_string(a) + " " + item + " per day at P" + to_string(b) + " each. During weekends, They sell " + to_string(c) +
                               " per day at P" + to_string(d) + " each. The theater operates for " + to_string(e) + " weekdays and " + to_string(f) + " weekend days in one month. How much money does the theater earn " + 
                               "from ticket sales during the entire month?";
                list[i].answer = (a * b * e) + (c * d * f);
                list[i].explanation = "\n1. " + to_string(a) + " * " + to_string(b) + " * " + to_string(e) + " = " + to_string(a * b * e) + " - Weekday earnings.\n"
                                    + "2. " + to_string(c) + " * " + to_string(d) + " * " + to_string(f) + " = " + to_string(c * d * f) + " - Weekend earnings.\n"
                                    + "3. " + to_string(a * b * e) + " + " + to_string(c * d * f) + " = " + to_string((a * b * e) + (c * d * f)) + " - Add both together";
            }else{
                string name = randomName();

                int a = rand() % 6 + 1;
                int b = rand() % 8 + 1;
                int c = rand() % 10 + 1;
                const int weekday = 5;

                list[i].question = name + " studies for " + to_string(a) + " hour/s every weekday and " + to_string(b) + " hours every Saturday. They follow this routine for " + to_string(c) + " weeks. How many hours does " + name +
                              " spend studying altogether?";
                list[i].answer = ((a * weekday) + b) * c;
                list[i].explanation = "\n1. " + to_string(a) + " * " + to_string(weekday) + " = " + to_string(a * weekday) + " - Find weekday hours.\n"
                                    + "2. " + to_string(a * weekday) + " + " + to_string(b) + " = " + to_string((a * weekday) + b) + " - Add it with Saturday.\n"
                                    + "3. " + to_string((a * weekday) + b) + " * " + to_string(c) + " = " + to_string(((a * weekday) + b) * c) + " - Multiply by the weeks";
            }
        }else if(type == 3){
            if(scenario == 0){
                string name = randomName();

                int b = (rand() % 5 + 3) * 2;
                int c = (rand() % 5 + 1) * 2;
                int rate = rand() % 15 + 2;
                int a = rate * (b * c);

                list[i].question = name + " printed " + commaFormat(a) + " pages over " + to_string(b) + " working days. They worked " + to_string(c) + " hours every day and printed at a constant rate. How many pages did they print per hour?";
                list[i].answer = rate;
                list[i].explanation = "\n1. " + to_string(b) + " * " + to_string(c) + " = " + to_string(b * c) + " - Find total hours worked.\n"
                                    + "2. " + to_string(a) + " / " + to_string(b * c) + " = " + to_string(rate) + " - Calculate pages per hour";
            }else if(scenario == 1){
                int a = (rand() % 10 + 15) * 2;
                int b = (rand() % 10 + 3) * 2;
                int c = a * b; 

                list[i].question = "The product of two numbers is " + to_string(c) + ". If one of them is " + to_string(a) + ", Find the other.";
                list[i].answer = c / a;
                list[i].explanation = "\n1. " + to_string(a) + "x = " + to_string(c) + " - Let the unknown number be x.\n"
                                      + "2. x = " + to_string(c) + " / " + to_string(a) + " - Divide both sides by " + to_string(a) + ".\n"
                                      + "3. x = " + to_string(c / a) + " - Calculate the result";
            }else{
                int b = (rand() % 25 + 25) * 2;
                int quotient = rand() % 25 + 25;
                int a = b * quotient;

                list[i].question = "The total amount raised from the school concert was P" + commaFormat(a) + ". " + to_string(b) + " people went to the concert. How much did each person donate on average?";
                list[i].answer = a / b; 
                list[i].explanation = to_string(a) + " / " + to_string(b) + " = " + to_string(a / b) + " - Divide total by the number of people";
            }
        }else if(type == 4){
            if(scenario == 0){
                string name1 = randomName();
                string name2 = randomName();

                 while(name1 == name2){
                    name2 = randomName();
                }

                int a = (rand() % 5 + 1) * 2;
                int b = (rand() % 26 + 10) * 2;
                int c = (b - a) / 2;
                
                list[i].question = name1 + " is " + to_string(a) + " older than " + name2 + ". Together their ages total " + to_string(b) + ". How old is " + name2 + "?";
                list[i].answer = c;
                list[i].explanation = "\n1. " + name2 + "'s age = x + " + to_string(a) + " - Let " + name2 + "'s age be x and set up the equation.\n"
                                    + "   x + (x + " + to_string(a) + ") = " + to_string(b) + ", or 2x + " + to_string(a) + " = " + to_string(b) + '\n'
                                    + "2. " + to_string(b) + " - " + to_string(a) + " = " + to_string(b - a) + " - Subtract the age difference.\n"
                                    + "3. " + to_string(b - a) + " / 2 = " + to_string(c) + " - Divide it by 2";
            }else if(scenario == 1){
                int a = (rand() % 9 + 1) * 5;
                int c = (rand() % 10 + 11) * 5;
                int d = c - a;

                int totalSeats = (rand() % 20 + 5) * 20;

                int b = (totalSeats * d) / 100;

                while(a >= c && (a + c) > 100){
                    a = (rand() % 9 + 1) * 5;
                }

                double pct = (c - a) / 100.0;

                ostringstream ossPct;
                ossPct << pct;

                list[i].question = "An auditorium has a certain number of seats. During an event, " + to_string(a) + "% of the seats were occupied. After " + to_string(b) + " more people entered, the\n" +
                                 "occupancy increased to " + to_string(c) + "%. If every seat was occupied at full capacity, how many seats does the auditorium have?";
                list[i].answer = totalSeats;
                list[i].explanation = "\n1. " + to_string(c) + "% - " + to_string(a) + "% = " + to_string(c - a) + "% - Find the percentage increase.\n"
                                    + "2. " + to_string(b) + " / " + ossPct.str() + " = " + to_string(totalSeats) + " - Calculate total seats";
            }else{
                int a = (rand() % 10 + 11) * 5;
                int c = (rand() % 9 + 1) * 5;

                while((a + c) > 100){
                    a = (rand() % 10 + 11) * 5;
                }

                int multiplier = rand() % 15 + 2;
                int b = a * multiplier;

                double percentage1 = a / 100.0;
                double percentage2 = c / 100.0;
                double originalNumber = b / (a / 100.0);

                ostringstream ossPct1, ossPct2, ossNum;
                ossPct1 << percentage1;
                ossPct2 << percentage2;
                ossNum << originalNumber;

                list[i].question = "If " + to_string(a) + "% of a number is " + to_string(b) + ", what is " + to_string(c) + "% of that number?";
                list[i].answer = (b / a) * c;
                list[i].explanation = "\n1. " + to_string(b) + " / " + ossPct1.str() + " = " + ossNum.str() + " - Find the original number.\n"
                                    + "2. " + ossNum.str() + " * " + ossPct2.str() + " = " + to_string((b / a) * c) + " - Calculate the " + to_string(c) + "%";
            }
        }else if(type == 5){
            if(scenario == 0){
                int a = rand() % 5 + 4;
                int b = rand() % 5 + 6;
                int c = rand() % 5 + 10;
                double rate = rate = (1.0 / a) + (1.0 / b) - (1.0 / c);
                double ans = trunc((1.0 / rate) * 10.0) / 10.0;

                ostringstream oss;
                oss << ans;

                list[i].question = "A reservoir can be filled by Pipe A in " + to_string(a) + " hours and Pipe B in " + to_string(b) + " hours. A leak can empty a full reservoir in " + to_string(c) + " hours. " +
                                "If both pipes are opened while the leak is active, how long will it take to fill the empty reservoir?";
                list[i].answer = ans;
                list[i].explanation = "\n1. " + to_string(a) + " * " + to_string(b) + " * " + to_string(c) + " = " + to_string(a * b * c) + " - Find the common denominator.\n"
                                    + "2. " + to_string(b) + " * " + to_string(c) + " = " + to_string(b * c) + " / " + to_string(a * b * c) + " - Rewrite each fraction with the common denominator.\n"
                                    + "   " + to_string(a) + " * " + to_string(c) + " = " + to_string(a * c) + " / " + to_string(a * b * c) + '\n'
                                    + "   " + to_string(a) + " * " + to_string(b) + " = " + to_string(a * b) + " / " + to_string(a * b * c) + '\n'
                                    + "3. " + to_string(b * c) + " + " + to_string(a * c) + " - " + to_string(a * b) + " = " + to_string((b * c) + (a * c) - (a * b)) + " / " + to_string(a * b * c) + " - Add the numerators.\n"  
                                    + "4. " + to_string(a * b * c) + " / " + to_string((b * c) + (a * c) - (a * b)) + " = " + oss.str() + " - Flip the fraction"; 
            }else if(scenario == 1){
                string name = randomName();

                int a = (rand() % 5 + 5) * 5;
                int b = rand() % 5 + 2;
                int c = rand() % 3 + 1;
                int minCorrect = (a * c) / (b + c) + 1;
                int correct = minCorrect + rand() % (a - minCorrect + 1);
                int incorrect = a - correct;
                int d = (correct * b) - (incorrect * c);

                double wrongAns = (((a * b) - d) / (b + c));

                ostringstream ossWrong;
                ossWrong << wrongAns;

                list[i].question =  name + " answered " + to_string(a) + " questions on an examination. Each correct answer earns " + to_string(b) + " points, while each incorrect answer loses " + to_string(c) + " point. " +
                                 name + " answered every question and received a total score of " + to_string(d) + ". How many questions did " + name + " answer correctly?";
                list[i].answer = correct;
                list[i].explanation = "\n1. " + to_string(a) + " * " + to_string(b) + " = " + to_string(a * b) + " - Find max possible score.\n"
                                    + "2. " + to_string(a * b) + " - " + to_string(d) + " = " + to_string((a * b) - d) + " - Find total points lost.\n"
                                    + "3. " + to_string(b) + " + " + to_string(c) + " = " + to_string(b + c) + " - Calculate the penalty.\n"
                                    + "4. " + to_string((a * b) - d) + " / " + to_string(b + c) + " = " + ossWrong.str() + " - Find the number of wrong answers.\n"
                                    + "5. " + to_string(a) + " - " + ossWrong.str() + " = " + to_string(correct) + " - Find the number of correct answers";
            }else{
                string name = randomName();

                int a = (rand() % 5 + 1) * 1000;
                int b = (rand() % 5 + 4) * 5;
                int c = (rand() % 5 + 2) * 5;
                double d = a * (100.0 - b) * (100.0 - c) / 10000.0;

                double percentageLeft = ((100 - b) / 100.0) * ((100 - c) / 100.0);

                ostringstream ossPct, ossRem, ossAns;
                ossPct << percentageLeft;
                ossRem << d;
                ossAns << a;

                list[i].question = name + " spends " + to_string(b) + "% of their monthly allowance on food and " + to_string(c) + "% of the remaining amount on transportation. If P" + commaFormat(d) + " remains, how much was the original allowance?";
                list[i].answer = a;
                list[i].explanation = "\n1. 100% - " + to_string(b) + "% = " + to_string(100 - b) + "% - Find the percentages left after food and transportation.\n"
                                    + "   100% - " + to_string(c) + "% = " + to_string(100 - c) + "%\n"
                                    + "2. " + to_string(100 - b) + "% * " + to_string(100 - c) + "% = " + ossPct.str() + " - Multiply both percentages.\n"
                                    + "3. " + ossRem.str() + " / " + ossPct.str() + " = " + ossAns.str() + " - Divide remaining to the percentage";
            }
        }else if(type == 6){
            if(scenario == 0){
                string name = randomName();

                int a = (rand() % 20 + 1) * 5;
                int b = (rand() % 20 + 1) * 5;
                int c = (rand() % 20 + 1) * 5;

                list[i].question = name + " scored " + to_string(a) + ", " + to_string(b) + ", and " + to_string(c) + " on three long quizzes. What is her average score?";
                list[i].answer = (a + b + c) / 3;
                list[i].explanation = "\n1. " + to_string(a) + " + " + to_string(b) + " + " + to_string(c) + " = " + to_string(a + b + c) + " - Add the three scores together.\n"
                                    + "2. " + to_string(a + b + c) + " / 3 = " + to_string((a + b + c) / 3) + " - Divide by the number of quizzes";
            }else if(scenario == 1){
                string item = randomItem("store");

                int a = (rand() % 200 + 1) * 5;
                int b = (rand() % 50 + 1) * 5;

                list[i].question = "A " + item + " costs P" + to_string(a) + " after a P" + to_string(b) + " discount. What was its original price?";
                list[i].answer = a + b;
                list[i].explanation = to_string(a) + " + " + to_string(b) + " = " + to_string(a + b) + " - Add the discount to the sale price";
            }else{
                int b = (rand() % 6 + 3) * 2;
                int ans = (rand() % 20 + 15);
                int a = b * ans;
               
                list[i].question = "A water station fills " + to_string(a) + " bottles in " + to_string(b) + " hours. How many bottles does it fill per hour?";
                list[i].answer = ans;
                list[i].explanation = to_string(a) + " / " + to_string(b) + " = " + to_string(ans) + " - Divide total bottles by total hours";
            }
        }
    }
}
#endif