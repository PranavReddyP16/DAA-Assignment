#include<bits/stdc++.h>
#include <regex>
using namespace std;
#define T long long

const T inf = 1e18;

//All global variables to be declared here
/*!
    This code is an implementation of the algorithm as shown in the paper
 */
long long numberOfRectangles;         ///<The number of rectangles as given by the user
set<string> edgeType { "left", "right", "bottom", "top" };    ///<set of all possible edgetypes

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

        bool operator < (Interval const &Int) const {
            return lower<Int.lower;
        }

        void print() {
            cout<<"Interval.lower and upper are: "<<lower<<" "<<upper<<endl;
        }
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

            xLeft = x1;
            xRight = x2;
            yLeft = y1;
            yRight = y2;
        }

        Rectangle(Interval X, Interval Y) {
            xLeft = min(X.lower, X.upper);
            xRight = max(X.lower, X.upper);
            yLeft = min(Y.lower, Y.upper);
            yRight = max(Y.lower, Y.upper);

            xInterval.lower = X.lower;
            xInterval.upper = X.upper;
            yInterval.lower = Y.lower;
            yInterval.upper = Y.upper;
        }

        Rectangle() { };

        bool operator < (Rectangle const &r) const {
            return xLeft < r.xLeft;
        }
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
            side.type = _side;
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

        bool operator < (const Stripe s) const {
            return yInterval.lower < s.yInterval.lower;
        }

        void print() {
            cout<<"x Interval of stripe is: ";
            xInterval.print();

            cout<<"y Interval of stripe is: ";
            yInterval.print();

            cout<<"The set of intervals in x_union of the stripe are: "<<endl;
            for(auto x : xUnion) {
                x.print();
            }
            cout<<endl;
        }
};

class Partition {
    public:
        set<T> ySet;

        void print() {
            cout<<"Partition points are: "<<endl;
            for(auto x : ySet) {
                cout<<x<<" ";
            }
            cout<<endl;
        }
};

struct ReturnSet{
    
        set<Interval> L;
        set<Interval> R;
        set<T> partition;
        set<Stripe> stripes;
};

set<Stripe> copyFunction(set<Stripe> S, set<T> P, set<T> P1, Interval x_int) {
    set<Stripe> ans;

    map<Interval, Stripe> reverseMap;
    vector<T> unionPartition;
    vector<T> partition;
    for(auto p : P) {
        unionPartition.push_back(p);
    }
    for(auto p1 : P1) {
        partition.push_back(p1);
    }

    for(auto s : S) {
        reverseMap[s.yInterval] = s;
    }


    int pointer1=0,pointer2=0;
    for(int i=0;i<(int)unionPartition.size()-1;i++) {
        Stripe s;
        s.yInterval = {unionPartition[i], unionPartition[i+1]};
        s.xInterval = x_int;

        pointer2++;
        while(pointer1+1 < partition.size() && unionPartition[pointer2] > partition[pointer1+1]) {
            pointer1++;
        }
        
        //TODO this might be the cause of an error check this later
        s.xUnion = reverseMap[{partition[pointer1], partition[pointer1+1]}].xUnion;

        ans.insert(s);
    }

    return ans;
}

set<Stripe> concat(set<Stripe> S1, set<Stripe> S2, set<T> P, Interval x_int) {
    vector<T> partition;
    for(auto p : P) {
        partition.push_back(p);
    }

    for(int i=0;i<(int)partition.size()-1;i++) {
        Stripe s;
        s.xInterval = x_int;
        s.yInterval = {partition[i], partition[i+1]};
    }
}

set<Interval> intervalIntersection(set<Interval> L1, set<Interval> R2) {
    map<Interval, int> cnt;
    for(auto l1 : L1) {
        cnt[l1]++;
    }

    set<Interval> ans;
    for(auto r2 : R2) {
        if(cnt[r2] > 0) ans.insert(r2);
    }

    return ans;
}

