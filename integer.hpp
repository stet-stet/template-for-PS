#ifndef INTEGER_HPP_
#define INTEGER_HPP_

unsigned long long pow(unsigned long long a,unsigned long long pw, unsigned long long mod=0){
    if(pw==0) return 1ll;
    long long x = pow(a,pw/2,mod);
    x = (x*x); if(mod>0) x %= mod;
    if(pw%2==1) x *= a; if(mod>0) x%=mod;
    return x;
}

#endif
