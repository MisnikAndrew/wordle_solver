#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

int word_size = 4;
/* Best words (based on popularity)
 * 11 - оперативник
 * 10 - созревание
 * 9 - самолетик
 * 8 - соратник
 * 7 - каротин
 * 6 - контра
 * 5 - норка
 * 4 - тора
 * */
int find_best_mode_old = 0;
string start_word_old = "норка";
int find_best_mode_newer = 0;
string start_word_newer = "тора";

vector<int> unused(33);
vector<int> used(33);
string al;

vector<string> dict;
vector<int> freq;
vector<vector<int>> pos(33, vector<int>(word_size));

vector<int> unused1(33);
vector<int> used1(33);
vector<vector<int>> pos1(33, vector<int>(word_size));

/*---------------------------------------------------------------*/
/* supporting functions */

int get_num(char c1, char c2)
{
    for (int k = 0; k < al.size(); k += 2)
    {
        if (al[k] == c1 && al[k + 1] == c2)
        {
            return k / 2;
        }
    }
    return -1;
}

vector<int> find_num(string s)
{
    vector<int> ans(33);
    for (int i = 0; i < s.size(); i += 2)
    {
        int ind = get_num(s[i], s[i + 1]);
        ans[ind] = 1;
    }
    return ans;
}

vector<int> get_mask(string guess, string word)
{
    vector<int> ans(word_size);
    vector<int> g1(word_size);
    vector<int> w1(word_size);
    for (int i = 0; i < word_size; i++)
        g1[i] = get_num(guess[2 * i], guess[2 * i + 1]);
    for (int i = 0; i < word_size; i++)
        w1[i] = get_num(word[2 * i], word[2 * i + 1]);
    for (int i = 0; i < word_size; i++)
        if (g1[i] == w1[i])
        {
            ans[i] = 2;
            g1[i] = 100 + i;
            w1[i] = 200 + i;
        }
    for (int i = 0; i < word_size; i++)
    {
        for (int j = 0; j < word_size; j++)
        {
            if (g1[i] == w1[j])
            {
                ans[i] = 1;
            }
        }
    }
    return ans;
}

void init_dict()
{
    string words;
    cin >> al;
    while (cin >> words)
    {
        int cur_freq;
        cin >> cur_freq;
        if (words.size() == 2 * word_size)
        {
            dict.push_back(words);
            freq.push_back(cur_freq);
        }
    }
}

void fix_dict()
{
    vector<string> dict1;
    vector<int> freq1;
    for (int i = 0; i < dict.size(); i++)
    {
        int check = 1;
        for (int j = 0; j < dict[i].size(); j += 2)
        {
            int uu = get_num(dict[i][j], dict[i][j + 1]);
            if (uu < 0 || uu > 32)
            {
                check = 0;
            }
        }
        if (check)
        {
            dict1.push_back(dict[i]);
            freq1.push_back(freq[i]);
        }
    }
    dict = dict1;
    freq = freq1;
}

template <typename T1>
ostream &operator<<(std::ostream &out, const vector<T1> &b)
{
    for (int i = 0; i < b.size(); i++)
    {
        out << b[i] << " ";
    }
    out << "\n";
    return out;
}

template <typename T1>
void sort(vector<T1> &m)
{
    sort(m.begin(), m.end());
}

template <typename T1>
void reverse(vector<T1> &m)
{
    reverse(m.begin(), m.end());
}

/*---------------------------------------------------------------*/
/* console new algo (based on popularity) */

void dict_update()
{
    vector<string> dict1;
    vector<int> freq1;
    for (int i = 0; i < dict.size(); i++)
    {
        string s = dict[i];
        int check = 1;
        for (int j = 0; j < s.size(); j += 2)
        {
            int u = get_num(s[j], s[j + 1]);
            if (unused[u] == 1 && pos[u][j / 2] != 1)
            {
                check = 0;
            }
            if (pos[u][j / 2] == -1)
            {
                check = 0;
            }
        }
        vector<int> find_b(33);
        for (int j = 0; j < s.size(); j += 2)
        {
            find_b[get_num(s[j], s[j + 1])] = 1;
        }
        for (int j = 0; j < 33; j++)
        {
            if (find_b[j] == 0 && used[j] == 1)
            {
                check = 0;
            }
        }
        if (check)
        {
            dict1.push_back(dict[i]);
            freq1.push_back(freq[i]);
        }
    }
    dict = dict1;
    freq = freq1;
}

