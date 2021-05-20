#include"segtree.hpp"
#include<vector>
#include<iostream>
using std::vector;
using std::cout;
using std::endl;
class Adder{
    public:
    int operator()(int a,int b){return a+b;}
} adder;

int main(){
    vector<int> V{1,2,3,4,5,6};
    StetAlgo::SegTree<int,100,Adder> Seg( V, adder, 0);
    cout << Seg[0] << endl;
    Seg[0] = 4;
    cout << Seg[0] << endl;
    cout << Seg.reduce({0,5}) << endl;
}
