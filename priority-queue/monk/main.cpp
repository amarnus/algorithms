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

struct Course {

    int id;
    int sum;
    int count;
    int lastIQ;

    Course(int _id) {
        id = _id;
        sum = 0;
        count = 0;
        lastIQ = 0;
    }

    void add(int newIQ) {
        sum = lastIQ + newIQ;
        count++;
        lastIQ = newIQ;
    }

    int priority() const {
        int _priority;
        if (count == 0) {
            _priority = 0;
        } else {
            _priority = sum * count;
        }
        return _priority;
    }

    bool operator > (const Course &c) const {
        if (priority() != c.priority()) {
            return priority() > c.priority();
        } else {
            return id > c.id;
        }
    }

    bool operator < (const Course &c) const {
        if (priority() != c.priority()) {
            return priority() < c.priority();
        } else {
            return id < c.id;
        }
    }
};

int main() {
    int c, p, n;
    cin >> c >> p >> n;
    PriorityQueue<Course> *courseQueue = new PriorityQueue<Course>(MIN);

    for (int i = 0; i < n; i++) {
        int iq = 0;
        cin >> iq;
        Course* course = new Course(i + 1);
        course->add(iq);
        courseQueue->add(course);
    }

    for (int i = n; i < c; i++) {
        courseQueue->add(new Course(i + 1));       
    }

    for (int i = 0; i < p; i++) {
        Course* course = courseQueue->pop();
        cout << course->id << " ";
        int iq = 0;
        cin >> iq;
        course->add(iq);
        courseQueue->add(course);
    }

    cout << endl;

    return 0;
}