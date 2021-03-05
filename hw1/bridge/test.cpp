#include <iostream>
#include <math.h>
#include <algorithm>
#include <set>
#include <vector>
#define MAX_LEN (1e9)
#define MAX_LEN_SQ (2e18)
using namespace std;

typedef pair<long long, long long> Point;
typedef long long ll;

inline ll distance(const Point &a, const Point &b)
{
    return (a.first - b.first) * (a.first - b.first) +
           (a.second - b.second) * (a.second - b.second);
}

bool compareX(const Point &a, const Point &b)
{
    if (a.first < b.first)
        return 1;
    if (a.first > b.first)
        return 0;
    return a.second < b.second;
}

struct pointCmp
{
    bool operator()(const Point &a, const Point &b)
    {
        if (a.second < b.second)
            return 1;
        else if (a.second > b.second)
            return 0;
        return a.first < b.first;
    }
};

ll smallestDist(vector<Point> &point)
{
    sort(point.begin(), point.end(), compareX);
    int n = point.size();
    ll min_dist = MAX_LEN_SQ;
    long double min_dist_sqrt = MAX_LEN_SQ;
    set<Point, pointCmp> strip;
    int l = 0;

    for (auto &current : point)
    {
        while (point[l].first + floor(min_dist_sqrt) <= current.first && distance(point[l], current) > min_dist)
        {
            strip.erase(point[l]);
            l++;
        }
        auto lower_iter = strip.lower_bound(Point(current.first, current.second - ceil(min_dist_sqrt) - 1));
        auto upper_iter = strip.upper_bound(Point(current.first, current.second + ceil(min_dist_sqrt) + 1));
        for (auto iter = lower_iter; iter != upper_iter; iter++)
        {
            ll dist = distance(*iter, current);
            if (dist == 0)
                return 0;
            if (dist < min_dist)
            {
                min_dist = dist;
                min_dist_sqrt = sqrt(min_dist);
            }
        }
        strip.insert(current);
    }
    return min_dist;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    cin >> n;
    vector<Point> point;
    for (int i = 0; i < n; i++)
    {
        ll x, y;
        cin >> x >> y;
        point.push_back(Point(x, y));
    }
    
    cout << -smallestDist(point) << endl;

    return 0;
}