#include"stdc++.hpp"
#include"mo.hpp"
#include"numbertheory.hpp"

using namespace std;
using namespace StetAlgo;

struct query_type{
    int s,e,idx;
    long long res;
};

unordered_map<int, pair<int,int> > repo;
constexpr int N = 100001;
constexpr int S = isqrt(N);

int main(){
    cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);
    int n,K; cin >> n >> K;
    MoCrit moCrit(S);
    vector<int> arr(n+1);
    vector<int> subsetsum(n+1);
    for(int i=1;i<=n;++i){
        cin >> arr[i];
        subsetsum[i] = subsetsum[i-1] ^ arr[i];
    }
    vector<long long> bins(1<<22 + 1,0);
    long long totl=0;

    int Q; cin >> Q;
    vector<query_type> query(Q);
    for(int i=0;i<Q;++i){
        cin >> query[i].s >> query[i].e;
        query[i].idx = i;
    }
    sort(query.begin(),query.end(),[moCrit](query_type x,query_type y){
            return moCrit({x.s,x.e},{y.s,y.e});
        });
     
    int s = query[0].s, e = query[0].e;
    for(int i=s-1;i<=e;++i){
        if(bins[subsetsum[i]]>=0 && bins[subsetsum[i]^K] > 0){
            totl += bins[subsetsum[i]^K];
        }
        ++bins[ subsetsum[i] ];
    }

    for(int i=0;i<Q;++i){
        while(s < query[i].s){    //exclusion.
            if(s-1<=e){
            --bins[subsetsum[s-1]];
            if(bins[subsetsum[s-1]]>=0 && bins[subsetsum[s-1]^K] > 0){
                totl -= (bins[subsetsum[s-1]^K]);
            }
            }
            ++s;
        }
        while(s > query[i].s ){ //inclusion
            --s;
            if(s-1<=e){
            if(bins[subsetsum[s-1]]>=0 && bins[subsetsum[s-1]^K]>0){
                totl += bins[subsetsum[s-1]^K];
            }
            ++bins[subsetsum[s-1]];
            }
        }
        while(e < query[i].e){//inclusion
            ++e;
            if(s-1<=e){
            if(bins[subsetsum[e]]>=0 && bins[subsetsum[e]^K]>0){
                totl += bins[subsetsum[e]^K];
            }
            ++bins[subsetsum[e]];
            }
        }
        while(e > query[i].e){ //exclusion
            if(s-1<=e){
            --bins[subsetsum[e]];
            if(bins[subsetsum[e]]>=0 && bins[subsetsum[e]^K]>0){
                totl -=(bins[subsetsum[e]^K]);
            }
            }
            --e;
        }
        query[i].res = totl;
    }
    sort(query.begin(),query.end(), [](query_type x,query_type y){return x.idx<y.idx;});
    for(int i=0;i<Q;++i){
        cout << query[i].res << '\n';
    }
}
