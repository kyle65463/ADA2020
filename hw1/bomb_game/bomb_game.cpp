#include <algorithm>
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

int N;
int *arr;

void _ins(int x, int k)
{
    while (x <= N)
        arr[x] += k, x += x & -x;
}

long long _qr(int x)
{
    long long ret = 0;
    while (x)
        ret += arr[x], x -= x & -x;
    return ret;
}

void reset()
{
    for (int i = N; i >= 0; --i)
        arr[i] = 0;
}

void add(int l, int r, int k)
{
    //assert(1 <= l && l <= r && r <= N && "add: the argument should satisfy 1 <= l <= r <= N");
    _ins(l, k);
    _ins(r + 1, -k);
}

long long get(int x)
{
    assert(1 <= x && x <= N && "get: x should be a positive integer in range [1, N]");
    return _qr(x);
}

class Attack
{
public:
    Attack(int l, int r, int power, int damage) : l(l), r(r), power(power), damage(damage) {}
    int l;
    int r;
    int power;
    int damage;
};

class Player
{
public:
    Player(int pos, int defense) : pos(pos), defense(defense), damage(0) {}
    int pos;
    int defense;
    int damage;
    int init_damage;
};

class Event
{
public:
    Event(char type, Attack *attack, Player *player) : type(type), attack(attack), player(player) {}
    char type;
    Attack *attack;
    Player *player;
};

class Result
{
public:
    vector<Attack *> attack;
    vector<Player *> player;
};

int cmp(Attack *a, Attack *b)
{
    return a->power > b->power;
}

void mergeAttack(vector<Attack *> &l, vector<Attack *> &r, vector<Attack *> &target)
{
    int l_count = 0, r_count = 0;
    while (l_count < l.size() && r_count < r.size())
    {
        if (l[l_count]->power < r[r_count]->power)
            target.push_back(r[r_count++]);
        else
            target.push_back(l[l_count++]);
    }
    while (l_count < l.size())
        target.push_back(l[l_count++]);
    while (r_count < r.size())
        target.push_back(r[r_count++]);
}

void mergePlayer(vector<Player *> &l, vector<Player *> &r, vector<Player *> &target)
{
    int l_count = 0, r_count = 0;
    while (l_count < l.size() && r_count < r.size())
    {
        if (l[l_count]->defense < r[r_count]->defense)
            target.push_back(r[r_count++]);
        else
            target.push_back(l[l_count++]);
    }
    while (l_count < l.size())
        target.push_back(l[l_count++]);
    while (r_count < r.size())
        target.push_back(r[r_count++]);
}

Result bombGame(vector<Event> &event, int l, int r)
{
    int mid = (l + r) / 2;
    Result result;
    if (l == r)
    {
        if (event[l].type == 'A')
            result.attack.push_back(event[l].attack);
        else if (event[l].type == 'P')
            result.player.push_back(event[l].player);
        return result;
    }

    Result result_l = bombGame(event, l, mid);
    Result result_r = bombGame(event, mid + 1, r);

    //reset();

    int index = 0;
    for (int i = 0; i < result_l.player.size(); i++)
    {
        result_l.player[i]->init_damage = get(result_l.player[i]->pos);
    }
    for (int i = 0; i < result_l.player.size(); i++)
    {

        while (index < result_r.attack.size() && result_r.attack[index]->power >= result_l.player[i]->defense)
        {
            add(result_r.attack[index]->l, result_r.attack[index]->r, result_r.attack[index]->damage);
            index++;
        }

        result_l.player[i]->damage += (get(result_l.player[i]->pos) - result_l.player[i]->init_damage);
    }

    mergeAttack(result_l.attack, result_r.attack, result.attack);
    mergePlayer(result_l.player, result_r.player, result.player);
    return result;
}

int main()
{
    //std::ios_base::sync_with_stdio(false);
    //std::cin.tie(nullptr);
    int n, m;
    scanf("%d%d", &N, &m);
    //cin >> n >> m;
    arr = new int[N]();

    vector<Event> event;
    event.push_back(Event('N', NULL, NULL));
    for (int i = 0; i < m; i++)
    {
        char type;
        cin >> type;

        switch (type)
        {
        case 'A':
            int l, r, power, damage;
            //cin >> l >> r >> power >> damage;
            scanf("%d%d%d%d", &l, &r, &power, &damage);
            event.push_back(Event(type, new Attack(l, r, power, damage), NULL));
            break;
        case 'P':
            int pos, defense;
            cin >> pos >> defense;
            scanf("%d%d", &pos, &defense);
            event.push_back(Event(type, NULL, new Player(pos, defense)));
            break;
        }
    }

    Result result = bombGame(event, 1, m);

    for (int i = 1; i <= m; i++)
    {
        if (event[i].type == 'P')
            printf("%d\n", event[i].player->damage);
        //cout << event[i].player->damage << endl;
    }
}