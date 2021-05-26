#include"stetsegtree.hpp"
using namespace StetAlgo;
using namespace std;
int main(){
    auto reducer = [](int a,int b){return a+b;};
    vector<int> a {1,2,3,4,5,6,7,8,9,10};
    auto tree = makeSegTree<16>(a, reducer, 0);
    cout << tree.reduce({0,9}) << endl;
    cout << tree[3] << endl;
    tree[3] = 1;
    cout << tree.reduce({0,9}) << endl;
}
