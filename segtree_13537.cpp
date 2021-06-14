#include"segtree.hpp"
#include"stdc++.hpp"

using namespace std;
using namespace StetAlgo;

struct query{
    int l; int r; int k;
};

struct partial{
    int loc, k;
};

int main(){
    cin.tie(NULL);cout.tie(NULL);ios_base::sync_with_stdio(false);
    unordered_map<int,int> coord;
    unordered_map<partial,int> results;

    int n;
    vector<int> nos(n);
    cin >> n;
    for(int i=0;i<n;++i) cin >> nos[i];
    vector<int> tosort = nos;
    sort(tosort.begin(),tosort.end(),[](int a,int b){return a<b;});

    int nowcoord = 0;
    coord[tosort[0]] = nowcoord;
    for(int i=1;i<n;++i){
        if(tosort[i] > tosort[i-1]) ++nowcoord;
        coord[tosort[i]] = nowcoord;
    }

    int Q; cin >> Q;
    vector<query> queries(Q);
    vector<partial> partials(2*Q);
    for(int i=0;i<Q;++i){
        cin >> queries[i].l >> queries[i].r >> queries[i].k;
        partials[2*i] = partial(queries[i].l-1, queries[i].k);
        partials[2*i+1] = partial(queries[i].r-1, queries[i].k);
    }

    sort( partials.begin(),partials.end(),[](partial x,partial y){return x.loc<y.loc;} );

    auto tree = makeSegmentTree<100000>(vector<int>(), add<int>());

    int tree_inch=0;

    for(int i=0;i<partials.size();++i){
        while(tree_inch < partials[i].loc){
            tree[ coord[nos[tree_inch]] ] = tree[coord[nos[tree_inch]]] + 1;
            ++tree_inch;
        }
        while( i<partials.size() && tree_inch == partials[i].loc ){
            // find the one we should look at
            // reduce from there to the end
            results[partials[i]] = res;
            ++i;
        }
    }

    for(int i=0;i<Q;++i){
        cout << results[partial(queries[i].r, queries[i].k)] - results[partial(queries[i].r,queries[i].k)] << '\n';
    }

}
