#include<bits/stdc++.h>
using namespace std;

//All global variables to be declared here
int numberOfRectangles;

unordered_set<string> edgeType { "left", "right", "bottom", "top" };

//All utility functions that the classes use to be declared here
template<class Container, class Contained>
bool contains(Container A, Contained B) {

    if(A.find(B) != A.end()) {
        return true;
    }
    else {
        return false;
    }
}

//All classes to be declared here
template<typename T = long long>
class Point {
    public:
        T x_coord;
        T y_coord;
};

template<typename T = long long>
class Interval {
    public:
        T lower;
        T upper;
};

template<typename T = long long>
class LineSegment {
    public:
        Interval<T> interval;
        T coord;
};

template<typename T = long long>
class Rectangle {
    public:
        T xLeft;
        T xRight;
        T yLeft;
        T yRight;

        Interval<T> xInterval;
        Interval<T> yInterval;

        Rectangle(T x1, T y1, T x2, T y2) {
            xInterval.upper = max(x1,x2);
            xInterval.lower = min(x1,x2);
            yInterval.upper = max(y1,y2);
            yInterval.lower = min(y1,y2);
        }

        Rectangle(Interval<T> X, Interval<T> Y) {
            xLeft = min(X.lower, X.upper);
            xRight = max(X.lower, X.upper);
            yLeft = min(Y.lower, Y.upper);
            yRight = max(Y.lower, Y.upper);
        }
};

class EdgeType {
    public:
        string type;

        EdgeType(string s) {
            assert(contains(edgeType, s));
        }
};

template<typename T = long long>
class Edge {
    public:
        T coord;
        Interval<T> interval;
        EdgeType side;
};

template<class T = long long>
class Stripe {
    public:
        Interval<T> xInterval;
        Interval<T> yInterval;
        set<Interval<T>> xUnion;
};

template<typename T = long long>
class Partition {
    public:
        vector<T> ySet;
};

//The important functions go here
set<Stripe<>> RECTANGLE_DAC(set<Rectangle<>> rect) {
    for(auto rectangle : rect) {
        //TODO seperate the given rectangles into vertical edges
    }
}

int main(int argc, char* argv[]) {
    cout<<"Enter the number of rectangles that you would like to input: ";
    cin>>numberOfRectangles;

    cout<<"Enter the co-ordinates of the upper left corner and the lower right corner respectively:"<<endl;
}
