/** @file */
#include <bits/stdc++.h>
#include <regex>
#include <streambuf>
using namespace std;
#define T long long

const T inf = 1e6 + 5;

long long numberOfRectangles;
set<string> edgeType{"left", "right", "bottom", "top"};
set<string> lru{"left", "right", "undefined"};

///
///Co-ordinate (x,y) where x and y are integers
///
class Point
{
public:
    T x_coord;
    T y_coord;
};

///
///Tree node containing members x coordinate, side type, left child, and right child.
///
struct ctree
{
    T x;
    string side;
    ctree *left_child;
    ctree *right_child;
};

///
///Interval containing lower coordinate, upper coordinate, and rectangle id for which it belongs to
/// comparison operator has also been overloaded to allow for use in sets and maps
///
class Interval
{
public:
    T lower;
    T upper;
    T rect_number;

    bool operator<(Interval const &Int) const
    {
        if (lower != Int.lower)
            return lower < Int.lower;
        else if (upper != Int.upper)
            return upper < Int.upper;
        else
            return rect_number < Int.rect_number;
    }

    void print()
    {
        cout << "Interval.lower and upper are: " << lower << " " << upper << endl;
    }
};

///
///Line Segment containing coordinate and Interval
///comparison operator has been overloaded for use in sets and maps
///
class LineSegment
{
public:
    Interval interval;
    T coord;

    void print()
    {
        cout << "Interval for the line segment is: ";
        interval.print();
        cout << "coord of the line segment is: " << coord << endl;
    }

    //overloading operator to use in sets
    bool operator<(LineSegment const &l) const
    {
        if (coord != l.coord)
            return coord < l.coord;
        else if (interval.lower != l.interval.lower)
            return interval.lower < l.interval.lower;
        else
            return interval.upper < l.interval.upper;
    }
};

///
///Rectangle with the four x and y coordinates as members as well as rectangle id number
/// To constructors are provided for use in different cases
///
class Rectangle
{
public:
    T xLeft;
    T xRight;
    T yLeft;
    T yRight;
    T rectNumber;

    Interval xInterval;
    Interval yInterval;

    Rectangle(T x1, T y1, T x2, T y2, T rectKey)
    {
        xInterval.upper = max(x1, x2);
        xInterval.lower = min(x1, x2);
        xInterval.rect_number = rectKey;
        yInterval.upper = max(y1, y2);
        yInterval.lower = min(y1, y2);
        yInterval.rect_number = rectKey;

        xLeft = x1;
        xRight = x2;
        yLeft = y1;
        yRight = y2;

        rectNumber = rectKey;
    }

    Rectangle(Interval X, Interval Y, T rectKey)
    {
        xLeft = min(X.lower, X.upper);
        xRight = max(X.lower, X.upper);
        yLeft = min(Y.lower, Y.upper);
        yRight = max(Y.lower, Y.upper);

        xInterval.lower = X.lower;
        xInterval.upper = X.upper;
        xInterval.rect_number = rectKey;
        yInterval.lower = Y.lower;
        yInterval.upper = Y.upper;
        yInterval.rect_number = rectKey;

        rectNumber = rectKey;
    }

    Rectangle(){};

    bool operator<(Rectangle const &r) const
    {
        if (xLeft != r.xLeft)
            return xLeft < r.xLeft;
        else if (xRight != r.xRight)
            return xRight < r.xRight;
        else
            return rectNumber < r.rectNumber;
    }
};

///
///EdgeType can be left/right/bottom/top
///
class EdgeType
{
public:
    string type;

    EdgeType(string s)
    {
        assert(edgeType.find(s) != edgeType.end());
    }
    EdgeType() {}
};

///
///Edge can be represented using coordinate, interval and direction
///
class Edge
{
public:
    T coord;
    Interval interval;
    EdgeType side;

    Edge(T _coord, Interval _interval, string _side)
    {
        coord = _coord;
        interval = _interval;
        side.type = _side;
    }
    Edge(string type)
    {
        EdgeType edgetype(type);
        side = edgetype;
    }
};
///
///Stripe is represented using xInterval, yInterval,x coordinates where the rectangles are present (X_Union) and ctree representing the end points of X_Union
///
class Stripe
{
public:
    Interval xInterval;
    Interval yInterval;
    set<Interval> xUnion;
    ctree *tree;

