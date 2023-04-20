#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
int main ()
{   
    int score = 0;
    system("g++ main.cpp -o main");
    string test[] = {"test01.txt", "test02.txt", "test03.txt", "test04.txt", "test05.txt", "test06.txt", "test07.txt", "test08.txt", "test09.txt", "test10.txt"};
    for(int i = 0; i < 10; i++){
        string cmd0 = "./main " + test[i];
        if (!system(cmd0.c_str())){
            cout << "Test " << i + 1 << ": Success\n";
            score ++;
        }
        else cout << "Test " << i + 1 << ": Fail\n";
    }
    system("rm main");
    cout << "Result: \t" << score << "/10\n";
    return(0);
} 