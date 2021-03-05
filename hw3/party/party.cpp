#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

#define MAXN 200010
typedef long long ll;

int n, q;
class Group
{
public:
    Group(ll type, ll n_people, ll time) : type(type), n_people(n_people), time(time) {} 
    ll type, n_people, time;
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    ll ans[MAXN] = {};
    deque<Group *> party;

    cin >> n >> q;
    for (int round = 0; round < q; round++)
    {
        string dir;
        ll n_people, type, k;
        cin >> dir >> n_people >> type >> k;
        Group *group = new Group(type, n_people + k, round);

        if (dir == "front")
        {
            if (!party.empty())
            {
                while (k > 0)
                {
                    Group *group = party.front();
                    ll temp_k = min(k, group->n_people);
                    group->n_people -= temp_k;
                    ans[group->type] += temp_k * (round - group->time);
                    k -= temp_k;
                    if (group->n_people == 0)
                        party.pop_front();
                }
            }
            party.push_front(group);
        }
        else if (dir == "back")
        {
            if (!party.empty())
            {
                while (k > 0)
                {
                    Group *group = party.back();
                    ll temp_k = min(k, group->n_people);
                    group->n_people -= temp_k;
                    ans[group->type] += temp_k * (round - group->time);
                    k -= temp_k;
                    if (group->n_people == 0)
                        party.pop_back();
                }
            }
            party.push_back(group);
        }
    }

    for (auto &group : party)
        ans[group->type] += group->n_people * (q - group->time);

    for (int i = 1; i <= n; i++)
        cout << ans[i] << " ";
    cout << endl;
}
