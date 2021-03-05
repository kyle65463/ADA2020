#include <bits/stdc++.h>
#include "helper.h"
using namespace std;

int target_size, target_sum;
bool used[10] = {0};
vector<int> ans;
vector<int> giant_matrix[1300];

void generate(int num, int size, int sum, vector<vector<int>> &total)
{
    if (num > 9)
        if (size != target_size)
            return;

    if (size == target_size)
    {
        if (sum != target_sum)
            return;

        int i;
        vector<int> tmp;

        for (i = 1; i <= 9; i++)
        {
            if (used[i] == false)
            {
                tmp.push_back(i);
            }
        }
        total.push_back(tmp);
    }

    if (sum + num > target_sum)
        return;

    used[num] = true;
    generate(num + 1, size + 1, sum + num, total);
    used[num] = false;
    generate(num + 1, size, sum, total);
    return;
}

int which(int i, int j)
{
    if (i <= 3)
    {
        if (j <= 3)
            return 1;
        if (j <= 6)
            return 2;
        if (j <= 9)
            return 3;
    }
    if (i <= 6)
    {
        if (j <= 3)
            return 4;
        if (j <= 6)
            return 5;
        if (j <= 9)
            return 6;
    }
    if (i <= 9)
    {
        if (j <= 3)
            return 7;
        if (j <= 6)
            return 8;
        if (j <= 9)
            return 9;
    }
}

int main()
{
    int sumtmp;
    char cagetmp;
    map<char, vector<int>> place_map;
    map<char, int> sum_map, index_map;
    char char_map[10][10];

    char c;
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            cin >> c;
            char_map[i][j] = c;
            place_map[c].push_back((i - 1) * 9 + j);
        }
    }
    int sum;
    for (int i = 0; i < place_map.size(); i++)
    {
        cin >> c >> sum;
        index_map[c] = i;
        sum_map[c] = sum;
    }

    int total_column = place_map.size() * 9 + 81 * 4;
    DLX::Init(total_column);
    
    int index = 1;
    for (int i = 1; i <= 9; i++)
    {
        for (int j = 1; j <= 9; j++)
        {
            for (int num = 1; num <= 9; num++)
            {
                vector<int> sol;

                sol.reserve(5);
                sol.push_back((i - 1) * 9 + j); // which cell is put
                sol.push_back(81 + (i - 1) * 9 + num);
                sol.push_back(162 + (j - 1) * 9 + num);
                sol.push_back(243 + (which(i, j) - 1) * 9 + num);
                sol.push_back(324 + index_map[char_map[i][j]] * 9 + num);

                DLX ::AddRow(index, sol);
                giant_matrix[index] = (sol);
                index++;
            }
        }
    }

    for (auto x : index_map)
    {
        vector<vector<int>> total;
        char c = x.first;
        int char_index = x.second;
        target_sum = sum_map[c];
        vector<int> places = place_map[c];
        target_size = places.size();
        if(target_size == 9)
            continue;
        generate(1, 0, 0, total);
        for (auto x : total)
        {
            vector<int> sol;

            for (auto y : x)
                sol.push_back(y + (char_index * 9) + 324);

            DLX ::AddRow(index, sol);
            index++;
        }

        for (int i = 0; i <= 10; i++)
            used[i] = false;
    }
    ans = DLX ::Solver();

    int arr_ans[82];
    for (auto x : ans)
    {
        if(x > 729)
            continue;

        int place = giant_matrix[x][0];
        arr_ans[place] = giant_matrix[x][1] % 9;

        if (arr_ans[place] == 0)
            arr_ans[place] = 9;
    }

    for (int i = 1; i <= 81; i++)
    {
        cout << arr_ans[i];
        if (i % 9 == 0)
            cout << endl;
    }
}