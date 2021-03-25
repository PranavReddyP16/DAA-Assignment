#include"testlib.h"
#include<iostream>

using namespace std;

int main(int argc, char* argv[])
{
    registerGen(argc, argv, 1);

    int n = atoi(argv[1]);
    cout<<n<<endl;

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<2;j++)
        {
            int x = rnd.next(-2*n,2*n);
            int y = rnd.next(-2*n,2*n);

            if(x>y) swap(x,y);
            if(x==y) y+=3;
            cout<<x<<" "<<y<<" ";
        }
        cout<<endl;
    }
}