//The important functions go here
struct ReturnSet computeStripes (
        vector<Edge> verticalEdges,
        Interval x_ext,
        set<Interval> L,
        set<Interval> R,
        set<T> partition,
        set<Stripe> stripes) {
    
    //cerr<<"Size of vertical edges is: "<<verticalEdges.size()<<endl;
    if(verticalEdges.size() == 1) {
        Stripe S;
        Edge v = verticalEdges[0];

        stripes.insert({x_ext, {-inf, v.interval.lower}, {}});

        //cerr<<"the edgetype is: "<<v.side.type<<endl;
        if(v.side.type == "left") {
            L.insert(v.interval);
            S = {x_ext, v.interval, {{v.coord, x_ext.upper}}};
        }

        else if(v.side.type == "right") {
            R.insert(v.interval);
            S = {x_ext, v.interval, {{x_ext.lower, v.coord}}};
        }

        stripes.insert(S);
        stripes.insert({x_ext, {v.interval.upper, inf}, {}});

        partition = {-inf, v.interval.lower, v.interval.upper, inf};

        return {L,R,partition,stripes};
    }

    else {
        T xMedian = verticalEdges.size()/2;

        //splitting vertical edges into two equal sized groups
        vector<Edge> V1,V2;
        for(int i=0;i<verticalEdges.size();i++) {
            if(i<xMedian) V1.push_back(verticalEdges[i]);
            else V2.push_back(verticalEdges[i]);
        }

        //creating empty containers for the subtasks to fill up
        set<Interval> L1,L2,R1,R2;
        set<T> P1, P2;
        set<Stripe> S1,S2;
        set<Interval> LR;

        //Solving the subtasks
        ReturnSet leftSubProblem;
        ReturnSet rightSubProblem;
        if(V1.size() > 0) leftSubProblem = computeStripes(V1, {x_ext.lower, verticalEdges[xMedian].coord}, L1, R1, P1, S1);
        if(V2.size() > 0) rightSubProblem = computeStripes(V2, {verticalEdges[xMedian].coord,x_ext.upper}, L2, R2, P2, S2);

        //DEBUGGING
        
        
        //cerr<<"This is for the left sub problem\n";
        //cerr<<"This is L:\n";
        //for(auto l : leftSubProblem.L) {
        //    cerr<<l.lower<<" "<<l.upper<<endl;
        //}
        //cerr<<"This is R:\n";
        //for(auto r : leftSubProblem.R) {
        //    cerr<<r.lower<<" "<<r.upper<<endl;
        //}
        //cerr<<"This is partition:\n";
        //for(auto p : leftSubProblem.partition) {
        //    cerr<<p<<" ";
        //}
        //cerr<<endl;
        //
        //cerr<<"This is for the right sub problem\n";
        //cerr<<"This is L:\n";
        //for(auto l : rightSubProblem.L) {
        //    cerr<<l.lower<<" "<<l.upper<<endl;
        //}
        //cerr<<"This is R:\n";
        //for(auto r : rightSubProblem.R) {
        //    cerr<<r.lower<<" "<<r.upper<<endl;
        //}
        //cerr<<"This is partition:\n";
        //for(auto p : rightSubProblem.partition) {
        //    cerr<<p<<" ";
        //}
        //cerr<<endl;

        //END DEBUGGING

        //Using a function to find the intersection of L1 and R2 in O(nlogn) time complexity
        LR = intervalIntersection(leftSubProblem.L, rightSubProblem.R);

        //cerr<<"The elements in the set LR are: "<<endl;
        //for(auto x : LR) {
        //    cerr<<x.lower<<" "<<x.upper<<endl;
        //}

        //Inserting all the useful edges into L and R. Basically removing all the edges that belong to LR. Time complexity is O(nlogn).
        for(auto l1 : leftSubProblem.L) {
            if(LR.find(l1)==LR.end()) L.insert(l1);
        }
        for(auto l2 : rightSubProblem.L) {
            L.insert(l2);
        }
        for(auto r1 : leftSubProblem.R) {
            R.insert(r1);
        }
        for(auto r2 : rightSubProblem.R) {
            if(LR.find(r2)==LR.end()) R.insert(r2);
        }
        for(auto p1 : leftSubProblem.partition) {
            partition.insert(p1);
        }
        for(auto p2 : rightSubProblem.partition) {
            partition.insert(p2);
        }

        //more print statements
        //cerr<<"the elements in L are:"<<endl;
        //for(auto x : L) {
        //    cerr<<x.lower<<" "<<x.upper<<endl;
        //}
        //cerr<<"the elements in R are:"<<endl;
        //for(auto x : R) {
        //    cerr<<x.lower<<" "<<x.upper<<endl;
        //}
        //cerr<<"the elements in partition are:"<<endl;
        //for(auto x : partition) {
        //    cerr<<x<<" ";
        //}
        //cout<<endl;

        //Performing the copy function
        set<Stripe> sLeft, sRight;

        sLeft = copyFunction(leftSubProblem.stripes, partition, leftSubProblem.partition, {x_ext.lower, verticalEdges[xMedian].coord});
        sRight = copyFunction(rightSubProblem.stripes, partition, rightSubProblem.partition, {verticalEdges[xMedian].coord, x_ext.upper});

        //cout<<"xMedian is: "<<verticalEdges[xMedian].coord<<endl;
        //for(auto x : partition) {
        //    cout<<x<<" ";
        //}
        //cout<<endl;
    }
    return {L,R,partition,stripes};
}

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
        //cerr<<x.coord<<" "<<x.side.type<<endl;
    }

    //End DEBUGGING
    
    Interval x_ext = {-inf,inf};
    set<Interval> L,R;
    set<T> partition;
    set<Stripe> stripes;

    auto returnValue = computeStripes(verticalEdges, x_ext, L, R, partition, stripes);
    //cerr<<"This is L:\n";
    //for(auto l : returnValue.L) {
    //    cerr<<l.lower<<" "<<l.upper<<endl;
    //}
    //cerr<<"This is R:\n";
    //for(auto r : returnValue.R) {
    //    cerr<<r.lower<<" "<<r.upper<<endl;
    //}
    //cerr<<"This is partition:\n";
    //for(auto p : returnValue.partition) {
    //    cerr<<p<<" ";
    //}
    //cerr<<endl;

    //placeholder
    set<Stripe> temp;
    return temp;
}

int main(int argc, char* argv[]) {
    cout<<"Enter the number of rectangles that you would like to input: ";
    cin>>numberOfRectangles;

    cout<<"Enter the co-ordinates of the upper left corner and the lower right corner respectively:"<<endl;

    set<Rectangle> rect;
    for(int i=0;i<numberOfRectangles;i++) {
        int x1,x2,y1,y2;
        cin>>x1>>y1>>x2>>y2;

        Rectangle r(x1,y1,x2,y2);
        rect.insert(r);
    }

    RECTANGLE_DAC(rect);
}
