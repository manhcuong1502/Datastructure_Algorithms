#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;
int main ()
{
    fstream ifs;
    int score = 0;
    system("g++ answer.cpp -o ans");
    system("g++ main.cpp -o main");
    string test[] = {"test01.txt", "test02.txt", "test03.txt", "test04.txt", "test05.txt", "test06.txt", "test07.txt", "test08.txt", "test09.txt", "test10.txt",
                    "test11.txt", "test12.txt", "test13.txt", "test14.txt", "test15.txt", "test16.txt", "test17.txt", "test18.txt", "test19.txt", "test20.txt"};
    for(int i = 0; i < 20; i++){
        string cmd0 = "main.exe " + test[i] + " > result.txt";
        string cmd1 = "ans.exe " + test[i] + " > answer.txt";
        system(cmd0.c_str());
        system(cmd1.c_str());

        ifs.open("answer.txt", ios::in);
        ifs.seekg(0, ifs.end);
        int fileSize = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        char* fileContent0 = new char[fileSize];
        ifs.read(fileContent0, fileSize);
        ifs.close();
        
        ifs.open("result.txt", ios::in);
        ifs.seekg(0, ifs.end);
        int fileSize = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        char* fileContent1 = new char[fileSize];
        ifs.read(fileContent1, fileSize);
        ifs.close();
        

        cout << "Test " << i + 1 << ": ";
        if (strcmp(fileContent0, fileContent1) != 0) {
            cout << "Fail\n";
        }
        else {
            cout << "Success\n";
            score++;
        }
    }
    system("rm main ans result.txt answer.txt");
    cout << "Result: \t" << score << "/20\n";
    return(0);
    
} 