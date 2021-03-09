#include<bits/stdc++.h>
using namespace std;
#define T long long

const T inf = 1e18;

//All global variables to be declared here
/*!
    This code is an implementation of the algorithm as shown in the paper
 */
long long numberOfRectangles;         ///<The number of rectangles as given by the user
unordered_set<string> edgeType { "left", "right", "bottom", "top" };    ///<set of all possible edgetypes

//All utility functions that the classes use to be declared here

//All classes to be declared here
class Point {
    public:
        T x_coord;
        T y_coord;
};

class Interval {
    public:
        T lower;
        T upper;
};

class LineSegment {
    public:
        Interval interval;
        T coord;
};

class Rectangle {
    public:
        T xLeft;
        T xRight;
        T yLeft;
        T yRight;

        Interval xInterval;
        Interval yInterval;

        Rectangle(T x1, T y1, T x2, T y2) {
            xInterval.upper = max(x1,x2);
            xInterval.lower = min(x1,x2);
            yInterval.upper = max(y1,y2);
            yInterval.lower = min(y1,y2);
        }

        Rectangle(Interval X, Interval Y) {
            xLeft = min(X.lower, X.upper);
            xRight = max(X.lower, X.upper);
            yLeft = min(Y.lower, Y.upper);
            yRight = max(Y.lower, Y.upper);
        }

        Rectangle() { };
};

class EdgeType {
    public:
        string type;

        EdgeType(string s) {
            assert(edgeType.find(s)!=edgeType.end());
        }
        EdgeType(){ }
};

class Edge {
    public:
        T coord;
        Interval interval;
        EdgeType side;

        Edge(T _coord, Interval _interval, string _side) {
            coord = _coord;
            interval = _interval;
            side = _side;
        }
        Edge(string type) {
            EdgeType edgetype(type);
            side = edgetype;
        }
};

class Stripe {
    public:
        Interval xInterval;
        Interval yInterval;
        set<Interval> xUnion;

        Stripe(Interval xInt, Interval yInt, set<Interval> xUn) {
            xInterval = xInt;
            yInterval = yInt;
            xUnion = xUn;
        }

        Stripe() {  };
};

class Partition {
    public:
        set<T> ySet;
};

struct ReturnSet{
    
        set<Interval> L;
        set<Interval> R;
        set<Point> partition;
        set<Stripe> stripes;
};

//set<Stripe> copy(set<Stripe> S, set<T> P, set<T> P1, Interval x_int) {
//    set<Stripe> ans;
//
//    map<Interval, Stripe> reverseMap;
//    vector<T> unionPartition;
//    vector<T> partition;
//    for(auto p : P) {
//        unionPartition.push_back(p);
//    }
//    for(auto p1 : P1) {
//        partition.push_back(p1);
//    }
//
//    for(auto s : S) {
//        reverseMap[s.yInterval] = s;
//    }
//
//
//    int pointer1=0,pointer2=0;
//    for(int i=0;i<(int)unionPartition.size()-1;i++) {
//        Stripe s;
//        s.yInterval = {partition[i], partition[i+1]};
//        s.xInterval = x_int;
//
//        while((pointer1+1 < partition.size() && pointer2 < unionPartition.size()) && unionPartition[pointer2] > partition[pointer1+1]) {
//            pointer1++;
//        }
//        pointer2++;
//
//        s.xUnion = reverseMap[{partition[pointer1], partition[pointer1+1]}].xUnion;
//
//        ans.insert(s);
//    }
//
//    return ans;
//}
//
//set<Stripe> concat(set<Stripe> S1, set<Stripe> S2, set<T> P, Interval x_int) {
//    vector<T> partition;
//    for(auto p : P) {
//        partition.push_back(p);
//    }
//
//    for(int i=0;i<(int)partition.size()-1;i++) {
//        Stripe s;
//        s.xInterval = x_int;
//        s.yInterval = {partition[i], partition[i+1]};
//    }
//}

set<Interval> intervalIntersection(vector<Interval> L1, vector<Interval> R2) {
    map<Interval, int> cnt;
    for(int i=0;i<(int)L1.size();i++) {
        cnt[L1[i]]++;
    }

    set<Interval> ans;
    for(int i=0;i<(int)R2.size();i++) {
        if(cnt[R2[i]]>0) ans.insert(R2[i]);
    }

    return ans;
}

