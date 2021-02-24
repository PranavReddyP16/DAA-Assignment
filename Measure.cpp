#include<bits/stdc++.h>
#include <memory>
using namespace std;

int numberOfRectangles;

class Point
{
    public:
        long long x_coord;
        long long y_coord;
};

class Interval
{
    public:
        long long lower;
        long long upper;
};

class LineSegment
{
    public:
        Interval interval;
        long long coord;
};

class Rectangle
{
    public:
        long long xLeft;
        long long xRight;
        long long yLeft;
        long long yRight;

        Interval xInterval;
        Interval yInterval;
};

class EdgeType
{
    public:
        string type;
};

class Edge
{
    public:
        long long coord;
        Interval interval;
        EdgeType side;
};

class Stripe
{
    public:
        Interval xInterval;
        Interval yInterval;
        set<Interval> xUnion;
};

int main(int argc, char* argv[])
{
    cout<<"Enter the number of rectangles that you would like to input: ";
    cin>>numberOfRectangles;

    cout<<"Enter the co-ordinates of the upper left corner and the lower right corner respectively:"<<endl;
}
