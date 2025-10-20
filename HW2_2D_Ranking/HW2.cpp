#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip> // setprecision
using namespace std;
#define F first
#define S second

void HeapSort(vector<pair<float, float>>&);
void heapify(vector<pair<float, float>>&, int, int);
void ranking_production(vector<pair<float, float>>&, vector<int>&, int, int);

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
void ranking_production(vector<pair<float, float>> &points, vector<int> &rank, int L, int R) {
    int len = R-L+1;
    if (len < 2) return;
    
    int mid = L + (len + 1) / 2;  // 偶數長度時取後，為了 len = 2, split < L 狀況
    float median_x = points[mid].F; // x 中位數

    int split; // 最後一個 x 小於中位數的索引
    for (split = L; split <= R; split++) {
        if (points[split].F >= median_x) break;
    }
    split--;

    // 所有 x = median_x，直接結束
    if (split < L) return;

    ranking_production(points, rank, L, split);
    ranking_production(points, rank, split+1, R);

    vector<pair<float, float>> L_points, R_points;
    // 左右集合，y 座標, idx
    for (int i = L; i <= split; i++) {
        L_points.push_back({points[i].S, i});
    }
    for (int i = split+1; i <= R; i++) {
        R_points.push_back({points[i].S, i});
    }
    // 根據 y 做升序排序
    HeapSort(L_points); HeapSort(R_points);

    for (int i = 0, j = 0; j < R_points.size(); j++) {
        while (i < L_points.size() && L_points[i].F < R_points[j].F) i++;
        rank[R_points[j].S] += i;
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

    // 根據 x 做 HeapSort，升序(最小堆)
    HeapSort(points);

    // 各點排名
    int n = points.size();
    vector<int> rank(n, 0);
    ranking_production(points, rank, 0, n-1);

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