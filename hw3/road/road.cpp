#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits.h>
using namespace std;
typedef long long ll;

#define MAXN 300005

int n, m, s, t;

class Road
{
public:
    Road(int u, int v, ll w) : u(u), v(v), w(w) {}
    int u, v;
    ll w;
    bool operator<(const Road &b) const
    {
        return w < b.w;
    }
};

class City
{
public:
    ll index, w, d;
    City *prev;
    vector<Road> road;
};

City city[MAXN];
vector<Road> all_road;

struct DisjointSets
{
    int *parent, *rnk;
    int n;
    DisjointSets(int n)
    {
        this->n = n;
        parent = new int[n + 1];
        rnk = new int[n + 1];
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;
            parent[i] = i;
        }
    }
    int find(int u)
    {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }
    void merge(int x, int y)
    {
        x = find(x), y = find(y);
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else
            parent[x] = y;
        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

DisjointSets d(0);

void testOrAdd(Road *road, bool is_test)
{
    int set_u = d.find(road->u);
    int set_v = d.find(road->v);
    if (set_u != set_v)
    {
        if (is_test)
        {
            city[road->u].road.push_back(*road);
            city[road->v].road.push_back(Road(road->v, road->u, road->w));
        }
        else
        {
            d.merge(set_u, set_v);
        }
    }
}

void findMst()
{
    d = DisjointSets(n);
    sort(all_road.begin(), all_road.end());

    int count = 0;
    ll last_w = all_road[0].w;
    bool test = true;
    for (int i = 0; i < all_road.size(); i++)
    {

        ///cout << all_road[i].w << " " << last_w << endl;
        if (all_road[i].w != last_w)
        {
            //cout << "yoooo " << count << endl;
            for (int j = i - count; j < i; j++)
            {
                //cout << "add:" << " " << all_road[j].v << " " << all_road[j].u << " " << all_road[j].w << endl;
                testOrAdd(&all_road[j], false);
            }
            count = 0;
        }
        //cout << "test:" << all_road[i].v << " " << all_road[i].u << " " << all_road[i].w << endl;
        testOrAdd(&all_road[i], true);
        count++;
        last_w = all_road[i].w;
    }
}

struct cmp
{
    inline bool operator()(City *a, City *b)
    {
        return a->d > b->d;
    }
};

priority_queue<City *, vector<City *>, cmp> pq;

void printAllEdge()
{
    for (int i = 1; i <= n; i++)
    {
        for (auto &road : city[i].road)
        {
            cout << road.u << " " << road.v << " " << road.w << endl;
        }
    }
}

void relax(City *u, City *v, ll w)
{
    if (v->d > u->d + w)
    {
        v->d = u->d + w;
        pq.push(v);
        v->prev = u;
    }
}

ll findShortestPath()
{
    for (int i = 1; i <= n; i++)
    {
        city[i].d = LLONG_MAX - 100;
        if (i == s)
        {
            city[i].d = 0;
        }
        city[i].prev = NULL;
        city[i].index = i;
    }

    pq.push(&city[s]);

    while (!pq.empty())
    {
        City *cur = pq.top();
        pq.pop();
        for (auto &road : cur->road)
        {
            relax(&city[road.u], &city[road.v], road.w);
        }
    }

    return city[t].d;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    cin >> n >> m >> s >> t;

    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        Road *road = new Road(u, v, w);
        //city[u].road.push_back(*road);
        //city[v].road.push_back(Road(v, u, w));
        all_road.push_back(*road);
    }

    findMst();
    //printAllEdge();
    cout << findShortestPath() << endl;
}