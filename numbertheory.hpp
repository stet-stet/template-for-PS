#ifndef NUMBERTHEORY_HPP_
#define NUMBERTHEORY_HPP_

namespace StetAlgo{

constexpr long long isqrt_impl(long long n,long long xk){
    long long xk1 = (xk+n/xk)/2;
    return (xk1>=xk) ? xk:isqrt_impl(n,xk1);
}

constexpr long long isqrt(long long n){
    return n==0 ? 0 : isqrt_impl(n,n);
}

long long pow(long long a,long long pw,long long mod){
    if(pw==0) return 1;
    else if(pw==1) return a%mod;
    long long res = pow(a,pw/2,mod);
    res = (res*res)%mod;
    if(pw%2==1) res  = (a*res)%mod;
    return res;
}

}
#endif
