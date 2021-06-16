#include"segtree.hpp"
#include"commonpatterns.hpp"
#include"stdc++.hpp"

using namespace std;
using namespace StetAlgo;

struct query{
    int l; int r; int k;
};

struct partial{
    int loc;
    int k;
    int origquery;
    partial(int a=0,int b=0, int c=0)
        : loc{a}, k{b}, origquery{c}
    {}
};

int main(){
    cin.tie(NULL);cout.tie(NULL);ios_base::sync_with_stdio(false);
    unordered_map<int,int> coord;

    int n;
    cin >> n;
    vector<int> nos(n);

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
    vector<int> results(Q);
    for(int i=0;i<Q;++i){
        cin >> queries[i].l >> queries[i].r >> queries[i].k;
        partials[2*i] = partial(queries[i].l-1, queries[i].k, -i-1);
        partials[2*i+1] = partial(queries[i].r, queries[i].k, i+1);
    }

    sort( partials.begin(),partials.end(),[](partial x,partial y){return x.loc<y.loc;} );

    auto tree = makeSegTree<100000>(vector<int>(), [](int x,int y){return x+y;} );

    int tree_inch=1;

    for(int i=0;i<partials.size();){
        while(tree_inch <= partials[i].loc ){
            tree[ coord[nos[tree_inch-1]] ] = tree[coord[nos[tree_inch-1]]] + 1;
            ++tree_inch;
        }
        while( i<partials.size() && tree_inch > partials[i].loc ){
            auto itt = lower_bound(tosort.begin(),tosort.end(),partials[i].k+1);
            int res;
            if(itt != tosort.end())
                res = tree.reduce({coord[ (*itt)], 100003});
            else
                res = 0;
            if(partials[i].origquery > 0) results[partials[i].origquery-1] += res;
            else results[-partials[i].origquery-1] -= res;
            ++i;
        }
    }

    for(int i=0;i<Q;++i){
        cout << results[i]<<'\n';
    }

}
