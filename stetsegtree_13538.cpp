#include"stetsegtree.hpp"
using namespace std;
using namespace StetAlgo;


int main(){
    vector<int> V;
    auto reducer = [](int a,int b){return a+b;};
    using checkpointType = size_t;
    checkpointType initChk = 0;
    auto tree = makePersistentSegTree<500000>(V, reducer, initChk);

    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    int m;
    cin >> m;
    int elemCount = 0;
    while(m--){
        int queryType, x, k;
        size_t L, R;
        cin >> queryType;
        if(queryType == 1){
            cin >> x;
            tree.checkout( ++elemCount , true); //overwriting
            tree[x] = (tree[x] + 1);
        }
        else if(queryType == 2){
            cin >> L >> R >> x;
            auto now = tree.begin( R );
            auto past = tree.begin(L-1);
            int left,right;
            while(!now.isTerminal()){
                auto nowL = now.left(); auto pastL = past.left();
                auto nowR = now.right(); auto pastR = past.right();
                left = ((*nowL) != (*pastL)) * ( 2 * (nowL.getInterval().first^x) + 1);
                right = ((*nowR) != (*pastR)) * ( 2 * (nowR.getInterval().second^x) + 1);
                if(left>right){
                    now = nowL; past = pastL;
                } else{
                    now = nowR; past = pastR;
                }
            }
            cout << now.getInterval().first <<'\n';
        }
        else if(queryType == 3){
            cin >> k;
            tree.checkout( elemCount-k , false); // not overwriting
            elemCount -=k ;
        }
        else if(queryType == 4){
            cin >> L >> R >> x;
           // cout << tree.reduce({0,x},R) << endl;
           // cout << tree.reduce({0,x},L-1) << endl;
            cout << tree.reduce({0,x},R) - tree.reduce({0,x},L-1) << '\n';
        }
        else if(queryType == 5){
            cin >> L >> R >> k;
            // kth largest number.
            auto now = tree.begin( R );
            auto past = tree.begin( L-1 );
            int leftval, rightval, a;
            while( !now.isTerminal() ){
                auto nowL = now.left(); auto pastL = past.left();
                leftval = (*nowL) - (*pastL);
                if(k==1){
                    if(leftval ==0){
                        now = now.right(); past = past.right();
                    }
                    else{
                        now = nowL; past = pastL;
                    }
                }
                else if(k>leftval){
                    k -= leftval;
                    now = now.right(); past = past.right();
                }
                else{
                    now = nowL; past = pastL;
                }
            }
            cout << now.getInterval().first << '\n';
        }
    }
}
