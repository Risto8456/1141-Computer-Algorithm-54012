// time O(n*log(n))
// 使用 merge sort + 保留已排序結果（避免每層都重新排序 y）
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip> // setprecision
using namespace std;
#define F first
#define S second

void HeapSort(vector<pair<float, float>>&);
void heapify(vector<pair<float, float>>&, int, int);
void ranking_production(vector<pair<float, float>>&, vector<pair<float, int>>&, vector<int>&, int, int);

// 堆積排序法
void HeapSort(vector<pair<float, float>> &points){
    int n = points.size();
    // 建構最大堆
    for(int i = n/2-1; i >= 0; i--){
        heapify(points, n, i);
    }
    // 建構完成
    
    // 將最大堆換為最小堆
    // 將根（索引=0）交換到數組末尾並刪除它
    for(int i = n-1; i>=0; i--){
        swap(points[0], points[i]);
        // 再次 heapify 以獲得新最大堆
        heapify(points, i, 0);
    }
}
void heapify(vector<pair<float, float>> &points, int n, int i){
    int rootIndex = i;
    int leftIndex = i*2 + 1;
    int rightIndex = i*2 + 2;

    if (leftIndex < n && points[leftIndex].F > points[rootIndex].F) rootIndex = leftIndex;
    if (rightIndex < n && points[rightIndex].F > points[rootIndex].F) rootIndex = rightIndex;
    // 檢查是否需要交換？
    if (rootIndex != i) {
        swap(points[i], points[rootIndex]);
        heapify(points, n, rootIndex);
    }
}
// 排名製作
void ranking_production(
    vector<pair<float, float>> &points, 
    vector<pair<float, int>> &y_idx, 
    vector<int> &rank, int L, int R) {

    int len = R-L+1;
    if (len < 2) return; // 長度小於 2
    if (points[L].F == points[R].F) return; // 所有 x 相同
    
    int mid = L + (len - 1) / 2;    // 偶數長度取前
    float median_x = points[mid].F; // x 中位數

    int split; // 第一個 x 大於中位數的索引, x = [1,1,1,2]
    for (split = L; split <= R; split++) {
        if (points[split].F > median_x) break;
    }
    if (split > R) { // 更改邏輯，第一個 x 大於等於中位數的索引, x = [1,2,2,2]
        for (split = L; split <= R; split++) {
            if (points[split].F >= median_x) break;
        }
    }

    ranking_production(points, y_idx, rank, L, split-1);
    ranking_production(points, y_idx, rank, split, R);

    // 左右集合，y 座標, idx
    vector<pair<float, int>> L_points, R_points;
    for (int i = L; i <= split-1; i++) {
        L_points.push_back(y_idx[i]);
    }
    for (int i = split; i <= R; i++) {
        R_points.push_back(y_idx[i]);
    }

    // 製作 rank
    int i = 0, j = 0;
    while (i < L_points.size() && j < R_points.size()) {
        if (L_points[i].F < R_points[j].F) {
            y_idx[L+i+j] = L_points[i]; // merge
            i++;
        } else {
            y_idx[L+i+j] = R_points[j]; // merge
            rank[R_points[j].S] += i;
            j++;
        }
    }
    while (i < L_points.size()) {
        y_idx[L+i+j] = L_points[i]; // merge
        i++;
    }
    while (j < R_points.size()) {
        y_idx[L+i+j] = R_points[j]; // merge
        rank[R_points[j].S] += i;
        j++;
    }
}

int main() {
    // 讀寫檔案
    freopen("test2.txt", "r", stdin);

    // 提升效能
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    
    // 定點表示法輸出浮點數，小數點後 2 位，四捨五入，不足補 0
    cout << fixed << setprecision(2);

    // 輸入各點座標
    vector<pair<float, float>> points;
    float x, y;
    while (cin >> x >> y) {
        points.push_back({x, y});
    }
    int n = points.size();

    // 根據 x 做 HeapSort，升序(最小堆)
    HeapSort(points);

    // merge sort 基底 : y 座標, idx
    vector<pair<float, int>> y_idx;
    for (int i = 0; i < n; i++) {
        y_idx.push_back({points[i].S, i});
    }

    // 各點排名
    vector<int> rank(n, 0);
    ranking_production(points, y_idx, rank, 0, n-1);

    for (int i = 0; i < n; i++) {
        cout << "x : " << setw(8) << points[i].F << "\t";
        cout << "y : " << setw(8) << points[i].S << "\t";
        cout << "rank : " << setw(4) << rank[i] << endl;
    }

    int maxRank = *max_element(rank.begin(), rank.end());
    int minRank = *min_element(rank.begin(), rank.end());
    float avgRank = 0;
    for (auto r : rank) avgRank += r;
    avgRank /= n;

    cout << "---------------------------------\n";
    cout << "Number of coordinate points: " << n << "\n";
    cout << "Maximum rank: " << maxRank << "\n";
    cout << "Minimum rank: " << minRank << "\n";
    cout << "Average rank: " << fixed << setprecision(2) << avgRank << "\n";
    return 0;
}