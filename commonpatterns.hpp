#include<queue>
#include<vector>

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



}
