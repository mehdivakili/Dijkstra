//Single Source Shortest Path with Dijkstra Algorithm (CLRS-Page 658)
#include<vector>
#include<iostream>

using namespace std;

#define MAX    100000
#define    INF    1000000000

int dist[MAX], parent[MAX], min_heap[MAX], min_heap_map[MAX], found[MAX];
vector<pair<int, int> > adj[MAX];


int n, m; //n: number of vertices, m: number of edges

int remove_min_heap(int d);

void min_heap_swap(int a, int b);

void up_heapify(int i, int d);

void down_heapify(int i, int d);

void down_heap_child(int base, int i, int d);

void up_heap_child(int base, int i, int d);

void min_heap_swap(int a, int b) {
    min_heap_map[min_heap[b]] = a;
    min_heap_map[min_heap[a]] = b;

    int temp = min_heap[b];
    min_heap[b] = min_heap[a];
    min_heap[a] = temp;
}

int get_first_child(int i, int d) {
    return d * i + 1;
}

int get_parent(int i, int d) {
    return (i - 1) / d;
}


int remove_min_heap(int d) {
    int res = min_heap[0];
    min_heap_swap(0, --n);
    down_heapify(0, d);
    return res;
}

void down_heapify(int i, int d) {
    while (i < n) {
        int temp = min_heap[i];
        int c2 = get_first_child(get_parent(i, d), d);
        down_heap_child(c2, i - c2, d);
        i = min_heap_map[temp];
        int j = get_first_child(i, d);
        if (j < n && dist[min_heap[i]] > dist[min_heap[j]]) {
            min_heap_swap(i, j);
            int c = get_first_child(get_parent(i, d), d);
            down_heap_child(c, i - c, d);
            i = j;
            continue;
        }
        break;
    }

}

void up_heapify(int i, int d) {

    while (i > 0) {
        int temp = min_heap[i];
        int c2 = get_first_child(get_parent(i, d), d);
        up_heap_child(c2, i - c2, d);
        i = min_heap_map[temp];
        int j = get_parent(i, d);
        if (dist[min_heap[i]] < dist[min_heap[j]]) {
            min_heap_swap(i, j);
            int c = get_first_child(j, d);
            down_heap_child(c, i - c, d);
            i = j;
            continue;
        }
        break;
    }
}

void up_heap_child(int base, int i, int d) {
    while (i > 0) {
        int p = get_parent(i, 2);
        if (dist[min_heap[base + i]] < dist[min_heap[base + p]]) {
            min_heap_swap(base + p, base + i);
            down_heapify(base + i, d);
            i = p;
        } else break;
    }
}

void down_heap_child(int base, int i, int d) {
    while (i < d && i + base < n) {
        int smallest = i;
        int l = get_first_child(i, 2);
        int r = l + 1;
        if (l < d && l + base < n && dist[min_heap[l + base]] < dist[min_heap[smallest + base]])
            smallest = l;
        if (r < d && r + base < n && dist[min_heap[r + base]] < dist[min_heap[smallest + base]])
            smallest = r;
        if (smallest != i) {
            min_heap_swap(base + i, base + smallest);
            down_heapify(base + i, d);
            i = smallest;
        } else break;
    }
}

void dijkstra(int s) {

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
        min_heap[i] = i;
        min_heap_map[i] = i;
        found[i] = false;
    }


    dist[s] = 0;
    int size = n;
    up_heapify(s, m / size);
    for (int i = 0; i < size; i++) {
        int v = remove_min_heap(m / size);
        found[v] = true;
        for (auto edge : adj[v]) {
            int to = edge.first;
            int len = edge.second;
            if (dist[v] + len < dist[to]) {
                dist[to] = dist[v] + len;
                if (!found[to]) {
                    up_heapify(min_heap_map[to], m / size);
                }
                parent[to] = v;
            }
        }
    }
}


int main() {
    int source;
    cin >> n >> m >> source;
    int size = n;

    int x, y, w;
    for (int i = 0; i < m; i++) {
        cin >> x >> y >> w; // edge x->y with length w
        adj[x].push_back({y, w});
    }

    dijkstra(source);
    for (int i = 0; i < size; i++) {
        cout << i << "\t" << ((dist[i] == INF) ? -1 : dist[i]) << endl;
    }

    return 0;
}
