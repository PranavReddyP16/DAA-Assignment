#include<iostream>
#include"testlib.h"

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    int n = atoi(argv[1]);
    cout<<n<<" "<<argv[2]<<endl;

    int cur=0;
    for(int i=0;i<n;i++)
    {
        cout<<cur<<" "<<rnd.next(0,2*(i+1))<<endl;
        cur += rnd.next(1,3);
    }
}
