#include <iostream>
#include <vector>

using namespace std;

enum PriorityQueueType { MIN, MAX };

template<typename T> struct PriorityQueue {
    vector<T*> queue;
    int s;
    PriorityQueueType type;
    less<T> L;
    greater<T> G;

    PriorityQueue(PriorityQueueType queueType) {
        s = 0;
        type = queueType;
        L = less<T>();
        G = greater<T>();
        queue.push_back(0);
    }

    void exch(int x, int y) {
        T* t = queue[x];
        queue[x] = queue[y];
        queue[y] = t;
    }
    
    bool compare(int x, int y) {
        if (type == MAX) {
            return L(*queue[x], *queue[y]);
        } else {
            return G(*queue[x], *queue[y]);
        }
    }

    void swim(int k) {
        while(k > 1 && compare(k/2, k)) {
            exch(k, k/2);
            k = k/2;
        };
    }

    void sink(int k) {
        while(2 * k <= s) {
            int j = 2 * k;
            if (j < s && compare(j, j + 1)) j++;
            if (compare(j, k)) break;
            exch(k, j);
            k = j;
        };
    }

    PriorityQueue<T>* add(T* x) {
        queue.push_back(x);
        s++;
        swim(s);
        return this;
    }

    T* pop() {
        T* max = queue[1];
        exch(1, s--);
        sink(1);
        queue.pop_back(); // prevent loitering
        return max;
    }

    T peek() {
        return queue[0];
    }

    int get_size() {
        return s;
    }

    void print() {
        for(int i = 1; i <= s; i++) {
            cout << queue[i] << " ";
        }
        cout << endl;
    }

};

struct Query {

    int id;
    int period;
    int priority;

    Query(int _id, int _period) {
        id = _id;
        period = _period;
        priority = _period;
    }

    void update() {
        priority += period;
    }

    bool operator > (const Query &q) const {
        if (priority != q.priority) {
            return priority > q.priority;
        } else {
            return id > q.id;
        }
    }

    bool operator < (const Query &q) const {
        if (priority != q.priority) {
            return priority < q.priority;
        } else {
            return id < q.id;
        }
    }
};

int main() {
    string keyword;
    int id, period, n;
    cin >> keyword;
    PriorityQueue<Query> *queryQueue = new PriorityQueue<Query>(MIN);

    while (cin >> keyword && keyword != "#") {
        cin >> id >> period;
        Query *query = new Query(id, period);
        queryQueue->add(query);
    }

    cin >> n;
    for (int i = 0; i < n; i++) {
        Query* query = queryQueue->pop();
        cout << query->id << endl;
        query->update();
        queryQueue->add(query);
    }

    cout << endl;

    return 0;
}