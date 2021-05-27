#include"stetsegtree.hpp"
using namespace StetAlgo;
using namespace std;
int main(){
	cin.tie(NULL); cout.tie(NULL); ios_base::sync_with_stdio(false);
	auto reducer = [](long long a,long long b){return a+b;};
	auto operation = [](Interval interval, long long origVal, long long opID){
		long long length = interval.second - interval.first + 1;
		return origVal + opID * length;
	};
	auto composer = [](long long opID1,long long opID2){return opID1+opID2;};
	long long chogistate = 0ll;

	int n,m,k;
	cin >> n>>m>>k;
	vector<long long> V(n);
	for(int i=0;i<n;++i) cin >> V[i];
	auto tree = makeLazyPropSegTree<1000000>(V, reducer, chogistate, operation, composer);
	for(int i=0;i<m+k;++i){
		int mode; long long l,r,val;
		cin >> mode;
		if(mode==1){
			cin >> l >> r>>val;
			tree.updateInterval({l-1,r-1}, val);
		}
		else{
			cin >> l >> r;
			cout << tree.reduce({l-1,r-1}) <<'\n';
		}
	}
}
