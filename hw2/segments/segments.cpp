#include <stdio.h>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

#define MAXN 200005
#define MAXM 200005

int M = 200030;
int n, m;
long long k;


inline void _ins(int x, long long y, long long arr[])
{

    while (x <= M)
    {
        arr[x] += y;
        x += x & -x;
    }
}

inline void reset(long long arr[])
{

    for (int i = M; i >= 0; --i)
        arr[i] = 0;
}

inline void add(int l, int r, long long y, long long arr[])
{
    _ins(l, y, arr);
    _ins(r + 1, -y, arr);
}

inline long long get(int x, long long arr[])
{
    long long ret = 0;
    while (x)
        ret += arr[x], x -= x & -x;
    return ret;
}

class Segment
{
public:
    int left, right;
    long long num, used;
};

int cmp_left(const Segment *x, const Segment *y)
{
    return (x->left < y->left);
}

struct cmp_right
{
    bool operator()(const Segment *x, const Segment *y)
    {
        return x->right < y->right;
    }
};

struct cmp_by_r2
{
    bool operator()(const pair<int, long long> &x, const pair<int, long long> &y)
    {
        return x.first < y.first;
    }
};

bool check(long long required, long long n_seg_can_use, Segment *sort_seg[], long long arr[], bool ans)
{
    priority_queue< pair<int, long long>, vector<pair<int, long long>>, cmp_by_r2> seg;
    vector<Segment *> used;
    reset(arr);

    int seg_index = 0;
    for (int start = 0; start < m; ++start)
    {
        long long cur_required = required - get(start + 1, arr);
        if (cur_required <= 0)
            continue;

        for (; seg_index < n; ++seg_index)
        {
            if (sort_seg[seg_index]->left <= start)
                seg.push(make_pair(sort_seg[seg_index]->right, sort_seg[seg_index]->num));
            else
                break;
        }

        while (!seg.empty() && cur_required > 0)
        {
            int r = seg.top().first;
            long long amount = seg.top().second;
            seg.pop();

            if (r <= start)
                break;

            long long n_consume = min(cur_required, amount);

            n_seg_can_use -= n_consume;
            cur_required -= n_consume;
            amount -= n_consume;
            add(start + 1, r, n_consume, arr);

            if (amount > 0)
                seg.push(make_pair(r, amount));
        }
        if (n_seg_can_use < 0 || cur_required > 0)
            return false;
    }
    return true;
}

bool getans(long long required, long long n_can_use, Segment *sort_seg[], long long arr[], bool ans)
{
    priority_queue<Segment *, vector<Segment *>, cmp_right> seg;
    vector<Segment *> used;
    reset(arr);
    for (int i = 0; i < n; i++)
        sort_seg[i]->used = 0;

    int seg_index = 0;
    for (int start = 0; start < m; ++start)
    {
        long long cur_required = required - get(start + 1, arr);
        if (cur_required <= 0)
            continue;

        for (; seg_index < n; ++seg_index)
        {
            if (sort_seg[seg_index]->left <= start)
                seg.push(sort_seg[seg_index]);
            else
                break;
        }

        while (!seg.empty() && cur_required > 0)
        {
            Segment *cur_seg = seg.top();
            seg.pop();

            if (cur_seg->right <= start)
                break;

            long long consume = min(cur_required, cur_seg->num - cur_seg->used);
            n_can_use -= consume;
            cur_required -= consume;
            cur_seg->used += consume;
            add(cur_seg->left + 1, cur_seg->right, consume, arr);

            if (cur_seg->num - cur_seg->used > 0)
                seg.push(cur_seg);
        }
        if (n_can_use < 0 || cur_required > 0)
            return false;
    }
    return true;
}

int main()
{
    Segment *sorted[MAXN];
    Segment seg[MAXN];
    scanf("%d%d%lld", &n, &m, &k);
    M = m + 3;

    for (int i = 0; i < n; ++i)
    {
        seg[i].used = 0;
        scanf("%d%d%lld", &seg[i].left, &seg[i].right, &seg[i].num);
        sorted[i] = &seg[i];
    }
    sort(sorted, sorted + n, cmp_left);

    long long l = 1, r = k;
    long long mid = l + (r - l) / 2, ans = 0;

    long long arr[MAXM];
    while (r - l >= 0)
    {
        mid = l + (r - l) / 2;
        if (check(mid, k, sorted, arr, false))
        {
            ans = mid;
            l = mid + 1;
        }
        else
            r = mid - 1;
    }

    printf("%lld\n", ans);
    getans(ans, k, sorted, arr, true);
    for (int i = 0; i < n; ++i)
        printf("%lld ", seg[i].used);
    printf("\n");
}