#include<iostream>
#include"testlib.h"

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    int n = atoi(argv[1]);
    cout<<n<<" "<<argv[2]<<endl;

    int cur=0;
    int l = rnd.next(0,n/2);
    int r = l+rnd.next(10,20);
    for(int i=0;i<n;i++)
    {
        cout<<cur<<" "<<rnd.next(l,r)<<endl;
        cur += rnd.next(1,3);
    }
}
