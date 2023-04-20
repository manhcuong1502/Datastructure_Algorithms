#include <iostream>
#include <string.h>
using namespace std;

/////////////////////////////////////////////////////////

void printHailstone(int n){
    cout << n;
    if (n != 1){
        cout << " ";
        if (n % 2 == 0){
            printHailstone(n/2);
        }
        else {
            printHailstone(n*3 + 1);
        }
    }
}

/////////////////////////////////////////////////////////

void printArray(int n){
    if (n == 0) cout << n;
    else
    {
        printArray(n - 1);
        cout << ", " << n;
    }
}

/////////////////////////////////////////////////////////

int findMax(int* arr, int length){
    if (length == 0) return arr[0];
    else {
        if (arr[length-1] > arr[0])
            arr[0] = arr[length-1];
        return findMax(arr, length-1);
    }
}

/////////////////////////////////////////////////////////

bool isPalindrome(string str){
    if (str.length() < 2) return 1;
    else if (str[0] != str[str.length()-1]) return 0;
    else {
        str = str.substr(1, str.length() - 2);
        return isPalindrome(str);
    }
}

///////////////// Chuoi con trong chuoi cha /////////////

int getlength(char* str){
    if (*str == '\0') return 0;
    else {
        str++;
        return 1 + getlength(str);
    }
}

bool compareStr(char* text, char* pattern){
    if (*pattern == '\0') return 1;
    else if (*text != *pattern) return 0;
    else {
        text++; 
        pattern++;
        return compareStr(text, pattern);
    }
}

bool containsPattern(char* text, char* pattern){
    if (getlength(text) < getlength(pattern)) return 0;
    else {
        if (*text != *pattern) {
            text++;
            return containsPattern(text, pattern);
        }
        else {
            bool result = false;
            result = compareStr(text, pattern);
            if (result == false){
                text++;
                return containsPattern(text, pattern);
            }
            else return true;
        }

    }
}

//////////////////////////////////////////////////////////

int findGCD(int a, int b){
    if (b != 0) return findGCD(a, a%b);
    else return a;
}

//////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////


int main(){
    char str[] = "2020";
    printf("%d", myArrayToInt(str, 4));
}