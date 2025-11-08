#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

int nodenum, edgenum = 0;   // 點與邊數量
vector<vector<bool>> graph; // 路線權重表
vector<int> path;           // 目前路徑
vector<bool> visited;       // 已使用點
unordered_set<string> unique_cycles;  // 環集合

// 兩點映射到邊編號，edges_map[min, max] = edge_order
unordered_map<long long, int> edges_map;

// 將環做排序
bool cycle_cmp(const vector<int> &a, const vector<int> &b){
    // 最小的 cycle 先顯示
    if (a.size() != b.size()) return a.size() < b.size();

    // 以較小的節點優先輸出
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) return a[i] < b[i]; 
    }
    return false;
}

// 兩 int 組成 long long，做為 map 的 key
long long mix(int a, int b) {
    int p1 = min(a, b);
    int p2 = max(a, b);
    long long p = ((long long)p1 << 31) + p2;
    return p;
}

// 分割字串，轉成整數陣列
vector<int> split(string &str, char delimiter) {
    vector<int> result;
    stringstream ss(str);
    string tok;

    while (getline(ss, tok, delimiter)) {
        result.push_back(stoi(tok));
    }
    return result;
}

// 生成最小字典序的路徑之旋轉、反轉版本，用於統一格式
string canonical_form(const vector<int>& cycle) {
    // 移除最後重複起點 (例如 : 0 1 2 3 0 -> 0 1 2 3)
    vector<int> c(cycle.begin(), cycle.end() - 1);
    int sz = c.size();

    // 找出最小節點作為起點，生成最小字典序的旋轉版本
    vector<vector<int>> rotations;
    for (int i = 0; i < sz; i++) {
        vector<int> rot;
        for (int j = 0; j < sz; j++)
            rot.push_back(c[(i + j) % sz]);
        rotations.push_back(rot);
    }

    // 同時計算反向旋轉（例如 : 0 1 2 3 -> 3 2 1 0）
    vector<vector<int>> rev_rotations;
    vector<int> rev = c;
    reverse(rev.begin(), rev.end());
    for (int i = 0; i < sz; i++) {
        vector<int> rot;
        for (int j = 0; j < sz; j++)
            rot.push_back(rev[(i + j) % sz]);
        rev_rotations.push_back(rot);
    }

    rotations.insert(rotations.end(), rev_rotations.begin(), rev_rotations.end());
    sort(rotations.begin(), rotations.end());
    vector<int> smallest = rotations[0];

    // 轉成字串方便存 set
    stringstream ss;
    for (int x : smallest) ss << x << ",";
    string str = ss.str();
    str.pop_back(); // 去掉尾逗號
    return str;
}

// 遞迴找出所有簡單環 (simple cycle)
void dfs(int start, int u, int parent) {
    visited[u] = true;
    path.push_back(u);

    for (int v = 0; v < nodenum; v++) {
        if (!graph[u][v]) continue;
        if (v == parent) continue;

        if (!visited[v]) {
            dfs(start, v, u);
        } else if (v == start && path.size() > 2) {
            // 找到一個環
            path.push_back(start);
            string key = canonical_form(path);
            unique_cycles.insert(key); // 自動去重
            path.pop_back();// 回溯
        }
    }
    // 回溯
    path.pop_back();
    visited[u] = false;
}