    Stripe(Interval xInt, Interval yInt, set<Interval> xUn, ctree *t)
    {
        xInterval = xInt;
        yInterval = yInt;
        xUnion = xUn;
        tree = t;
    }

    Stripe(){};

    bool operator<(const Stripe s) const
    {
        if (yInterval.lower != s.yInterval.lower)
            return yInterval.lower < s.yInterval.lower;
        else
            return yInterval.upper < s.yInterval.upper;
    }

    void print()
    {
        cout << "x Interval of stripe is: ";
        xInterval.print();

        cout << "y Interval of stripe is: ";
        yInterval.print();

        cout << "The set of intervals in x_union of the stripe are: " << endl;
        for (auto x : xUnion)
        {
            x.print();
        }
        cout << endl;
    }
};

///
///Partition represents a set of y coordinates
///
class Partition
{
public:
    set<T> ySet;

    void print()
    {
        cout << "Partition points are: " << endl;
        for (auto x : ySet)
        {
            cout << x << " ";
        }
        cout << endl;
    }
};

///
///Custom struct type declared to use in computeStripes function as a way of returning multiple objects at once
///
struct ReturnSet
{

    set<Interval> L;
    set<Interval> R;
    set<T> partition;
    set<Stripe> stripes;
};

/**
 * Finds the intersection of sets of intervals to be used by the computeStripes function
 * @param Set of intervals L1
 * @param Set of intervals R2
 * @return Set of intervals ans
*/
set<Interval> intervalIntersection(set<Interval> L1, set<Interval> R2)
{
    map<T, T> cnt;
    for (auto l1 : L1)
    {
        cnt[l1.rect_number]++;
    }

    set<Interval> ans;
    for (auto r2 : R2)
    {
        if (cnt[r2.rect_number] > 0)
            ans.insert(r2);
    }

    return ans;
}

/**
 * Implements the copy function as described in the algorithm by dividing the existing stripes on both sides and setting the X_Union accordingly
 * @param Set of Stripes S
 * @param Set of integers P
 * @param Set of integers P1
 * @param Interval x_int
 * @return Set of Stripes ans
*/
set<Stripe> copyFunction(set<Stripe> S, set<T> P, set<T> P1, Interval x_int)
{
    set<Stripe> ans;

    map<pair<T, T>, Stripe> reverseMap;
    vector<T> unionPartition;
    vector<T> partition;
    for (auto p : P)
    {
        unionPartition.push_back(p);
    }
    for (auto p1 : P1)
    {
        partition.push_back(p1);
    }

    for (auto s : S)
    {
        reverseMap[make_pair(s.yInterval.lower, s.yInterval.upper)] = s;
    }

    T pointer1 = 0, pointer2 = 0;
    for (T i = 0; i < (T)unionPartition.size() - 1; i++)
    {
        Stripe s;
        s.yInterval = {unionPartition[i], unionPartition[i + 1]};
        s.xInterval = x_int;

        pointer2++;
        while (pointer1 + 1 < (T)partition.size() && unionPartition[pointer2] > partition[pointer1 + 1])
        {
            pointer1++;
        }

        s.xUnion = reverseMap[{partition[pointer1], partition[pointer1 + 1]}].xUnion; //(D)
        s.tree = reverseMap[{partition[pointer1], partition[pointer1 + 1]}].tree;

        ans.insert(s);
    }

    return ans;
}

/**
 * Implements the blacken function as described in the algorithm by taking the stripes for which edges in it don't have a partner in the opposite stripe and setting its X_Union to empty
 * @param Set of Stripes S
 * @param Set of Intervals J
 * @return Set of Stripes ans
*/
set<Stripe> blacken(set<Stripe> S, set<Interval> J)
{
    vector<T> sVector, jVector;
    vector<Stripe> sTemp;

    for (auto x : S)
    {
        sTemp.push_back(x);
    }

    for (auto x : S)
    {
        sVector.push_back(x.yInterval.lower);
        sVector.push_back(x.yInterval.upper);
    }

    for (auto x : J)
    {
        jVector.push_back(x.lower);
        jVector.push_back(x.upper);
    }

    Interval x_ext;
    if ((T)S.size() > 0)
        x_ext = (*S.begin()).xInterval;

    set<Stripe> ans;

    T p1 = 0, p2 = 0;
    while (p1 < (T)jVector.size() && p2 < (T)sVector.size())
    {
        if (sVector[p2] >= jVector[p1] && sVector[p2 + 1] <= jVector[p1 + 1])
        {
            sTemp[p2 / 2].xUnion.clear();
            sTemp[p2 / 2].xUnion.insert(x_ext);

            sTemp[p2 / 2].tree = NULL; //(E)
            p2 += 2;
        }

        else if (sVector[p2] <= jVector[p1])
        {
            p2 += 2;
        }
        else if (sVector[p2] >= jVector[p1 + 1])
        {
            p1 += 2;
        }
    }

    for (auto x : sTemp)
    {
        ans.insert(x);
    }

    return ans;
}

