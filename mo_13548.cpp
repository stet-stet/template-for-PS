#include"stdc++.hpp"

using namespace std;

// https://stackoverflow.com/questions/21657491/an-efficient-algorithm-to-calculate-the-integer-square-root-isqrt-of-arbitrari
long long isqrt_impl(long long n,long long xk){
    long long xk1 = (xk+n/xk)/2;
    return (xk1>=xk) ? xk:isqrt_impl(n,xk1);
}

long long isqrt(long long n){
    if(n==0) return 0;
    return isqrt_impl(n,n);
}

class MoCrit{
    long long S;
public:
    MoCrit(long long _S)
        :S{_S}
    {}
    bool operator()(pair<int,int> x,pair<int,int> y) const{
        if(x.first / S != y.first / S) return x.first < y.first;
        return x.second < y.second;
    }
};

struct query_type{
    int s,e,idx,res;
};

int main(){
    cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);
    int n; cin >> n;
    int S = isqrt(n);
    MoCrit moCrit(S);

    vector<int> arr(n+1);
    for(int i=1;i<=n;++i) cin >> arr[i];
    
    int Q; cin >> Q;
    vector<query_type> query(Q);
    for(int i=0;i<Q;++i){
        cin >> query[i].s >> query[i].e;
        query[i].idx = i;
    }
    sort(query.begin(),query.end(),[moCrit](query_type x,query_type y){
            return moCrit({x.s,x.e},{y.s,y.e});
        });

    vector<int> bindo(100002);
    vector<int> histogram(100020);
    histogram[0] = n;

    int s = query[0].s, e = query[0].e;
    for(int i=s;i<=e;++i){
        ++bindo[arr[i]];
        ++histogram[ bindo[arr[i]] ];
    }
    int histmax = 0;
    while(histogram[histmax]>0) ++histmax;
    --histmax;
    for(int i=0;i<Q;++i){
        while( s < query[i].s ){
            //exclude.
            --histogram[bindo[arr[s]]];
            --bindo[arr[s]];
            ++s;
            while(histogram[histmax] == 0) --histmax;
        }
        while( s > query[i].s ){ //inclusion
            --s;
            ++bindo[arr[s]];
            ++histogram[bindo[arr[s]]];
            while(histogram[histmax] > 0) ++histmax; 
            --histmax;
        }
        while(e < query[i].e ){ //inclusion
            ++e;
            ++bindo[arr[e]];
            ++histogram[bindo[arr[e]]];
            while(histogram[histmax] > 0) ++histmax;
            --histmax;
        }
        while(e > query[i].e ){ //exclusion
            --histogram[bindo[arr[e]]];
            --bindo[arr[e]];
            --e;
            while(histogram[histmax] == 0) --histmax;
        }
        query[i].res = histmax;
    }
    sort(query.begin(),query.end(),[](query_type x,query_type y){return x.idx<y.idx;});
    for(int i=0;i<Q;++i){
        cout << query[i].res << '\n';
    }
}
