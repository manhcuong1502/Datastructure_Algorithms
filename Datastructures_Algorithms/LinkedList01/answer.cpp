
// This file just show answer, not solution of each function
// Đây chỉ là file hiển thị kết quả đúng để chấm điểm, không phải đáp án gợi ý hay code mẫu.

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

int main(int argv, char** argc){
    ifstream ifs;
    ifs.open(argc[1]);
    int n, pos, val;
    ifs >> n;
    vector<int> list1;
    for (int i = 0; i < n; i++){
        ifs >> val;
        list1.push_back(val);
    }
    ifs >> pos;
    ifs >> val;
    ifs.close();
    for (int i = 0; i < n-1; i++){
        cout << list1[i] << " ";
    }
    cout << list1[n-1] << endl;
    vector<int> list;
    bool check = 0;
    for (int i = 0; i < list1.size(); i++){
        if (list1[i] != val){
            list.push_back(list1.at(i));
        }
        else check = 1;
    }
    if (check == 1)
        if (list.size() == 0) cout << -1;
        else cout << 1;
    else cout << 0;

    cout << endl;
    list1.clear();
    if (pos < 0 || pos >= list.size()) cout << -1 << endl;
    else cout << list[pos] << endl;
    
    for (int i = 0; i < list.size(); i++){
        if (i != pos) list1.push_back(list.at(i));
    }
    list.clear();
    
    if (list1.size() == 0) cout << "Empty List!\n";
    else {
        sort(list1.begin(), list1.end(), [](int a, int b)->bool {return a > b;});
        for (int i = 0; i <list1.size() - 1; i++){
            cout << list1.at(i) << " ";
        }
        cout << list1.back() << endl;
    }
}