/**
 * Implements the concat function as described in the algorithm by concatenating the stripes in both subproblems
 * @param S1
 * @param S2
 * @param P
 * @param x_int
 * @return ans
*/
set<Stripe> concat(set<Stripe> S1, set<Stripe> S2, set<T> P, Interval x_int)
{
    vector<T> partition;
    T xMedian;
    for (auto p : P)
    {
        partition.push_back(p);
    }

    vector<Stripe> blackenedsLeftVector;
    vector<Stripe> blackenedsRightVector;

    for (auto x : S1)
    {
        blackenedsLeftVector.push_back(x);
        xMedian = x.xInterval.upper;
    }
    for (auto x : S2)
    {
        blackenedsRightVector.push_back(x);
    }

    set<Stripe> ans;

    for (T i = 0; i < (T)partition.size() - 1; i++)
    {
        Stripe s;
        s.xInterval = x_int;
        s.yInterval = {partition[i], partition[i + 1]};
        for (auto x : blackenedsLeftVector[i].xUnion)
        {
            s.xUnion.insert(x);
        }
        for (auto x : blackenedsRightVector[i].xUnion)
        {
            s.xUnion.insert(x);
        }

        ctree *tree = new ctree;

        tree->x = xMedian;
        tree->side = "undefined";
        tree->left_child = blackenedsLeftVector[i].tree;
        tree->right_child = blackenedsRightVector[i].tree;

        s.tree = tree; //(F)
        //cout<<s.tree->x<<endl;

        ans.insert(s);
    }

    return ans;
}

/**
 * Computes the final area of the rectangles
 * @param Set of Stripes S
 * @return Integer ans
*/
T calculateMeasure(set<Stripe> S)
{
    T ans = 0;
    for (auto x : S)
    {
        for (auto y : x.xUnion)
        {
            T temp = y.upper - y.lower;
            temp *= (x.yInterval.upper - x.yInterval.lower);

            ans += temp;
        }
    }

    return ans;
}

