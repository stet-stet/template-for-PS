#include"persistentsegtree_faster.hpp"
using namespace std;
using namespace StetAlgo;


int main(){
    vector<int> V;
    int (*reducer)(int,int) = [](int a,int b){return a+b;};
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
            tree.update( x, 1 + tree.get(x, elemCount) );
        }
        else if(queryType == 2){
            cin >> L >> R >> x;
            auto now = tree.getRootNode(R);
            auto past = tree.getRootNode(L-1);
            Interval cover {0, 524287};
            int left,right;
            while(cover.first != cover.second){
                left = (now->left->val != past->left->val) * ( 2 * (cover.first ^ x) + 1);
                right = (now->right->val != past->right->val) * ( 2 * (rightHalf(cover).first^x ) + 1);
                if(left>right){
                    now = now->left; past = past->left; cover = leftHalf(cover);
                } else{
                    now = now->right; past = past->right; cover = rightHalf(cover);
                }
            }
            cout << cover.first <<'\n';
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
            auto now = tree.getRootNode(R);
            auto past = tree.getRootNode(L-1);
            Interval cover{0,524287};
            int leftval, rightval, a;
            while( !now->terminal ){
                leftval = now->left->val - past->left->val;
                if(k==1){
                    if(leftval ==0){
                        now = now->right; past = past->right; cover = rightHalf(cover);
                    }
                    else{
                        now = now->left; past = past->left; cover = leftHalf(cover);
                    }
                }
                else if(k>leftval){
                    k -= leftval;
                    now = now->right; past = past->right; cover = rightHalf(cover);
                }
                else{
                    now = now->left; past = past->left; cover = leftHalf(cover);
                }
            }
            cout << cover.first << '\n';
        }
    }
}
