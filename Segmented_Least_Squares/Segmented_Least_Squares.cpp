#include "bits/stdc++.h"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;
using namespace std;
using namespace std::chrono;

#define ordered_set tree<int, null_type,less<int>, rb_tree_tag,tree_order_statistics_node_update>
const long long inf = 1e18;

const int mod = 1e9 + 7;
const int mod2 = 998244353;

high_resolution_clock::time_point curTime() { return high_resolution_clock::now(); }

class Point {
    public:
        long long x;
        long long y;
};

pair<long double, long double> calculateCoefficients(vector<Point> points, int i, int j) {
    long long n = i-j+1;

    long long xy = 0;
    long long x = 0;
    long long y = 0;
    long long xSquared = 0;
    for(int k=j;k<=i;k++) {
        xy += points[k].x*points[k].y;
        x += points[k].x;
        y += points[k].y;
        xSquared += points[k].x*points[k].x;
    }

    long double A = ((long double)n*xy - (long double)x*y) / ((long double)n*xSquared - x*x);

    long double B = ((long double)y - (long double)A*x) / ((long double) n);

    return {A, B};
}

long double findDistance(long long x, long long y, long double A, long double B) {

    return abs(A*x - y + B) / sqrt(A*A + 1);
}

signed main()
{
    ios_base :: sync_with_stdio(false);
    cin.tie(NULL);

    int n,C;
    cin>>n>>C;

    vector<Point> points(n);
    for(int i=0;i<n;i++) {
        cin>>points[i].x>>points[i].y;
    }

    auto startTime = curTime();

    sort(points.begin(), points.end(), [&](Point a, Point b) {
                return a.x < b.x;
            });

    vector<long double> dp(n+1, inf);
    dp[0] = 0;
    vector<int> minPrevIndex(n);
    for(int i=0;i<n;i++) {
        if(i>0) minPrevIndex[i] = i-1;
    }

    for(int i=1;i<=n;i++) {
        for(int j=1;j<=i;j++) {
            pair<long double, long double> coeff = calculateCoefficients(points, i-1, j-1);

            long double A = coeff.first;
            long double B = coeff.second;

            long double sumOfSquaredDistances=0;
            for(int k=j;k<=i;k++) {
                long double distance = findDistance(points[k-1].x, points[k-1].y, A, B);
                sumOfSquaredDistances += distance*distance;
            }

            long double error = sumOfSquaredDistances + C;
            if(i==j) error = C; 
            cout<<j<<" "<<i<<" "<<error<<endl;

            if(dp[j-1]+error < dp[i]) {
                dp[i] = dp[j-1]+error;
                minPrevIndex[i-1] = j-2;
            }
        }
    }

    //for(int i=1;i<=n;i++) cout<<dp[i]<<" ";
    //cout<<endl;
    //
    for(int i=0;i<n;i++) cout<<minPrevIndex[i]<<" ";
    cout<<endl;

    cout<<dp[n]<<endl;

    auto stopTime = curTime();
    auto duration = duration_cast<microseconds>(stopTime - startTime);
    cout<<"Program ran for "<<(long double)duration.count()/1e6<<" "<<"seconds"<<endl;
}
