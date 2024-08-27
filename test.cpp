#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;
#define int long long 
const int  inf = 2005020600;
const int N = 2e6 + 9;

int m, src, sink, n;
int  ans, dist[N];
int idx = 1, now[N], head[N];

struct node {
    int to, next;
    int  capacity;
} e[N];

void add(int u, int v, int  val) {
    // 添加正向边
    e[++idx] = {v, head[u], val}; // 添加从u到v的边，容量为val
    head[u] = idx; // 更新u的链表头指针
    
    // 添加反向边
    e[++idx] = {u, head[v], 0}; // 添加从v到u的反向边，容量为0
    head[v] = idx; // 更新v的链表头指针
}

int bfs() {  // 在残量网络中构造分层图
    fill(dist, dist + n + 1, inf); // 初始化所有节点的距离为无穷大
    
    queue<int> q;
    q.push(src); // 从源点开始
    dist[src] = 0; // 源点的距离为0
    now[src] = head[src]; // 初始化当前弧
    
    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int i = head[x]; i!=0; i = e[i].next) {
            int v = e[i].to;
            if (e[i].capacity > 0 && dist[v] == inf) {
                q.push(v); // 将v加入队列
                now[v] = head[v]; // 更新v的当前弧
                dist[v] = dist[x] + 1; // 更新v的距离
                if (v == sink) 
                    return true; // 如果找到汇点，返回1
            }
        }
    }
    return false; // 如果没有找到汇点，返回0
}

int  dfs(int x, int  sum) {  // sum是整条增广路对最大流的贡献
    if (x == sink) return sum; // 如果到达汇点，返回流量
    
    int  k, res = 0; // k是当前最小的剩余容量
    for (int i = now[x]; i && sum; i = e[i].next) {
        now[x] = i; // 当前弧优化
        int v = e[i].to;
        if (e[i].capacity > 0 && (dist[v] == dist[x] + 1)) {
            k = dfs(v, min(sum, e[i].capacity)); // 递归寻找增广路径
            if (k == 0) dist[v] = inf; // 剪枝，去掉增广完毕的点
            e[i].capacity -= k; // 更新正向边的容量
            e[i ^ 1].capacity += k; // 更新反向边的容量
            res += k; // 计算流量
            sum -= k; // 更新剩余流量
        }
    }
    return res; // 返回经过当前点的所有流量和
}
int dinic(){
    while (bfs()) {
        ans += dfs(src, inf); 
    }
    return ans;
}
signed main() {
    cin >> n >> m >> src >> sink; 
    for (int i = 1; i <= m; i++) {
        int u, v;
        int  val;
        cin >> u >> v >> val; 
        add(u, v, val); 
    }
    cout <<dinic(); // 输出最大流
    return 0;
}
