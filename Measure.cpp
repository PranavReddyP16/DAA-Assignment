#include<bits/stdc++.h>
using namespace std;

template<typename T>
const T inf = 1e18;

//All global variables to be declared here
/*!
    This code is an implementation of the algorithm as shown in the paper
 */
int numberOfRectangles;         ///<The number of rectangles as given by the user
unordered_set<string> edgeType { "left", "right", "bottom", "top" };    ///<set of all possible edgetypes

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
        EdgeType(){ }
};

template<typename T = long long>
class Edge {
    public:
        T coord;
        Interval<T> interval;
        EdgeType side;

        Edge(T _coord, Interval<T> _interval, string _side) {
            coord = _coord;
            interval = _interval;
            side = _side;
        }
        Edge(string type) {
            EdgeType edgetype(type);
            side = edgetype;
        }
};

template<class T = long long>
class Stripe {
    public:
        Interval<T> xInterval;
        Interval<T> yInterval;
        set<Interval<T>> xUnion;

        Stripe(Interval<T> xInt, Interval<T> yInt, set<Interval<T>> xUn) {
            xInterval = xInt;
            yInterval = yInt;
            xUnion = xUn;
        }

        Stripe() {  };
};

template<typename T = long long>
class Partition {
    public:
        vector<T> ySet;
};

template<typename T = long long>
struct ReturnSet{
    
        vector<Interval<T>> L;
        vector<Interval<T>> R;
        vector<Point<T>> partition;
        set<Stripe<T>> stripes;
};

//The important functions go here

template<class T = long long>
set<Stripe<T>> RECTANGLE_DAC(set<Rectangle<T>> rect) {

    vector<Edge<T>> verticalEdges;

    for(auto rectangle : rect) {
        Edge<T> leftVerticalEdge(rectangle.xLeft, rectangle.yInterval, "left");
        Edge<T> rightVerticalEdge(rectangle.xRight, rectangle.yInterval, "right");

        verticalEdges.push_back(leftVerticalEdge);
        verticalEdges.push_back(rightVerticalEdge);
    }

    sort(verticalEdges.begin(), verticalEdges.end(), [&] (Edge<T> e1, Edge<T> e2) {
                return e1.coord < e2.coord;
            });
}

template<class T = long long>
struct ReturnSet<T> computeStripes (
        vector<Edge<T>> verticalEdges,
        Interval<T> x_ext,
        vector<Interval<T>> L,
        vector<Interval<T>> R,
        vector<T> partition,
        set<Stripe<T>> stripes) {
    
    if(verticalEdges.size() == 1) {

        Stripe<T> S;
        Edge<T> v = verticalEdges[0];

        if(v.side == "left") {
            L.push_back(v.interval);
            S = {x_ext, v.yInterval, {{v.coord, x_ext.upper}}};
        }

        else if(v.side == "right") {
            R.push_back(v.interval);
            S = {x_ext, v.yInterval, {{x_ext.lower, v.coord}}};
        }

        stripes.insert(S);
        partition = {-inf<T>, v.interval.lower, v.interval.upper, inf<T>};

        return {L,R,partition,stripes};
    }

    else {
        T xMedian = verticalEdges.size()/2;

        vector<Edge<T>> V1,V2;
        for(int i=0;i<verticalEdges.size();i++) {
            if(i<xMedian) V1.push_back(verticalEdges[i]);
            else V2.push_back(verticalEdges[i]);
        }

        vector<Interval<T>> L1,L2,R1,R2;
        vector<Point<T>> P1, P2;
        set<Stripe<T>> S1,S2;

        ReturnSet<T> leftSubProblem = computeStripes(V1, {x_ext.bottom, xMedian}, L1, R1, P1, S1);
        ReturnSet<T> rightSubProblem = computeStripes(V2, {xMedian,x_ext.top}, L2, R2, P2, S2);
    }
}

int main(int argc, char* argv[]) {
    cout<<"Enter the number of rectangles that you would like to input: ";
    cin>>numberOfRectangles;

    cout<<"Enter the co-ordinates of the upper left corner and the lower right corner respectively:"<<endl;
}