int main() {
    // 讀寫檔案
    // freopen("txt4.txt", "r", stdin);
    freopen("cost239", "r", stdin);
    freopen("cycle.txt","w", stdout);

    // 讀入點數量，支援無「邊數量」之測資
    string first_line;
    getline(cin, first_line);
    int cut = first_line.find(' ');
    if (cut != -1) nodenum = stoi(first_line.substr(0, cut));
    else nodenum = stoi(first_line);

    graph.assign(nodenum, vector<bool>(nodenum, false));
    int a, b, w;
    while (cin >> a >> b >> w) {
        graph[a][b] = graph[b][a] = true;

        long long key = mix(a, b);
        if (edges_map.count(key)) continue;
        edges_map[key] = edgenum;
        edgenum++;
    }
    /*
    cerr << "nodenum : " << nodenum << ", edgenum : " << edgenum << endl;
    cerr << "edges(node1, node2, edge_order) : \n";
    for(auto m : edges_map) {
        long long node_ll = m.first;
        int p1 = node_ll >> 31;
        int p2 = node_ll - (p1 << 31);
        cerr << p1 << " " << p2 << " " << m.second << endl;
    }
    */

    // 遞迴找出所有簡單環 (simple cycle)
    for (int i = 0; i < nodenum; i++) {
        visited.assign(nodenum, false);
        dfs(i, i, -1); // 起點, 新增點, 最後一點
    }
    int cyclenum = unique_cycles.size();
    /*
    cerr << "cycles_num : " << cyclenum << endl;
    cerr << "cycles_string : " << endl;
    for (auto s : unique_cycles) {
        cerr << s << endl;
    }
    */

    // 將環字串還原
    vector<vector<int>> cycles;
    for (auto s : unique_cycles) {
        cycles.push_back(split(s, ',')); // 依 ',' 分割
    }
    // 將環做排序
    sort(cycles.begin(), cycles.end(), cycle_cmp);
    /*
    cerr << "cycles_nodes : \n";
    for (int i = 0; i < cycles.size(); i++) {
        for (int j = 0; j < cycles[i].size(); j++) {
            cerr << cycles[i][j] << " ";
        }
        cerr << endl;
    }
    */

    // 將環轉成邊集合
    vector<vector<int>> cycles_edges;
    for (auto cycle : cycles) {
        vector<int> edge;
        for (int i = 0; i < cycle.size(); i++) {
            int a = cycle[i];
            int b = cycle[(i+1) % cycle.size()];
            edge.push_back(edges_map[mix(a, b)]);
        }
        cycles_edges.push_back(edge);
    }
    /*
    cerr << "cycles_edges : \n";
    for (int i = 0; i < cycles_edges.size(); i++) {
        for (int j = 0; j < cycles_edges[i].size(); j++) {
            cerr << cycles_edges[i][j] << " ";
        }
        cerr << endl;
    }
    */
    
    // 將邊集合轉成布林陣列
    vector<vector<bool>> cycles_edges_bools(cyclenum, vector<bool>(edgenum, false));
    for (int i = 0; i < cyclenum; i++) {
        for (int j = 0; j < cycles_edges[i].size(); j++) {
            cycles_edges_bools[i][cycles_edges[i][j]] = true;
        }
    }
    /*
    cerr << "cycles_edges_bools : \n";
    for (int i = 0; i < cycles_edges_bools.size(); i++) {
        for (int j = 0; j < cycles_edges_bools[i].size(); j++) {
            cerr << cycles_edges_bools[i][j] << " ";
        }
        cerr << endl;
    }
    */

    // 依布林陣列做高斯消去，以取得 minimal cycle basis 最小環基
    vector<int> cycles_indexs(cyclenum); // 交換前位置
    for (int i = 0; i < cyclenum; i++) cycles_indexs[i] = i;
    for (int r = 0, c = 0; c < edgenum && r < cyclenum; c++) {
        int pivot = -1;
        // 從第 r 個環開始找，第一個有第 c 條邊的環
        for (int i = r; i < cyclenum; i++) {
            if (cycles_edges_bools[i][c]) {
                pivot = i;
                break;
            }
        }
        if (pivot == -1) continue;
        
        // 換到第 r 列
        swap(cycles_edges_bools[r], cycles_edges_bools[pivot]);
        swap(cycles_indexs[r], cycles_indexs[pivot]);

        for (int i = 0; i < cyclenum; i++) {
            if (i != r && cycles_edges_bools[i][c]) {
                // 其他也有第 c 條邊的環，與 r 做 XOR 以消去
                for (int j = c; j < edgenum; j++)
                    cycles_edges_bools[i][j] = cycles_edges_bools[i][j] ^ cycles_edges_bools[r][j];
            }
        }
        r++;
    }
    /*
    cerr << "Gaussian_result : \n";
    for (int i = 0; i < cycles_edges_bools.size(); i++) {
        for (int j = 0; j < cycles_edges_bools[i].size(); j++) {
            cerr << cycles_edges_bools[i][j] << " ";
        }
        cerr << endl;
    }
    */

    // 取得非零列其原本的環位置，並輸出
    vector<int> result_cycles_indexs;
    for (int i = 0; i < cycles_edges_bools.size(); i++) {
        for (int j = 0; j < cycles_edges_bools[i].size(); j++) {
            if (cycles_edges_bools[i][j]) {
                result_cycles_indexs.push_back(cycles_indexs[i]);
                break;
            }
        }
    }
    sort(result_cycles_indexs.begin(), result_cycles_indexs.end());
    if (result_cycles_indexs.size() != edgenum - nodenum + 1) {
        cerr << "ERROR!!!\n";
    }
    for (int i = 0; i < result_cycles_indexs.size(); i++) {
        auto &cycle = cycles[result_cycles_indexs[i]];
        for (int j = 0; j < cycle.size(); j++) {
            if (j != 0) cout << "->";
            cout << cycle[j];
        }
        cout << endl;
    }
    return 0;
}
