#include<queue>
#include<vector>
#include<algorithm>
#include<string>
#include<functional>
#include<iostream>
namespace StetAlgo{

template<typename T>
auto makeMaxHeap(){
    return std::priority_queue<T,std::vector<T>,std::less<T> >();
}

template<typename T>
auto makeMinHeap(){
    return std::priority_queue<T,std::vector<T>,std::greater<T> >();
}

template<typename C, typename T>
auto binary_search(const C& container, const T& val){
    if( static_cast<T>(container.front()) < static_cast<T>(container.back()))
        return binary_search(container.cbegin(),container.cend(),val);
    else
        return binary_search(container.crbegin(),container.crend(),val);
}

template<typename C, typename T>
auto lower_bound(const C& container, const T& val){
    if( static_cast<T> (container.front()) < static_cast<T>(container.back()) )
        return std::lower_bound(container.cbegin(),container.cend(),val);
    else 
        return std::lower_bound(container.crbegin(),container.crend(),val);
}

template<typename C, typename T>
auto upper_bound(const C& container, const T& val){
    if( static_cast<T> (container.front()) < static_cast<T>(container.back()) )
        return std::upper_bound(container.cbegin(), container.cend(), val);
    else
        return std::upper_bound(container.crbegin(), container.crend(), val);
}

template<typename PredicateType>
size_t upper_bound(long long l, long long r, PredicateType pred){
    // tries to find the least i in [l,r] that satisfies pred.
    while(l != r){
        long long mid = (l+r)/2;
        if(l==mid || r==mid){
            if(pred(l) == true) return l;
            else return r;
        }
        if(pred(mid) == false) l = mid;
        else r = mid;
    }
}



std::vector<int> boyerMooreSearch(const std::string& in, const std::string& needle){
    std::vector<int> ret;
    auto nowLoc = in.cbegin();
    auto searcher = std::boyer_moore_searcher(needle.cbegin(), needle.cend() );
    while(1){
        nowLoc = std::search(nowLoc, in.cend(), searcher);
        if(nowLoc != in.cend()){
            ret.push_back(std::distance(in.cbegin(),nowLoc)) ;
            nowLoc++;
        }
        else break;
    }
    return ret;
} //this is slow.


std::vector<int> KMPSearch(const std::string& in, const std::string& needle){
    std::vector<int> ret;
    std::vector<int> needleProfile(needle.size()+1);
    needleProfile[0] = -1;
    //makes profile -
    // if mismatched on needle[3], we retry with needle[go[3]]
    int cur;
    for(int i=1;i<=needle.length();++i){
        cur = needleProfile[i-1];
        while(cur>=0 && needle[i-1] != needle[cur])
            cur = needleProfile[cur];
        needleProfile[i] = cur + 1;
    }
    cur=0;
    for(int i=0;i<in.length();i++,cur++){
        if(cur == needle.length() ){
            ret.push_back(i-needle.length());
            cur = needleProfile[cur];
        }
        while(cur>=0 && in[i] != needle[cur])
            cur = needleProfile[cur];
    }
    if(cur == needle.length()) ret.push_back(in.length()-needle.length());
    return ret;
}

}
