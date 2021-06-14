#include"persistentsegtree.hpp"
#include"stdc++.hpp"

using namespace std;
using namespace StetAlgo;

struct dat{
    int count;
    int kind;
    dat(int _count=0,int _kind=0)
        : count{_count},kind{_kind} 
    {}
};

int main(){
//    cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
    int n; cin >> n;
    dat (*reducer)(dat,dat) = [](dat x,dat y){ 
        return dat( x.count+y.count, x.kind+y.kind);
    };
    auto tree = makePersistentSegTree<100000>(vector<dat>(),reducer, 0);
    for(int chk=1;chk<=n;++chk){
        int vk;
        cin >> vk;
        tree.checkout(chk, true);
        dat retrieved = tree.get(vk,chk);
        if(retrieved.kind == 0)
            tree.update(vk, dat(1,1));
        else
            tree.update(vk, dat(retrieved.count+1,1 ));//only one kind possible for terminal node.
    }
    int m; cin >> m;
    int ans = 0;
    while(m--){
        long long a,b,c,d; int k;
        cin >> a>>b>>c>>d>>k;
        if(ans<0) ans = 0;
        int l = 1 + (a*ans + b + n)%n;
        int r = 1 + (c*ans + d + n)%n;
        if(l>r) l^=r^=l^=r;
        // journey for ans starts here
        auto now = tree.getRootNode(r);
        auto past = tree.getRootNode(l-1);
        if(now->val.kind - past->val.kind <k) ans = -1;
        else{
            Interval cover = {0,(1<<17) - 1};
            while(cover.first!=cover.second){
            }


        }


        // ends here
        cout << ans << '\n';
    }

}
