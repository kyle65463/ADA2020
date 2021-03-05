#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
using namespace std;

typedef long long ll;

class Point
{
public:
    ll x, y;
};

int compareX(const void *a, const void *b)
{
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return p1->x > p2->x;
}

int compareY(const void *a, const void *b)
{
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return p1->y > p2->y;
}

ll dist(Point p1, Point p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) +
           (p1.y - p2.y) * (p1.y - p2.y);
}

ll bruteForce(Point P[], int n)
{
    ll min = LLONG_MAX;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}

ll stripClosest(Point strip[], int size, double d)
{
    ll min = d;

    qsort(strip, size, sizeof(Point), compareY);

    for (int i = 0; i < size; ++i)
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < sqrt(min); ++j)
            if (dist(strip[i], strip[j]) < min)
                min = dist(strip[i], strip[j]);

    return min;
}

ll closest(Point P[], int n)
{
    if (n <= 7)
        return bruteForce(P, n);

    int mid = n / 2;
    Point midPoint = P[mid];

    ll dl = closest(P, mid);
    ll dr = closest(P + mid, n - mid);

    ll d = min(dl, dr);

    Point strip[n];
    int j = 0;
    strip[j++] = midPoint;

    for (int i = mid + 1; i < n; i++){
        if (abs(P[i].x - midPoint.x) < sqrt(d))
            strip[j] = P[i], j++;
        else
            break;
    }

    for (int i = mid - 1; i >= 0; i--){
        if (abs(P[i].x - midPoint.x) < sqrt(d))
            strip[j] = P[i], j++;
        else
            break;
    }

    return min(d, stripClosest(strip, j, d));
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    cin >> n;

    Point P[n];
    for (int i = 0; i < n; i++)
    {
        cin >> P[i].x >> P[i].y;
    }

    qsort(P, n, sizeof(Point), compareX);
    cout << -closest(P, n);
    return 0;
}