/**
 * STRIPES function according to the algorithm in which there are Divide, Conquer, and Merge steps
 * @param Vector of Edges verticalEdges
 * @param Interval x_ext
 * @param Set of Intervals L
 * @param Set of Intervals R
 * @param Set of Integers partition
 * @param Set of Stripes stripes
 * @return ReturnSet object
*/
struct ReturnSet computeStripes(
    vector<Edge> verticalEdges,
    Interval x_ext,
    set<Interval> L,
    set<Interval> R,
    set<T> partition,
    set<Stripe> stripes)
{

    if ((T)verticalEdges.size() == 1)
    {
        Stripe S;
        Edge v = verticalEdges[0];

        stripes.insert({x_ext, {-inf, v.interval.lower}, {}, NULL}); //(A)

        if (v.side.type == "left")
        {
            L.insert(v.interval);
            ctree *tree = new ctree;

            tree->x = v.coord;
            tree->side = "left";
            tree->left_child = NULL;
            tree->right_child = NULL;

            S = {x_ext, v.interval, {{v.coord, x_ext.upper}}, tree}; //(B)
        }

        else if (v.side.type == "right")
        {
            R.insert(v.interval);

            ctree *tree = new ctree;
            tree->x = v.coord;
            tree->side = "right";
            tree->left_child = NULL;
            tree->right_child = NULL;

            S = {x_ext, v.interval, {{x_ext.lower, v.coord}}, tree}; //(C)
        }

        //cout << S.tree->x << endl;
        stripes.insert(S);

        stripes.insert({x_ext, {v.interval.upper, inf}, {}, NULL});

        partition = {-inf, v.interval.lower, v.interval.upper, inf};

        return {L, R, partition, stripes};
    }

    else
    {
        T xMedian = verticalEdges.size() / 2;

        //splitting vertical edges into two equal sized groups
        vector<Edge> V1, V2;
        for (T i = 0; i < (T)verticalEdges.size(); i++)
        {
            if (i < xMedian)
                V1.push_back(verticalEdges[i]);
            else
                V2.push_back(verticalEdges[i]);
        }

        //creating empty containers for the subtasks to fill up
        set<Interval> L1, L2, R1, R2;
        set<T> P1, P2;
        set<Stripe> S1, S2;
        set<Interval> LR;

        //Solving the subtasks
        ReturnSet leftSubProblem;
        ReturnSet rightSubProblem;
        if (V1.size() > 0)
            leftSubProblem = computeStripes(V1, {x_ext.lower, verticalEdges[xMedian].coord}, L1, R1, P1, S1);
        if (V2.size() > 0)
            rightSubProblem = computeStripes(V2, {verticalEdges[xMedian].coord, x_ext.upper}, L2, R2, P2, S2);

        //Using a function to find the intersection of L1 and R2 in O(nlogn) time complexity
        LR = intervalIntersection(leftSubProblem.L, rightSubProblem.R);

        //Inserting all the useful edges into L and R. Basically removing all the edges that belong to LR. Time complexity is O(nlogn).
        for (auto l1 : leftSubProblem.L)
        {
            if (LR.find(l1) == LR.end())
                L.insert(l1);
        }
        for (auto l2 : rightSubProblem.L)
        {
            L.insert(l2);
        }
        for (auto r1 : leftSubProblem.R)
        {
            R.insert(r1);
        }
        for (auto r2 : rightSubProblem.R)
        {
            if (LR.find(r2) == LR.end())
                R.insert(r2);
        }
        for (auto p1 : leftSubProblem.partition)
        {
            partition.insert(p1);
        }
        for (auto p2 : rightSubProblem.partition)
        {
            partition.insert(p2);
        }

        //Performing the copy function
        set<Interval> R2minusLR, L1minusLR;
        for (auto x : rightSubProblem.R)
        {
            if (LR.find(x) == LR.end())
            {
                R2minusLR.insert(x);
            }
        }
        for (auto x : leftSubProblem.L)
        {
            if (LR.find(x) == LR.end())
            {
                L1minusLR.insert(x);
            }
        }

        set<Stripe> sLeft, sRight;

        sLeft = copyFunction(leftSubProblem.stripes, partition, leftSubProblem.partition, {x_ext.lower, verticalEdges[xMedian].coord});
        sRight = copyFunction(rightSubProblem.stripes, partition, rightSubProblem.partition, {verticalEdges[xMedian].coord, x_ext.upper});

        set<Stripe> blackenedsLeft, blackenedsRight;

        blackenedsLeft = blacken(sLeft, R2minusLR);
        blackenedsRight = blacken(sRight, L1minusLR);
        assert(blackenedsLeft.size() == blackenedsRight.size());

        stripes = concat(blackenedsLeft, blackenedsRight, partition, x_ext);
        return {L, R, partition, stripes};
    }
}

/**
 * Provides a basic setup for divide-and-conquer algorithm computeStripes(STRIPES)
 * @param Set of Rectangles rect 
 * @return Set of Stripes returnValue.stripes
 * 
*/
set<Stripe> RECTANGLE_DAC(set<Rectangle> rect)
{

    vector<Edge> verticalEdges;

    for (auto rectangle : rect)
    {
        Edge leftVerticalEdge(rectangle.xLeft, rectangle.yInterval, "left");
        Edge rightVerticalEdge(rectangle.xRight, rectangle.yInterval, "right");

        verticalEdges.push_back(leftVerticalEdge);
        verticalEdges.push_back(rightVerticalEdge);
    }

    sort(verticalEdges.begin(), verticalEdges.end(), [&](Edge e1, Edge e2) {
        if (e1.coord != e2.coord)
            return e1.coord < e2.coord;
        else
        {
            if (e1.side.type == "left" && e2.side.type == "right")
                return true;
            else
                return false;
        }
    });

    Interval x_ext = {-inf, inf};
    set<Interval> L, R;
    set<T> partition;
    set<Stripe> stripes;

    auto returnValue = computeStripes(verticalEdges, x_ext, L, R, partition, stripes);

    return returnValue.stripes;
}

