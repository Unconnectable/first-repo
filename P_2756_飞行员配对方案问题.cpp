#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;
#define int long long 
const int inf = LLONG_MAX;  // 使用长整型的最大值
const int N = 2e6 + 9;  // 根据需要调整

int m, src, sink, n;
int ans, dist[N];
int idx = 1, now[N], head[N];

struct node {
    int to, next;
    int capacity;
} e[N];

void add(int u, int v, int val) {
    e[++idx] = {v, head[u], val}; 
    head[u] = idx;
    e[++idx] = {u, head[v], 0}; 
    head[v] = idx;
}

int bfs() {
    fill(dist, dist + n + 2, inf);
    queue<int> q;
    q.push(src);
    dist[src] = 0;
    now[src] = head[src];
    
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int i = head[x]; i != 0; i = e[i].next) {
            int v = e[i].to;
            if (e[i].capacity > 0 && dist[v] == inf) {
                q.push(v);
                now[v] = head[v];
                dist[v] = dist[x] + 1;
                if (v == sink) 
                    return true;
            }
        }
    }
    return false;
}

int dfs(int x, int sum) {
    if (x == sink) return sum;
    
    int k, res = 0;
    for (int i = now[x]; i && sum; i = e[i].next) {
        now[x] = i;
        int v = e[i].to;
        if (e[i].capacity > 0 && dist[v] == dist[x] + 1) {
            k = dfs(v, min(sum, e[i].capacity));
            if (k == 0) dist[v] = inf;
            e[i].capacity -= k;
            e[i ^ 1].capacity += k;
            res += k;
            sum -= k;
        }
    }
    return res;
}

int dinic() {
    while (bfs()) {
        ans += dfs(src, inf);
    }
    return ans;
}

signed main() {
    cin >> m >> n;
    src = 0;  // 源点
    sink = n + 1;  // 汇点
    
    // 从源点连接到所有外籍飞行员（编号 1 到 m）
    for (int i = 1; i <= m; i++) {
        add(src, i, 1);
    }
    
    // 从所有英国飞行员（编号 m+1 到 n）连接到汇点
    for (int i = m + 1; i <= n; i++) {
        add(i, sink, 1);
    }
    
    int u, v;
    while (cin >> u >> v) {
        if (u == -1 && v == -1) break;
        add(u, v, 1); // 外籍飞行员 u 与英国飞行员 v 的匹配
    }
    
    cout<<dinic()<<"\n"; // 执行 Dinic 算法，计算最大匹配

    // 输出实际匹配的边
    for (int i = 2; i <= idx; i += 2) {
        // 只有当正向边的容量为0且反向边的容量大于0时，表示这条边在最大流中被使用
        if (e[i].to != src && e[i].to != sink && e[i^1].capacity == 0) {
            cout << e[i^1].to << " " << e[i].to << endl;
        }
    }
    
    return 0;
}
