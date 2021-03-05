#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;

#define MAXK 500000
#define INCREMENT 0
#define DECREMENT 1

typedef long long ll;

ll k;

inline void mod(ll &num)
{
    num %= k;
    if (num < 0)
        num += k;
}

ll merge_id = 0;

class Entry
{
public:
    Entry() : value(0), id(0) {}
    ll value;
    ll id;
};

class Value
{
public:
    Value() : maximum(0), minimum(0), prefix(0) {}
    ll maximum;
    ll minimum;
    ll prefix;
};

Value value[MAXK];

class Table
{
private:
    Entry *table;

    inline void checkId(ll key)
    {
        if (table[key].id != merge_id)
        {
            table[key].value = 0;
            table[key].id = merge_id;
        }
        return;
    }

public:
    Table() : table(new Entry[MAXK]()) { init(); }
    ll offset;
    ll pos;
    ll size;

    inline void init()
    {
        pos = 0;
        offset = 0;
        size = 0;
    }

    void shiftOffset(ll value)
    {
        offset += value;
        mod(offset);
    }

    void update(ll key, char type)
    {
        key += offset;
        mod(key);
        checkId(key);
        if (type == INCREMENT)
            table[key].value++;
        else if (type == DECREMENT)
            table[key].value--;
    }
    inline ll get(ll key)
    {
        key += offset;
        mod(key);
        checkId(key);
        return table[key].value;
    }
};

Table *global_table = new Table();
Table *max_table = new Table();
Table *min_table = new Table();

ll getCount(ll key)
{
    return global_table->get(key) + min_table->get(key) + max_table->get(key);
}

ll last_max = 0, last_min = LLONG_MAX;

ll getSum(int pos)
{
    ll sum = value[pos].prefix;

    if (pos < max_table->pos)
        sum -= last_max;
    else
        sum -= value[pos].maximum;
    if (pos < min_table->pos)
        sum -= last_min;
    else
        sum -= value[pos].minimum;

    mod(sum);
    return sum;
}

ll findCrossPair(vector<int> &candies, int l_bound, int r_bound)
{
    merge_id++;

    int m = (l_bound + r_bound) / 2;

    ll sum = 0;
    max_table->init();
    min_table->init();

    for (int r = m + 1; r <= r_bound; r++)
    {
        int index = r - m - 1;
        value[index].maximum = max(r != m + 1 ? value[index - 1].maximum : 0, (ll)candies[r]);
        value[index].minimum = min(r != m + 1 ? value[index - 1].minimum : LLONG_MAX, (ll)candies[r]);
        value[index].prefix = ((r != m + 1 ? value[index - 1].prefix : 0) + candies[r]) % k;

        sum = getSum(index);
        mod(sum);
        global_table->update(sum, INCREMENT);
        global_table->size++;
    }

    last_max = 0, last_min = LLONG_MAX;
    ll left_sum = 0, result = 0;

    for (int l = m; l >= l_bound; l--)
    {
        //Handle maximum
        if (candies[l] > last_max)
        {
            //Change offset
            ll diff = candies[l] - last_max;
            diff %= k;
            max_table->shiftOffset(diff);
            if (min_table->pos <= max_table->pos)
                min_table->shiftOffset(diff);

            last_max = candies[l];
        }
        while (max_table->pos < (r_bound - m) && candies[l] > value[max_table->pos].maximum)
        {
            if (min_table->pos == max_table->pos && max_table->size > 0)
            {
                swap(min_table, max_table);
            }
            //Delete from global table
            sum = getSum(max_table->pos);
            if (max_table->pos < min_table->pos)
            {
                min_table->update(sum, DECREMENT);
                min_table->size--;
            }
            else
            {
                global_table->update(sum, DECREMENT);
                global_table->size--;
            }

            //Add to maximum table
            value[max_table->pos].maximum = candies[l];
            sum = getSum(max_table->pos);
            max_table->update(sum, INCREMENT);
            max_table->size++;

            max_table->pos++;
            last_max = candies[l];
        }

        //Handle minimum
        if (candies[l] < last_min)
        {
            //Change offset
            ll diff = candies[l] - last_min;
            diff %= k;
            min_table->shiftOffset(diff);
            if (max_table->pos <= min_table->pos)
                max_table->shiftOffset(diff);

            last_min = candies[l];
        }

        while (min_table->pos < (r_bound - m) && candies[l] < value[min_table->pos].minimum)
        {
            if (min_table->pos == max_table->pos && min_table->size > 0)
            {
                swap(min_table, max_table);
            }

            //Delete from global/max table
            sum = getSum(min_table->pos);
            if (min_table->pos < max_table->pos)
            {
                max_table->update(sum, DECREMENT);
                max_table->size--;
            }
            else
            {
                global_table->update(sum, DECREMENT);
                global_table->size--;
            }

            //Add to minimum table
            value[min_table->pos].minimum = candies[l];
            sum = getSum(min_table->pos);
            min_table->update(sum, INCREMENT);
            min_table->size++;

            min_table->pos++;
            last_min = candies[l];
        }

        left_sum += candies[l];
        mod(left_sum);

        ll compliment = (k - left_sum) % k;

        result += getCount(compliment);
    }
    return result;
}

ll bruteForce(vector<int> &candies, int l_bound, int r_bound)
{
    ll max, min;
    ll sum = 0;
    ll result = 0;

    for (int i = l_bound; i <= r_bound; i++)
    {
        sum = candies[i];
        max = candies[i];
        min = candies[i];
        for (int j = i + 1; j <= r_bound; j++)
        {
            if (candies[j] > max)
                max = candies[j];

            if (candies[j] < min)
                min = candies[j];

            sum += candies[j];

            if ((sum - max - min) % k == 0)
            {
                result++;
            }
        }
    }

    return result;
}

ll findPair(vector<int> &candies, int l, int r)
{
    //Base case
    if (r - l < 20)
    {
        return bruteForce(candies, l, r);
    }

    //D&C
    ll result = 0;
    int m = (l + r) / 2;
    result += findPair(candies, l, m);
    result += findPair(candies, m + 1, r);
    result += findCrossPair(candies, l, r);

    return result;
}

char BufferRead[1 << 15];
int rLen = 0, rPos = 0;
inline char Getchar()
{
    if (rPos == rLen)
        rPos = 0, rLen = fread(BufferRead, 1, 1 << 15, stdin);
    if (rPos == rLen)
        return EOF;
    return BufferRead[rPos++];
}

template <typename _TpInt>
inline _TpInt read()
{
    int flag = 1;
    char c = Getchar();
    while ((c > '9' || c < '0') && c != '-')
        c = Getchar();
    if (c == '-')
        flag = -1, c = Getchar();
    _TpInt init = (c & 15);
    while ((c = Getchar()) <= '9' && c >= '0')
        init = (init << 3) + (init << 1) + (c & 15);
    return init * flag;
}
template <typename _TpInt>
inline void write(_TpInt x)
{
    if (x < 0)
    {
        putchar('-');
        write<_TpInt>(~x + 1);
    }
    else
    {
        if (x > 9)
            write<_TpInt>(x / 10);
        putchar(x % 10 + '0');
    }
}

int main()
{
    int n;
    cin >> n >> k;

    vector<int> candies(n, 0);

    for (int i = 0; i < n; i++)
    {
        candies[i] = read<int>();
        //cin >> candies[i];
    }

    write<ll>(findPair(candies, 0, n - 1));

    return 0;
}