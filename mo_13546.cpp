#include"stdc++.hpp"
#include"mo.hpp"
#include"numbertheory.hpp"

using namespace std;
using namespace StetAlgo;

struct query_type{
    int s,e,idx,res;
};

unordered_map<int, pair<int,int> > repo;
constexpr int N = 100001;
constexpr int S = isqrt(N);
constexpr int bucketNo = N/S;
int bucket[bucketNo+10];
int cnt[N];
int QQQ(){
    for(int idx = bucketNo+9;idx>=0;--idx){
        if(bucket[idx] == 0) continue;
        for(int j=S-1;j>=0;--j){
            if(idx*S+j < N && cnt[idx*S+j] > 0){
                return idx*S+j;
            }
        }
    }
    return 0;
}

int main(){
    cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);
    int n,k; cin >> n >> k;
    MoCrit moCrit(S);
    cnt[0] = n;
    bucket[0] = n;
    vector<int> subsetsum(n+1);
    vector<int> link_to_next(n+1,-1);
    vector<int> link_to_prev(n+1,-1);
    unordered_map<int,int> tempidx;
    for(int i=1;i<=n;++i){
        cin >> subsetsum[i];
    }
    for(int i=0;i<=n;++i){
        link_to_prev[i] = -1;
        if(tempidx.count(subsetsum[i])){
            link_to_prev[i] = tempidx[subsetsum[i]];
            link_to_next[tempidx[subsetsum[i]]] = i;
        }
        link_to_next[i] = -1;
        tempidx[subsetsum[i]] = i;
    }
    for(int i=0;i<=n;++i){
        repo[subsetsum[i]] = {-1,-1};
    }
    
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
    for(int i=s;i<=e;++i){
        pair<int,int> temp = repo[subsetsum[i]];
        if(temp == make_pair(-1,-1)){
            repo[subsetsum[i]] =  {i,i};
        } else {
            repo[subsetsum[i]] = {temp.first,i};
            --cnt[temp.second - temp.first];
            ++cnt[i - temp.first];
            --bucket[(temp.second - temp.first)/S];
            ++bucket[(i - temp.first) / S];
        }
    }

    for(int i=0;i<Q;++i){
        while(s < query[i].s){    //exclusion.
            if(s<=e){
            pair<int,int> temp = repo[subsetsum[s]];
            if(temp.first==temp.second){
                repo[subsetsum[s]] = {-1,-1};
            } else {
                repo[subsetsum[s]] = {link_to_next[s],temp.second};
                --cnt[temp.second-temp.first];
                ++cnt[temp.second-link_to_next[s]];
                --bucket[(temp.second-temp.first)/S];
                ++bucket[(temp.second-link_to_next[s])/S];
            }
            }
            ++s;
        }
        while(s > query[i].s ){ //inclusion
            --s;
            if( s <= e){
            pair<int,int> temp = repo[subsetsum[s]];
            if(temp == make_pair(-1,-1)){
                repo[subsetsum[s]] = {s,s};
            } else {
                repo[subsetsum[s]] = {s,temp.second};
                --cnt[temp.second-temp.first];
                ++cnt[temp.second - (s)];
                --bucket[(temp.second-temp.first)/S];
                ++bucket[(temp.second-(s))/S];
            }
            }
        }
        while(e < query[i].e){//inclusion
            ++e;
            if( s <=e){
            pair<int,int> temp = repo[subsetsum[e]];
            if(temp == make_pair(-1,-1)){
                repo[subsetsum[e]] = {e,e};
            }
            else{
                repo[subsetsum[e]] = {temp.first,e};
                --cnt[temp.second-temp.first];
                ++cnt[e - temp.first];
                --bucket[(temp.second-temp.first)/S];
                ++bucket[(e-temp.first)/S];
            }
            }
        }
        while(e > query[i].e){ //exclusion
            if(s<=e){
            pair<int,int> temp = repo[subsetsum[e]];
            if(temp.first == temp.second){
                repo[subsetsum[e]] = {-1,-1};
            } else {
                repo[subsetsum[e]] = {temp.first,link_to_prev[e]};
                --cnt[temp.second-temp.first];
                ++cnt[link_to_prev[e]-temp.first];
                --bucket[(temp.second-temp.first)/S];
                ++bucket[(link_to_prev[e]-temp.first)/S];
            }
            }
            --e;
        }
        query[i].res = QQQ();
    }
    sort(query.begin(),query.end(), [](query_type x,query_type y){return x.idx<y.idx;});
    for(int i=0;i<Q;++i){
        cout << query[i].res << '\n';
    }
}
