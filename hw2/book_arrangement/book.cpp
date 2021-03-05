#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

bool flag;
void goLeft(ll book[], ll &left_odd, ll &left_zero, ll &right_odd, ll &right_zero, ll &cur, bool isBound)
{
    if (!isBound)
    {
        right_odd += (book[cur] % 2 == 1 ? 1 : 0);
        right_zero += (book[cur] == 0 ? 1 : 0);
    }
    cur--;
    left_odd -= (book[cur] % 2 == 1 ? 1 : 0);
    left_zero -= (book[cur] == 0 ? 1 : 0);
}

void goRight(ll book[], ll &left_odd, ll &left_zero, ll &right_odd, ll &right_zero, ll &cur, bool isBound)
{
    if (!isBound)
    {
        left_odd += (book[cur] % 2 == 1 ? 1 : 0);
        left_zero += (book[cur] == 0 ? 1 : 0);
    }
    cur++;
    right_odd -= (book[cur] % 2 == 1 ? 1 : 0);
    right_zero -= (book[cur] == 0 ? 1 : 0);
}
ll left_odd = 0, right_odd = 0;
ll left_zero = 0, right_zero = 0;
ll cur = 0;
ll result = -1;
ll cur_result = 0;

ll run(ll book[], long long total, ll l_bound, ll r_bound, vector<ll> &output)
{
    cur_result = 0;
    while (total > 0)
    {
        if (flag)
            output.push_back(cur);

        if (book[cur] == 0)
        {
            cur_result++;
            if (cur_result >= result && result >= 0)
            {
                return 1;
                break;
            }
        }
        else
        {
            book[cur]--;
            total--;
        }

        //cout << cur << " " << book[cur] << " l:" << left_zero << " r:" << right_zero << endl;

        if (cur == l_bound)
        {

            if (book[cur] == 0 && r_bound - l_bound > 1)
            {
                l_bound++;
                goRight(book, left_odd, left_zero, right_odd, right_zero, cur, true);
            }
            else
            {
                goRight(book, left_odd, left_zero, right_odd, right_zero, cur, false);
            }
            continue;
        }
        if (cur == r_bound)
        {
            if (book[cur] == 0 && r_bound - l_bound > 1)
            {
                r_bound--;
                goLeft(book, left_odd, left_zero, right_odd, right_zero, cur, true);
            }
            else
            {
                goLeft(book, left_odd, left_zero, right_odd, right_zero, cur, false);
            }
            continue;
        }

        if (left_odd < right_odd)
        {
            goLeft(book, left_odd, left_zero, right_odd, right_zero, cur, false);
        }
        else if (left_odd > right_odd)
        {
            goRight(book, left_odd, left_zero, right_odd, right_zero, cur, false);
        }
        else
        {
            if (left_zero < right_zero)
            {
                goLeft(book, left_odd, left_zero, right_odd, right_zero, cur, false);
            }
            else if (left_zero > right_zero)
            {
                goRight(book, left_odd, left_zero, right_odd, right_zero, cur, false);
            }
            else
            {
                if (l_bound - cur > cur - r_bound)
                {
                    goLeft(book, left_odd, left_zero, right_odd, right_zero, cur, false);
                }
                else
                {
                    goRight(book, left_odd, left_zero, right_odd, right_zero, cur, false);
                }
            }
        }
    }

    return 0;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;

    cin >> n;
    ll book[n + 2];
    ll book2[n + 2];
    ll book3[n + 2];

    ll max_book = 0, max_index;
    long long total = 0;

    ll max_zero = 0, max_odd = 0;
    ll all_zero = 0, all_odd = 0;

    for (ll i = 1; i <= n; i++)
    {
        cin >> book[i];
        book2[i] = book[i];
        book3[i] = book[i];

        if (book[i] > max_book)
        {
            max_book = book[i];
            max_index = i;
            max_zero = all_zero;
            max_odd = all_odd;
        }

        if (book[i] == 0)
        {
            all_odd++;
            all_zero++;
        }
        else if (book[i] % 2 == 1)
        {
            all_odd++;
        }

        total += book[i];
    }
    cin >> flag;

    ll l_bound = 1, r_bound = n;

    if (!flag)
    {
        while (book[r_bound] == 0)
            r_bound--;
        while (book[l_bound] == 0)
            l_bound++;
            
        ll pre_answer = 0;
        for (ll i = l_bound; i <= r_bound; i++)
        {
            if (book[i] == 0)
            {
                book[i]++;
                pre_answer++;
            }
        }

        ll cur_in, cur_out, next_in = book[l_bound], next_out = book[l_bound];
        ll ans = 0;

        for (ll i = l_bound; i <= r_bound; i++)
        {
            cur_in = next_in, cur_out = next_out;
            //cout << cur_in << " " << cur_out << endl;
            if (i != r_bound)
                next_in = book[i + 1], next_out = book[i + 1];
            else
                next_in = -1, next_out = -1;

            if (next_in > 0)
            {
                ll val = min(next_in, cur_out);
                next_in -= val;
                cur_out -= val;
            }
            if (next_out > 0)
            {
                ll val = min(i == r_bound - 1 ? next_out : next_out - 1, cur_in);
                cur_in -= val;
                next_out -= val;
            }

            ans += cur_in + cur_out;
        }
        ans = ans / 2 - 1;
        ans = max(ans, (ll)0);

        cout << ans + pre_answer << endl;
        return 0;
    }

    while (book[r_bound] == 0 && r_bound - l_bound > 1)
    {
        r_bound--;
        all_odd--;
        all_zero--;
    }
    while (book[l_bound] == 0 && r_bound - l_bound > 1)
    {
        l_bound++;
        max_odd--;
        all_odd--;
        max_zero--;
    }

    vector<ll> output;
    vector<ll> output2;
    vector<ll> output3;

    result = -1;
    ll id = 0;

    // Left
    cur = l_bound;
    left_odd = 0, right_odd = all_odd;
    left_zero = 0, right_zero = all_zero;
    if (book[l_bound] % 2 == 1)
        right_odd--;
    if (run(book, total, l_bound, r_bound, output) == 0)
        id = 0;
    result = cur_result;

    // Right
    if (result != 0)
    {
        left_odd = all_odd, right_odd = 0;
        left_zero = all_zero, right_zero = 0;
        cur = r_bound;
        if (book[r_bound] % 2 == 1)
            left_odd--;
        if (run(book2, total, l_bound, r_bound, output2) == 0)
            id = 1;
        result = cur_result;
    }

    // Max
    if (result != 0)
    {
        left_odd = max_odd, right_odd = all_odd - max_odd;
        left_zero = max_zero, right_zero = all_zero - max_zero;
        if (book[max_index] % 2 == 1)
            right_odd--;
        cur = max_index;
        if (run(book3, total, l_bound, r_bound, output3) == 0)
            id = 2;
        result = cur_result;
    }

    cout << result << endl;

    if (flag)
    {
        if (id == 0)
        {
            for (ll i = 0; i < output.size(); ++i)
                cout << output[i] << " ";
            cout << endl;
        }
        if (id == 1)
        {
            for (ll i = 0; i < output2.size(); ++i)
                cout << output2[i] << " ";
            cout << endl;
        }
        if (id == 2)
        {
            for (ll i = 0; i < output3.size(); ++i)
                cout << output3[i] << " ";
            cout << endl;
        }
    }
}