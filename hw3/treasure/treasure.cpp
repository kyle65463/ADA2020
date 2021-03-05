#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

#define MAXN 500005
#define MAXM 500005
#define UNTRAVERSED -1

typedef long long ll;

class Road
{
public:
    ll v, w;
};

class City
{
public:
    City()
    {
        w = 0;
    }
    ll index, w, d, f;
    vector<Road> road;
    vector<Road> rev_road;
};

ll n, m;
City *city[MAXN];
City **city_ptr[MAXN];

ll t = 0;
bool traversed[MAXN] = {};

void dfs(ll index, bool is_rev, set<ll> *scc = NULL)
{
    if (traversed[index])
        return;
    traversed[index] = true;
    t++;
    city[index]->d = t;

    if (!is_rev)
    {
        for (ll i = 0; i < city[index]->road.size(); i++)
            if (!traversed[city[index]->road[i].v])
                dfs(city[index]->road[i].v, is_rev);
    }
    else
    {
        for (ll i = 0; i < city[index]->rev_road.size(); i++)
            if (!traversed[city[index]->rev_road[i].v])
                dfs(city[index]->rev_road[i].v, is_rev, scc);
    }

    t++;
    city[index]->f = t;

    if (is_rev)
    {
        scc->insert(index);
    }
}

void shrinkGraph(ll index)
{
    if (traversed[index])
        return;
    set<ll> *scc = new set<ll>();
    dfs(index, true, scc);

    City *merged_city = new City();
    merged_city->index = index;
    for (auto it = scc->begin(); it != scc->end(); ++it)
    {
        ll scc_index = *it;

        merged_city->w += city[scc_index]->w;
        for (ll j = 0; j < city[scc_index]->road.size(); j++)
        {
            if (scc->find(city[scc_index]->road[j].v) == scc->end())
            {
                // Not duplicate
                merged_city->road.push_back(city[scc_index]->road[j]);
            }
            else
            {
                // Duplicate
                merged_city->w += city[scc_index]->road[j].w;
            }
        }
        city[scc_index] = merged_city;
    }
}

struct cmp
{
    inline bool operator()(City **a, City **b)
    {
        return (*a)->f > (*b)->f;
    }
};

ll dp[MAXN];

ll getValue(ll index)
{
    index = city[index]->index;
    if (dp[index] != UNTRAVERSED)
        return dp[index];

    ll max_value = 0;
    for (ll i = 0; i < city[index]->road.size(); i++)
    {
        max_value = max(max_value, getValue(city[index]->road[i].v) + city[index]->road[i].w);
    }
    dp[index] = max_value + city[index]->w;
    return dp[index];
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    for (ll i = 1; i <= n; i++)
    {
        city[i] = new City();
        cin >> city[i]->w;
        city[i]->index = i;
        city_ptr[i] = &city[i];
        dp[i] = UNTRAVERSED;
    }

    ll u;
    for (ll i = 0; i < m; i++)
    {
        cin >> u;
        Road road;
        cin >> road.v >> road.w;
        city[u]->road.push_back(road);
        swap(road.v, u);
        city[u]->rev_road.push_back(road);
    }

    // Do dfs on G
    for (ll i = 1; i <= n; i++)
        dfs(i, false);

    sort(city_ptr + 1, city_ptr + n + 1, cmp());
    for (ll i = 1; i <= n; i++)
        traversed[i] = false;

    for (ll i = 1; i <= n; i++)
    {
        shrinkGraph((*city_ptr[i])->index);
    }

    ll ans = 0;
    for (ll i = 1; i <= n; i++)
        ans = max(ans, getValue(i));

    cout << ans << endl;
}