int get_new_prior(int num)
{
    string s = dict[num];
    vector<vector<int>> masks;
    map<vector<int>, int> num_masks;
    for (auto &j : dict)
    {
        vector<int> cur = get_mask(s, j);
        num_masks[cur]++;
        masks.push_back(cur);
    }

    int ans = 0;
    for (int j = 0; j < dict.size(); j++)
    {
        vector<int> cur = masks[j];
        if (j != num)
        {
            ans += freq[j] * num_masks[cur];
        }
    }
    return ans;
}

void get_best_word_new()
{
    vector<pair<int, string>> new1;
    new1.reserve(dict.size());
    for (int i = 0; i < dict.size(); i++)
    {
        new1.emplace_back(get_new_prior(i), dict[i]);
    }
    sort(new1);
    cout << dict.size() << "\n"
         << dict;
    for (int i = 0; i < min(5, (int)new1.size()); i++)
    {
        cout << new1[i].first << " " << new1[i].second << "\n";
    }
}

// 0 - unused, 1 - cow, 2 - bull
void update(string s, vector<int> a)
{
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 0)
        {
            unused[get_num(s[2 * j], s[2 * j + 1])] = 1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 1)
        {
            used[get_num(s[2 * j], s[2 * j + 1])] = 1;
            unused[get_num(s[2 * j], s[2 * j + 1])] = -1;
            pos[get_num(s[2 * j], s[2 * j + 1])][j] = -1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 2)
        {
            int b = get_num(s[2 * j], s[2 * j + 1]);
            used[b] = 0;
            pos[b][j] = 1;
            for (int i = 0; i < 33; i++)
            {
                if (i != b)
                {
                    pos[i][j] = -1;
                }
            }
        }
    }
    dict_update();
    get_best_word_new();
}

/*---------------------------------------------------------------*/
/* dictionary decrease checker (without changing) */

int check_update_dict_size()
{
    vector<string> dict1;
    for (auto &i : dict)
    {
        string s = i;
        int check = 1;
        for (int j = 0; j < s.size(); j += 2)
        {
            int u = get_num(s[j], s[j + 1]);
            if (unused1[u] == 1)
            {
                check = 0;
            }
            if (pos1[u][j / 2] == -1)
            {
                check = 0;
            }
        }
        vector<int> find_b(33);
        for (int j = 0; j < s.size(); j += 2)
        {
            find_b[get_num(s[j], s[j + 1])] = 1;
        }
        for (int j = 0; j < 33; j++)
        {
            if (find_b[j] == 0 && used1[j] == 1)
            {
                check = 0;
            }
        }
        if (check)
        {
            dict1.push_back(i);
        }
    }
    return (int)dict1.size();
}
void check_update(string s, vector<int> a)
{
    unused1 = unused;
    used1 = used;
    pos1 = pos;
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 0)
        {
            unused1[get_num(s[2 * j], s[2 * j + 1])] = 1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 1)
        {
            used1[get_num(s[2 * j], s[2 * j + 1])] = 1;
            unused1[get_num(s[2 * j], s[2 * j + 1])] = -1;
            pos1[get_num(s[2 * j], s[2 * j + 1])][j] = -1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 2)
        {
            int b = get_num(s[2 * j], s[2 * j + 1]);
            unused1[b] = -1;
            used1[b] = 1;
            pos1[b][j] = 1;
            for (int i = 0; i < 33; i++)
            {
                if (i != b)
                {
                    pos1[i][j] = -1;
                }
            }
        }
    }
    cout << check_update_dict_size();
}

/*---------------------------------------------------------------*/
/* non - console old algo (based on letters probability) */
vector<int> probs_old(33);
vector<string> dict_old;
vector<vector<int>> pos_old(33, vector<int>(word_size));
vector<int> unused_old(33);
vector<int> used_old(33);

void get_probs_old()
{
    for (int i = 0; i < 33; i++)
    {
        probs_old[i] = 0;
    }
    for (auto &i : dict_old)
    {
        for (int j = 0; j < i.size(); j += 2)
        {
            int ind = get_num(i[j], i[j + 1]);
            {
                probs_old[ind]++;
            }
        }
    }
}

