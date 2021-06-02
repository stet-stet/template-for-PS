#ifndef MILLERRABIN_HPP_
#define MILLERRABIN_HPP_

#include"randint.hpp"
#include"integer.hpp"
#include<algorithm>
#include<iostream>

//using std::cout;
//using std::endl;
namespace StetAlgo{
//TODO: replace "long long" with bigbigint
bool millerTest(unsigned long long n, unsigned long long d, int iterations){
//    cout << "millerTest("<<n<<","<<d<<","<<iterations<<")"<<endl;
    RNG<unsigned long long> rng(2,n-2);
    bool passflag=true;
    while(iterations--){
  //      cout << "=============iteration " << iterations << endl; 
        unsigned long long a = rng();
    //    cout << a << endl;
        unsigned long long x = pow(a, d, n);
      //  cout << x << endl;
        if(x==1 || x==n-1) continue;
        else{
            unsigned long long dtemp = d;
            while(dtemp != n-1){
  //              cout << "dtemp: " << dtemp << endl;
                x = (x*x)%n;
                dtemp*=2;
                if(x==1){
                    passflag=false; break;
                }
                if(x==n-1) break;
            }
            if(dtemp == n-1) passflag=false;
        }

        if(passflag == false) return false;
    }
    
    return true;
}

bool millerRabin(unsigned long long n,int iterations){
    // approx O( iterations * log(n) * log(n) )
    if(n==1) return false;
    if(n==2 || n==3) return true;
    if(n%2==0) return false;
    long long d=n-1;
    while(d%2==0) d/=2;
    return millerTest(n,d,iterations);
}

}

#endif

