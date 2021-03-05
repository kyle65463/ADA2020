#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <limits.h>
using namespace std;

#define MAXN 500010
#define INDEX 0
#define VALUE 1
typedef long long ll;

class City
{
public:
    ll d;
    ll prev, index;
    map<ll, ll> road;
    bool traversed, protect = false;

    inline void init(ll i)
    {
        traversed = false;
        index = i;
        prev = -1;
        d = LLONG_MAX;
    }
};

ll n;
City city[MAXN];
queue<ll> protected_city;

inline void removeEdges(ll s)
{
    while (city[s].prev != -1)
    {
        city[s].road.erase(city[s].prev);
        city[city[s].prev].road.erase(s);
        if (!city[s].protect && !city[s].road.empty())
        {
            //cout << "push " << s << endl;
            protected_city.push(s);
            city[s].protect = true;
        }
        s = city[s].prev;
    }
    if (!city[s].protect && !city[s].road.empty())
    {
        //cout << "push " << s << endl;
        protected_city.push(s);
        city[s].protect = true;
    }
}

ll bfs(ll s, ll type, bool remove = false)
{
    vector<ll> visited;
    city[s].d = 0;
    city[s].traversed = true;

    ll max_value = 0, max_index = s;

    queue<ll> q;
    q.push(s);
    while (!q.empty())
    {
        City *cur = &city[q.front()];
        q.pop();

        visited.push_back(cur->index);
        for (auto &road : cur->road)
        {
            ll v = road.first;
            if (!city[v].traversed)
            {
                city[v].traversed = true;
                city[v].prev = cur->index;
                city[v].d = cur->d + road.second;
                if (max_value < city[v].d)
                {
                    max_value = city[v].d;
                    max_index = v;
                }
                q.push(v);
            }
        }
    }

    if (remove)
        removeEdges(max_index);

    for (ll i = 0; i < visited.size(); i++)
        city[visited[i]].init(visited[i]);

    return type == INDEX ? max_index : max_value;
}

vector<ll> all_ans;

void getFirstLongestPath()
{
    for (ll i = 1; i <= n; i++)
        city[i].init(i);
    ll max_index = bfs(1, INDEX);
    ll max_value = bfs(max_index, VALUE, true);
    all_ans.push_back(max_value);
}

void getOtherLongestPaths()
{
    while (!protected_city.empty())
    {
        City *cur = &city[protected_city.front()];
        //cout << protected_city.front() << endl;
        protected_city.pop();
        while (!cur->road.empty())
        {
            ll cur_max_value = bfs(cur->index, VALUE, true);
            all_ans.push_back(cur_max_value);
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    cin >> n;
    ll u, v, w;
    for (ll i = 0; i < n - 1; i++)
    {
        cin >> u >> v >> w;
        city[u].road[v] = w;
        city[v].road[u] = w;
    }

    ll ans = 0;
    cout << ans << " ";

    getFirstLongestPath();
    getOtherLongestPaths();
    sort(all_ans.begin(), all_ans.end(), greater<ll>());

    for (int i = 0; i < n - 1; i++)
    {
        if (i < all_ans.size())
            ans += all_ans[i];
        cout << ans << " ";
    }
    cout << endl;
}