void old_dict_update()
{
    vector<string> dict1;
    for (auto &i : dict_old)
    {
        string s = i;
        int check = 1;
        for (int j = 0; j < s.size(); j += 2)
        {
            int u = get_num(s[j], s[j + 1]);
            if (unused_old[u] == 1 && pos_old[u][j / 2] != 1)
            {
                check = 0;
            }
            if (pos_old[u][j / 2] == -1)
            {
                check = 0;
            }
        }
        vector<int> find_b(33);
        for (int j = 0; j < s.size(); j += 2)
        {
            find_b[get_num(s[j], s[j + 1])] = 1;
        }
        for (int j = 0; j < 33; j++)
        {
            if (find_b[j] == 0 && used_old[j] == 1)
            {
                check = 0;
            }
        }
        if (check)
        {
            dict1.push_back(i);
        }
    }
    dict_old = dict1;
}

int get_prior_old(string s)
{
    vector<int> ans = find_num(std::move(s));
    int ret = 0;
    for (int i = 0; i < 33; i++)
    {
        ret += ans[i] * probs_old[i];
    }
    return ret;
}

string get_best_word_old()
{
    vector<pair<int, string>> my_dict;
    get_probs_old();
    my_dict.reserve(dict_old.size());
    for (auto &i : dict_old)
    {
        my_dict.emplace_back(get_prior_old(i), i);
    }
    sort(my_dict);
    reverse(my_dict);
    return my_dict[0].second;
}

void update_old(string s, vector<int> a)
{
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 0)
        {
            unused_old[get_num(s[2 * j], s[2 * j + 1])] = 1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 1)
        {
            used_old[get_num(s[2 * j], s[2 * j + 1])] = 1;
            unused_old[get_num(s[2 * j], s[2 * j + 1])] = -1;
            pos_old[get_num(s[2 * j], s[2 * j + 1])][j] = -1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 2)
        {
            int b = get_num(s[2 * j], s[2 * j + 1]);
            used_old[b] = 0;
            pos_old[b][j] = 1;
            for (int i = 0; i < 33; i++)
            {
                if (i != b)
                {
                    pos_old[i][j] = -1;
                }
            }
        }
    }
    old_dict_update();
}
void init_old()
{
    dict_old = dict;
    pos_old = pos;
    unused_old = unused;
    used_old = used;
}

int play_old(const string &s)
{
    init_old();
    if (find_best_mode_old == 1)
    {
        start_word_old = get_best_word_old();
        cout << start_word_old << "\n";
    }
    string start_word = start_word_old;
    vector<int> ans = get_mask(start_word, s);
    update_old(start_word, ans);
    get_probs_old();
    int cur_ans = 1;

    while (dict_old.size() > 1 && cur_ans < 30)
    {
        string new_guess = get_best_word_old();
        ans = get_mask(new_guess, s);
        update_old(new_guess, ans);
        cur_ans++;
    }
    if (cur_ans > 20)
    {
        cout << "infinite loop on word " << s << "\n";
    }
    return cur_ans + 1;
}

void play_all_old()
{
    int sum = 0;
    int sum_freq = 0;
    for (int i = 0; i < dict.size(); i++)
    {
        int cur = play_old(dict[i]);
        sum += cur * freq[i];
        sum_freq += freq[i];
        if (i % 100 == 0)
        {
            cout << i << " iterations of " << dict.size() << " done\n";
        }
    }
    cout << "avg: " << (long double)sum / (long double)(sum_freq) << " moves\n";
}

/*---------------------------------------------------------------*/
/* non - console new algo (based on popularity) */
vector<string> dict_newer;
vector<int> freq_newer;
vector<vector<int>> pos_newer(33, vector<int>(word_size));
vector<int> unused_newer(33);
vector<int> used_newer(33);

int get_new_prior_newer(int num)
{
    string s = dict_newer[num];
    vector<vector<int>> masks;
    map<vector<int>, int> num_masks;
    for (auto &j : dict_newer)
    {
        vector<int> cur = get_mask(s, j);
        num_masks[cur]++;
        masks.push_back(cur);
    }

    int ans = 0;
    for (int j = 0; j < dict_newer.size(); j++)
    {
        vector<int> cur = masks[j];
        if (j != num)
        {
            ans += freq_newer[j] * num_masks[cur];
        }
    }
    return ans;
}