/**
 * Finds the leaves of a ctree and pushes it into an array
 * @param pointer to ctree u
 * @param Vector of pointers to a ctree leaves (passed by reference)
 * 
*/
void dfs(ctree *u, vector<ctree *> &leaves)
{
    if (u == NULL)
    {
        return;
    }
    if (u->left_child == NULL && u->right_child == NULL && u->side != "undefined")
    {
        leaves.push_back(u);
        return;
    }

    if (u->left_child != NULL)
    {
        dfs(u->left_child, leaves);
    }
    if (u->right_child != NULL)
    {
        dfs(u->right_child, leaves);
    }
}

/**
 * Finds the free part of an intersection intervals of an interval with the X_Union of a stripe
 * @param Interval h
 * @param Stripe s
 * @return Set of Intervals ans
 * 
*/
set<Interval> freeQuery(Interval h, Stripe s)
{
    set<Interval> ans;
    vector<ctree *> leaves;

    ctree *tempTree = new ctree;
    tempTree->x = -inf;
    tempTree->left_child = NULL;
    tempTree->right_child = NULL;
    tempTree->side = "right";

    leaves.push_back(tempTree);

    //cout<<"s.tree is: "<<s.tree->side<<endl;
    dfs(s.tree, leaves);

    ctree *tempTree2 = new ctree;
    tempTree2->x = inf;
    tempTree2->left_child = NULL;
    tempTree2->right_child = NULL;
    tempTree2->side = "left";

    leaves.push_back(tempTree2);

    //cout << "Leaves are: " << endl;
    //for (auto x : leaves)
    //{
    //    cout << x->x << " ";
    //}
    //cout << endl;

    for (int i = 0; i < (T)leaves.size(); i++)
    {
        if (i > 0 && h.lower >= leaves[i - 1]->x && h.upper <= leaves[i]->x && leaves[i - 1]->side == "right" && leaves[i]->side == "left")
        {
            ans.insert(h);
            break;
        }

        else if (i > 0 && leaves[i - 1]->x >= h.lower && leaves[i]->x <= h.upper && leaves[i - 1]->side == "right" && leaves[i]->side == "left")
        {
            ans.insert({leaves[i - 1]->x, leaves[i]->x});
        }

        else if (i > 0 && h.lower >= leaves[i - 1]->x && h.lower <= leaves[i]->x && leaves[i]->side == "left" && h.upper >= leaves[i]->x)
        {
            ans.insert({h.lower, leaves[i]->x});
        }

        else if (i > 0 && h.upper >= leaves[i - 1]->x && h.upper <= leaves[i]->x && leaves[i - 1]->side == "right" && h.lower <= leaves[i - 1]->x)
        {
            ans.insert({leaves[i - 1]->x, h.upper});
        }
    }

    return ans;
}

/**
 * Given an edge, matches the corresponding Stripe to pass to freeQuery
 * @param Edge h
 * @param Set of Stripes S
 * @return Set of Line Segments ans
 * 
*/
set<LineSegment> contourPieces(Edge h, set<Stripe> S)
{
    set<LineSegment> ans;

    Stripe s;
    if (h.side.type == "bottom")
    {
        for (auto x : S)
        {
            if (x.yInterval.upper == h.coord)
            {
                s = x;
                break;
            }
        }
    }
    else if (h.side.type == "top")
    {
        for (auto x : S)
        {
            if (x.yInterval.lower == h.coord)
            {
                s = x;
                break;
            }
        }
    }

    set<Interval> J = freeQuery(h.interval, s);

    for (auto x : J)
    {
        ans.insert({x, h.coord});
    }

    return ans;
}

/**
 * Divides the rectangles into horizontal edges and feeds them to the other functions
 * @param Set of Rectangles rect
 * @param Set of Stripes S
 * @return set of Line Segments ans
 * 
*/
set<LineSegment> contour(set<Rectangle> rect, set<Stripe> S)
{

    set<LineSegment> ans;

    for (auto r : rect)
    {
        Edge eTop(r.yRight, r.xInterval, "top");
        Edge eBottom(r.yLeft, r.xInterval, "bottom");

        set<LineSegment> pieces1 = contourPieces(eTop, S);
        set<LineSegment> pieces2 = contourPieces(eBottom, S);

        for (auto x : pieces1)
        {
            ans.insert(x);
        }
        for (auto x : pieces2)
        {
            ans.insert(x);
        }
    }

    return ans;
}

