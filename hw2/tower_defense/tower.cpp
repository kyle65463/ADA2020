#include <iostream>
#include <limits.h>
using namespace std;

#define MAXN 500000
#define MAXK 400

typedef long long ll;

class Value
{
public:
    Value() : value(LLONG_MIN) {}
    Value(ll value, int pos) : value(value), pos(pos) {}
    ll value;
    int pos;
};

ll a, b;

class Entry
{
public:
    Value real, temp;
};

inline ll bonus(int cur_pos, int last_pos)
{
    return max((ll)0, a * (b - cur_pos + last_pos));
}

inline ll calValue(Value& cur_val, Value& last_val)
{
    return cur_val.value + last_val.value + bonus(cur_val.pos, last_val.pos);
}

inline ll calValue(int i, Value& last_val)
{
    return last_val.value + bonus(i, last_val.pos);
}

Entry dp[2][MAXK];
int field[MAXN];

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k >> a >> b;
    
    for (int i = 0; i < n; ++i)
        cin >> field[i];

    dp[0][0].real = dp[0][0].temp = Value(field[0], 0);

    for (int i = 1; i < n; ++i)
    {
        for (int j = 0; j < min(i + 1, k); ++j)
        {
            Entry &cur = dp[i % 2][j];
            cur.temp = Value(field[i], i);

            // Get temp
            if (j != 0)
            {
                Entry &dia = dp[(i - 1) % 2][j - 1];
                cur.temp.value = max(calValue(cur.temp, dia.real), calValue(cur.temp, dia.temp));
            }

            Entry &left = dp[(i - 1) % 2][j];
            if (calValue(i + 1, left.temp) > calValue(i + 1, cur.temp))
                cur.temp = left.temp;

            // Get real
            cur.real = left.real;
            if (cur.real.value < cur.temp.value)
                cur.real = cur.temp;

            //cout << cur.real.value << " " << cur.temp.value << endl;
        }
    }

    ll result = 0;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < k; ++j)
            result = max(result, dp[i][j].real.value);

    cout << result << endl;
}