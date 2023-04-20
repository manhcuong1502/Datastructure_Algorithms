#include "ConcatStringTree.cpp"

int main() {
    ConcatStringTree a("ahihi");
    ConcatStringTree b("bhaha");
    ConcatStringTree c("chehe");
    ConcatStringTree d("dhoho");
    ConcatStringTree e("ehuhu");
    cout << a.toString() << endl;
    ConcatStringTree f = a.concat(b).concat(c);
    ConcatStringTree g = d.concat(e);

    cout << f.toStringPreOrder() << endl;
    cout << g.toString() << endl;
    try{
        ConcatStringTree h = f.subString(3,5);
        cout << h.toString();
    }
    catch (exception &e){
        cout << e.what();
    }
    
    return 0;
}