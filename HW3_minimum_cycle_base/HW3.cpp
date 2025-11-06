#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 讀寫檔案
    freopen("cycle_ex/txt1.txt", "r", stdin);
    // freopen("cycle.txt","w", stdout);
    
    int nodenum, edgenum; // 點與邊數量
    cin >> nodenum >> edgenum;
    // cout << nodenum << " " << edgenum << endl;

    // 製作圖
    vector<vector<int>> graph(nodenum, vector<int>(nodenum, 0));
    int begin, end, cost;
    while (cin >> begin >> end >> cost) {
        graph[begin][end] = 1;
        graph[end][begin] = 1;
    }
    /*
    for (int i = 0; i < nodenum; i++) {
        for (int j = 0; j < nodenum; j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    */

    // 製作路線
    vector<vector<int>> lines;  // 目前路線
    for (int i = 0; i < nodenum; i++) { // 依序遍歷所有點
        for (int j = 0; j < lines.size(); j++) {    // 遍歷所有路線
            if (graph[lines[j].back()][i] == 1) {   // 該路線末可以連到 i
                lines.push_back(lines[j]);          // 複製一份
                lines.back().push_back(i);          // 將複製的連到 i
            }
        }
        lines.push_back({i}); // 加入做為起點
    }
    /*
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            cout << lines[i][j] << " ";
        }
        cout << endl;
    }
    */

    // 依路線完成環
    vector<vector<int>> cycles;
    for (int i = 0; i < lines.size(); i++) { // 遍歷所有路線
        if (lines[i].size() < 3) continue;  // 避免折返
        // 路線末可以走到路線初
        if (graph[lines[i].back()][lines[i].front()] == 1) {
            lines[i].push_back(lines[i].front());
            cycles.push_back(lines[i]);
        }
    }
    /*
    for (int i = 0; i < cycles.size(); i++) {
        for (int j = 0; j < cycles[i].size(); j++) {
            cout << cycles[i][j] << " ";
        }
        cout << endl;
    }
    */
    
    
    
    return 0;
}