/**
 * Utility function to help with debugging
 * @param ctree *tree 
 * 
*/
void dfs2(ctree *tree)
{
    //cout << tree->x << " " << tree->side << endl;

    if (tree->left_child != NULL)
        dfs2(tree->left_child);
    if (tree->right_child != NULL)
        dfs2(tree->right_child);
}

/**
 * Utility function to find the union of a set of intervals
 * @param vector of intervals a
 * @return set of intervals ans
 */
set<Interval> mergeIntervals(vector<Interval> a) {
    int n = (T)a.size();
    set<Interval> ans;

    if(n==0) return ans;

    sort(a.begin(), a.end(), [&](Interval u, Interval v) { return u.lower < v.lower ;});

    stack<Interval> s;
    s.push(a[0]);

    for(int i=1;i<n;i++) {
        Interval top = s.top();
        if(top.upper < a[i].lower) {
            s.push(a[i]);
        }

        else if(top.upper < a[i].upper) {
            top.upper = a[i].upper;
            s.pop();
            s.push(top);
        }
    }

    while(!s.empty()) {
        Interval temp = s.top();
        ans.insert(temp);
        s.pop();
    }

    return ans;
}

/**
 * This is the main function entry point of the program 
*/
int main(int argc, char *argv[])
{
    cin >> numberOfRectangles;

    set<Rectangle> rect;
    for (T i = 0; i < numberOfRectangles; i++)
    {
        T x1, x2, y1, y2;
        cin >> x1 >> x2 >> y1 >> y2;

        Rectangle r(x1, y1, x2, y2, i);
        rect.insert(r);
    }

    set<Stripe> ans = RECTANGLE_DAC(rect);
    cout << "size of final set of stripes is: " << ans.size() << endl;

    T measure = calculateMeasure(ans);
    cout << "The measure of ths stripes is: " << calculateMeasure(ans) << endl;

    set<LineSegment> finalContourHorizontal = contour(rect, ans);

    map<T, vector<T>> cnt;
    map<T, vector<Interval>> yMap;
    for (auto x : finalContourHorizontal)
    {
        yMap[x.coord].push_back(x.interval);
    }

    set<LineSegment> horizontalMergedContour;
    for(auto x : yMap) 
    {
        set<Interval> temp = mergeIntervals(x.second);
        for(auto y : temp) {
            LineSegment l;
            l.coord = x.first;
            l.interval = y;

            horizontalMergedContour.insert(l);
        }
    }
    //cout<<"The merged horizontal edges are: \n";
    //for(auto x : horizontalMergedContour) 
    //{
    //    x.print();
    //}

    for(auto x : horizontalMergedContour) 
    {
        cnt[x.interval.lower].push_back(x.coord);
        cnt[x.interval.upper].push_back(x.coord);
    }

    set<LineSegment> finalContourVertical;
    for (auto x : cnt)
    {
        vector<T> temp = x.second;
        sort(temp.begin(), temp.end());

        for (int i = 0; i < (T)temp.size() - 1; i += 2)
        {
            LineSegment l;
            l.coord = x.first;
            l.interval = {temp[i], temp[i + 1]};

            finalContourVertical.insert(l);
        }
    }

    //for (auto x : finalContourHorizontal)
    //{
    //    x.print();
    //}
    //for (auto x : finalContourVertical)
    //{
    //    x.print();
    //}

    ofstream outputFile;
    outputFile.open("data.txt");

    outputFile << numberOfRectangles << endl;
    for (auto x : rect)
    {
        outputFile << x.xLeft << " " << x.xRight << " " << x.yLeft << " " << x.yRight << endl;
    }

    outputFile << measure << endl;
    outputFile << finalContourHorizontal.size() + finalContourVertical.size() << endl;
    for (auto x : finalContourHorizontal)
    {
        outputFile << x.interval.lower << " " << x.coord << " " << x.interval.upper << " " << x.coord << endl;
    }
    for (auto x : finalContourVertical)
    {
        outputFile << x.coord << " " << x.interval.lower << " " << x.coord << " " << x.interval.upper << endl;
    }

    outputFile.close();
}