//The important functions go here

set<Stripe> RECTANGLE_DAC(set<Rectangle> rect) {

    vector<Edge> verticalEdges;

    for(auto rectangle : rect) {
        Edge leftVerticalEdge(rectangle.xLeft, rectangle.yInterval, "left");
        Edge rightVerticalEdge(rectangle.xRight, rectangle.yInterval, "right");

        verticalEdges.push_back(leftVerticalEdge);
        verticalEdges.push_back(rightVerticalEdge);
    }

    sort(verticalEdges.begin(), verticalEdges.end(), [&] (Edge e1, Edge e2) {
                return e1.coord < e2.coord;
            });

    //DEBUGGING
    for(auto x : verticalEdges) {
        cout<<x.coord<<endl;
    }

    //placeholder
    set<Stripe> temp;
    return temp;
}

//struct ReturnSet computeStripes (
//        vector<Edge> verticalEdges,
//        Interval x_ext,
//        set<Interval> L,
//        set<Interval> R,
//        set<T> partition,
//        set<Stripe> stripes) {
//    
//    if(verticalEdges.size() == 1) {
//
//        Stripe S;
//        Edge v = verticalEdges[0];
//
//        stripes.insert({x_ext, {-inf, v.interval.lower}, {}});
//
//        if(v.side == "left") {
//            L.insert(v.interval);
//            S = {x_ext, v.yInterval, {{v.coord, x_ext.upper}}};
//        }
//
//        else if(v.side == "right") {
//            R.insert(v.interval);
//            S = {x_ext, v.yInterval, {{x_ext.lower, v.coord}}};
//        }
//
//        stripes.insert(S);
//        stripes.insert({x_ext, {v.interval.upper, inf}, {}});
//
//        partition = {-inf, v.interval.lower, v.interval.upper, inf};
//
//        return {L,R,partition,stripes};
//    }
//
//    else {
//        T xMedian = verticalEdges.size()/2;
//
//        //splitting vertical edges into two equal sized groups
//        vector<Edge> V1,V2;
//        for(int i=0;i<verticalEdges.size();i++) {
//            if(i<=xMedian) V1.push_back(verticalEdges[i]);
//            else V2.push_back(verticalEdges[i]);
//        }
//
//        //creating empty containers for the subtasks to fill up
//        set<Interval> L1,L2,R1,R2;
//        set<T> P1, P2;
//        set<Stripe> S1,S2;
//        set<Interval> LR;
//
//        //Solving the subtasks
//        ReturnSet leftSubProblem = computeStripes(V1, {x_ext.bottom, xMedian}, L1, R1, P1, S1);
//        ReturnSet rightSubProblem = computeStripes(V2, {xMedian,x_ext.top}, L2, R2, P2, S2);
//
//        //Using a function to find the intersection of L1 and R2 in O(nlogn) time complexity
//        LR = intervalIntersection(L1, R2);
//
//        //Inserting all the useful edges into L and R. Basically removing all the edges that belong to LR. Time complexity is O(nlogn).
//        for(auto l1 : L1) {
//            if(LR.find(l1)==LR.end()) L.insert(l1);
//        }
//        for(auto l2 : L2) {
//            L.insert(l2);
//        }
//        for(auto r1 : R1) {
//            R.insert(r1);
//        }
//        for(auto r2 : R2) {
//            if(LR.find(r2)==LR.end()) R.insert(r2);
//        }
//        for(auto p1 : P1) {
//            partition.insert(p1);
//        }
//        for(auto p2 : P2) {
//            partition.insert(p2);
//        }
//
//        //Performing the copy function
//        set<Stripe> sLeft, sRight;
//    }
//}

int main(int argc, char* argv[]) {
    cout<<"Enter the number of rectangles that you would like to input: ";
    //cin>>numberOfRectangles;

    cout<<"Enter the co-ordinates of the upper left corner and the lower right corner respectively:"<<endl;

    //set<Rectangle> rect;
    for(int i=0;i<numberOfRectangles;i++) {
        int x1,x2,y1,y2;
        cin>>x1>>y1>>x2>>y2;

        //Rectangle<long long> r(x1,x2,y1,y2);
        //rect.insert(r);
    }

    //RECTANGLE_DAC(rect);
}
