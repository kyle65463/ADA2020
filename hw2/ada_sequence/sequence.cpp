#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <limits.h>
using namespace std;

typedef long long ll;

#define MAXN 210
#define MAXS 605

#define LEFT 1
#define DOWN 2
#define DIA 3
#define DIA_CHANGE_I 4
#define DIA_CHANGE_J 5

ll a[MAXN], c[MAXN], s[MAXN];
ll dp[MAXN][MAXN][MAXS] = {};
ll path[MAXN][MAXN][MAXS] = {};

class Pair
{
public:
    Pair(ll index, ll value) : index(index), value(value) {}
    ll index;
    ll value;
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll n, k, d;
    cin >> n >> k >> d;
    for (int i = 0; i < n; ++i)
        cin >> a[i];
    for (int i = 0; i < n; ++i)
        cin >> c[i];
    for (int i = 0; i < n; ++i)
        cin >> s[i];

    ll max_score = n * 3;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int s_sum = 1; s_sum <= max_score; ++s_sum)
                dp[i][j][s_sum] = INT_MAX - max_score;

    for (int len = 1; len <= n; ++len)
    {
        for (int i = 0; i <= n - len; ++i)
        {
            ll j = i + len - 1;
            if (len == 1)
            {
                for (int s_sum = 0; s_sum <= s[i]; ++s_sum)
                    dp[i][j][s_sum] = 0;
                continue;
            }

            for (int s_sum = 0; s_sum <= max_score; ++s_sum)
            {
                ll cur_s_sum = s[i] + s[j];

                // a[i] and a[j] are same
                if (abs(a[i] - a[j]) <= d)
                {
                    if (s_sum - cur_s_sum >= 0)
                    {
                        dp[i][j][s_sum] = dp[i + 1][j - 1][s_sum - cur_s_sum];
                    }
                    else
                    {
                        dp[i][j][s_sum] = dp[i + 1][j - 1][s_sum];
                    }
                    path[i][j][s_sum] = DIA;
                }

                // a[i] and a[j] are different
                ll last_min_cost = min(dp[i + 1][j][s_sum], dp[i][j - 1][s_sum]);
                ll last_min_path = dp[i + 1][j][s_sum] < dp[i][j - 1][s_sum] ? DOWN : LEFT;

                // Default: don't change a[i] or a[j]
                ll temp_dp = last_min_cost;
                ll temp_path = last_min_path;

                // Find whether change a[i] or a[j] is better
                if (c[i] < c[j])
                {
                    // Change a[i]
                    if (s_sum - cur_s_sum >= 0)
                    {
                        // Can change
                        if (dp[i + 1][j - 1][s_sum - cur_s_sum] + c[i] < last_min_cost)
                        {
                            // Change a[i] is better than not change
                            temp_dp = dp[i + 1][j - 1][s_sum - cur_s_sum] + c[i];
                            temp_path = DIA_CHANGE_I;
                        }
                    }
                }
                else
                {
                    // Change a[j]
                    if (s_sum - cur_s_sum >= 0)
                    {
                        // Can change
                        if (dp[i + 1][j - 1][s_sum - cur_s_sum] + c[j] < last_min_cost)
                        {
                            // Change a[j] is better than not change
                            temp_dp = dp[i + 1][j - 1][s_sum - cur_s_sum] + c[j];
                            temp_path = DIA_CHANGE_J;
                        }
                    }
                }

                // Get minimum dp value
                if (temp_dp < dp[i][j][s_sum])
                {
                    dp[i][j][s_sum] = temp_dp;
                    path[i][j][s_sum] = temp_path;
                }
            }
        }
    }

    ll ans = 0;
    for (int i = 0; i <= max_score; ++i)
        if (dp[0][n - 1][i] <= k)
            ans = i;
    cout << ans << endl;

    ll cur_i = 0, cur_j = n - 1, cur_s = ans;
    ll dir = path[cur_i][cur_j][cur_s];
    vector<ll> ans_index;
    vector<Pair> change_index;
    while (cur_j - cur_i > 0 && cur_s >= 0)
    {
        if (dir == DIA_CHANGE_I)
        {
            change_index.push_back(Pair(cur_i, a[cur_j]));
            ans_index.push_back(cur_i);
            ans_index.push_back(cur_j);
            cur_s -= (s[cur_i] + s[cur_j]), cur_i++, cur_j--;
        }
        else if (dir == DIA_CHANGE_J)
        {
            change_index.push_back(Pair(cur_j, a[cur_i]));
            ans_index.push_back(cur_i);
            ans_index.push_back(cur_j);
            cur_s -= (s[cur_i] + s[cur_j]), cur_i++, cur_j--;
        }
        else if (dir == LEFT)
        {
            cur_j--;
        }
        else if (dir == DOWN)
        {
            cur_i++;
        }
        else if (dir == DIA)
        {
            ans_index.push_back(cur_i);
            ans_index.push_back(cur_j);
            cur_s -= (s[cur_i] + s[cur_j]);
            cur_i++, cur_j--;
        }

        dir = path[cur_i][cur_j][cur_s];
    }

    if (cur_i == cur_j)
        ans_index.push_back(cur_i);

    cout << change_index.size() << endl;
    for (int i = 0; i < change_index.size(); ++i)
        cout << change_index[i].index + 1 << " " << change_index[i].value << endl;

    sort(ans_index.begin(), ans_index.end());
    cout << ans_index.size() << endl;
    for (int i = 0; i < ans_index.size(); ++i)
        cout << ans_index[i] + 1 << " ";
    cout << endl;
}