string get_best_word_newer()
{
    vector<pair<int, string>> new1;
    new1.reserve(dict_newer.size());
    for (int i = 0; i < dict_newer.size(); i++)
    {
        new1.emplace_back(get_new_prior_newer(i), dict_newer[i]);
    }
    string ans = new1[0].second;
    int min1 = new1[0].first;
    for (auto &i : new1)
    {
        if (i.first < min1)
        {
            min1 = i.first;
            ans = i.second;
        }
    }
    return ans;
}

void newer_dict_update()
{
    vector<string> dict1;
    vector<int> freq1;
    for (int i = 0; i < dict_newer.size(); i++)
    {
        string s = dict_newer[i];
        int check = 1;
        for (int j = 0; j < s.size(); j += 2)
        {
            int u = get_num(s[j], s[j + 1]);
            if (unused_newer[u] == 1 && pos_newer[u][j / 2] != 1)
            {
                check = 0;
            }
            if (pos_newer[u][j / 2] == -1)
            {
                check = 0;
            }
        }
        vector<int> find_b(33);
        for (int j = 0; j < s.size(); j += 2)
        {
            find_b[get_num(s[j], s[j + 1])] = 1;
        }
        for (int j = 0; j < 33; j++)
        {
            if (find_b[j] == 0 && used_newer[j] == 1)
            {
                check = 0;
            }
        }
        if (check)
        {
            dict1.push_back(dict_newer[i]);
            freq1.push_back(freq_newer[i]);
        }
    }
    dict_newer = dict1;
    freq_newer = freq1;
}

void update_newer(string s, vector<int> a)
{
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 0)
        {
            unused_newer[get_num(s[2 * j], s[2 * j + 1])] = 1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 1)
        {
            used_newer[get_num(s[2 * j], s[2 * j + 1])] = 1;
            unused_newer[get_num(s[2 * j], s[2 * j + 1])] = -1;
            pos_newer[get_num(s[2 * j], s[2 * j + 1])][j] = -1;
        }
    }
    for (int j = 0; j < word_size; j++)
    {
        if (a[j] == 2)
        {
            int b = get_num(s[2 * j], s[2 * j + 1]);
            used_newer[b] = 0;
            pos_newer[b][j] = 1;
            for (int i = 0; i < 33; i++)
            {
                if (i != b)
                {
                    pos_newer[i][j] = -1;
                }
            }
        }
    }
    newer_dict_update();
}

void init_newer()
{
    dict_newer = dict;
    freq_newer = freq;
    pos_newer = pos;
    unused_newer = unused;
    used_newer = used;
}
int play_newer(const string &s)
{
    init_newer();

    if (find_best_mode_newer == 1)
    {
        start_word_newer = get_best_word_newer();
        cout << start_word_newer << "\n";
    }
    string start_word = start_word_newer;
    vector<int> ans = get_mask(start_word, s);
    update_newer(start_word, ans);
    int cur_ans = 1;
    while (dict_newer.size() > 1 && cur_ans < 30)
    {
        string new_guess = get_best_word_newer();
        ans = get_mask(new_guess, s);
        cout << dict_newer.size() << " " << new_guess << "\n";
        update_newer(new_guess, ans);
        cur_ans++;
    }
    if (cur_ans > 20)
    {
        cout << "infinite loop on word " << s << "\n";
    }
    return cur_ans + 1;
}

void play_all_newer()
{
    int sum = 0;
    int sum_freq = 0;
    for (int i = 0; i < dict.size(); i++)
    {
        int cur = play_newer(dict[i]);
        sum += cur * freq[i];
        sum_freq += freq[i];
        if (i % 100 == 0)
        {
            cout << i << " iterations of " << dict.size() << " done\n";
        }
    }
    cout << "avg: " << (long double)sum / (long double)(sum_freq) << " moves\n";
}

/*---------------------------------------------------------------*/

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    freopen("to_cout.txt", "r", stdin);
    init_dict();
    fix_dict();
    init_old();
    init_newer();
    cout << play_old("виза");
    return 0;

    // play_all_newer();
}