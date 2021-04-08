/** @file */

//Pranav Reddy Pesaladinne - 2018A7PS0238H
//Dhruv Adlakha - 2018A7PS0303H
//Donkada Vishal Dheeraj - 2018A7PS0239H
//Pranay Tarigopula - 2018A7PS0237H

#include "bits/stdc++.h"
#include <cmath>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include<sys/time.h>
#include<sys/resource.h>

using namespace __gnu_pbds;
using namespace std;
using namespace std::chrono;

#define ordered_set tree<int, null_type,less<int>, rb_tree_tag,tree_order_statistics_node_update>
const long double inf = 1e18;

const int mod = 1e9 + 7;
const int mod2 = 998244353;

high_resolution_clock::time_point curTime() { return high_resolution_clock::now(); }

///
///Point (x,y) where x and y are integers
///

class Point {
    public:
        long long x;
        long long y;
};

/**
 * Given a contiguous subsegment of points, finds the coefficients of line of best fit for those points
 * @param Vector of points (passed by reference to reduce time complexity due to copying)
 * @param Start index of subsegment
 * @param Ending index of subsegment
 * @return Pair of decimals containing x coefficient and constant term of the line equation respectively
*/
pair<long double, long double> calculateCoefficients(vector<Point> &points, int i, int j) {
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

    long double A;
    A = ((long double)n*xy - (long double)x*y) / ((long double)n*xSquared - x*x);
    if(isnan(A)) A = inf;

    long double B = ((long double)y - (long double)A*x) / ((long double) n);

    return {A, B};
}

/**
 * Given a point and a line, find the square of distance to that line
 * @param X coordinate of the point
 * @param Y coordinate of the point
 * @param X coefficient of the line equation
 * @param Constant term of the line equation
 * @return Square of distance
*/
long double findDistance(long long x, long long y, long double A, long double B) {

    return ((long double)A*x - y + B)*((long double)A*x - y + B);
}

/**
 * Main driver function
*/
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
                sumOfSquaredDistances += distance;
            }

            long double error = sumOfSquaredDistances + C;
            //cout<<fixed<<setprecision(10)<<j<<" "<<i<<" "<<A<<" "<<B<<endl;

            if(dp[j-1]+error < dp[i]) {
                dp[i] = dp[j-1]+error;
                minPrevIndex[i-1] = j-2;
            }
        }
    }

    //for(int i=1;i<=n;i++) cout<<dp[i]<<" ";
    //cout<<endl;
    
    ofstream outputFile;
    outputFile.open("data.txt");

    outputFile<<n<<endl;
    for(int i=0;i<n;i++) {
        outputFile<<points[i].x<<" "<<points[i].y<<endl;
    }

    int pos = n-1;
    int cnt=1;
    vector<int> final_indices = {n-1};
    while(minPrevIndex[pos]!=-1) {
        final_indices.push_back(minPrevIndex[pos]);
        //cout<<minPrevIndex[pos]<<" ";
        cnt++;
        pos = minPrevIndex[pos];
    }
    final_indices.push_back(-1);
    //outputFile<<-1<<endl;

    outputFile<<cnt<<endl;
    for(int i=0;i<(int)final_indices.size()-1;i++) {
        pair<long double, long double> coeff = calculateCoefficients(points, final_indices[i], final_indices[i+1]+1);

        long double A = coeff.first;
        long double B = coeff.second;

        outputFile<<points[final_indices[i+1]+1].x<<" "<<A*(points[final_indices[i+1]+1]).x+B<<" "<<points[final_indices[i]].x<<" "<<A*(points[final_indices[i]]).x+B
            <<endl;
    }

    outputFile<<dp[n]<<endl;



    cout<<"line count is: "<<cnt<<endl;
    cout<<fixed<<setprecision(10)<<dp[n]<<endl;

    int who = RUSAGE_SELF;
    struct rusage usage;
    getrusage(who, &usage);

    auto stopTime = curTime();
    auto duration = duration_cast<microseconds>(stopTime - startTime);
    cout<<"Program ran for "<<(long double)duration.count()/1e6<<" "<<"seconds"<<endl;
    cout<<"Program took up a maximum memory of: "<<usage.ru_idrss+usage.ru_isrss<<"Kb"<<endl;
}
