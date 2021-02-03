//
// Created by loenberg on 1/21/21.
//

#ifndef THESISTESTSOURCE_MINHEAP_HPP
#define THESISTESTSOURCE_MINHEAP_HPP

#include <cfloat>
#include <iostream>

struct QueueItem{
    int v;
    double distance;
};

struct QueueItemComparer{
    bool operator()(const QueueItem& q1, const QueueItem& q2){
        return q1.distance < q2.distance;
    }
};

// A class for Min Heap
class MinHeap
{
    vector<int> vertexIndeces;
    vector<QueueItem> harr; // pointer to array of elements in heap
    int capacity; // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap
public:
    // Constructor
    explicit MinHeap(int cap):
            harr(cap, {-1, INFINITY}),
            vertexIndeces(cap, -1)
    {
        heap_size = 0;
        capacity = cap;
    }

    // to heapify a subtree with the root at given index
    void MinHeapify(int i)
    {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < heap_size && harr[l].distance < harr[i].distance)
            smallest = l;
        if (r < heap_size && harr[r].distance < harr[smallest].distance)
            smallest = r;
        if (smallest != i)
        {
            swap_index(harr[i].v, harr[parent(i)].v);
            swap_items(&harr[i], &harr[smallest]);
            MinHeapify(smallest);
        }
    }

    static int parent(int i) { return (i-1)/2; }

    // to get index of left child of node at index i
    static int left(int i) { return (2*i + 1); }

    // to get index of right child of node at index i
    static int right(int i) { return (2*i + 2); }

    // to extract the root which is the minimum element
    QueueItem extractMin()
    {
        if (heap_size <= 0)
            return {-1, INFINITY};
        if (heap_size == 1)
        {
            heap_size--;
            return harr[0];
        }

        // Store the minimum value, and remove it from heap
        QueueItem root = harr[0];
        harr[0] = harr[heap_size-1];
        heap_size--;
        MinHeapify(0);

        return root;
    }

    // Decreases key value of key at index i to new_val
    void decreaseKey(int v, double new_val)
    {
        int i = vertexIndeces[v];
        // Alter to fit key/value form
        harr[i].distance = new_val;
        while (i != 0 && harr[parent(i)].distance > harr[i].distance)
        {
            swap_index(harr[i].v, harr[parent(i)].v);
            swap_items(&harr[i], &harr[parent(i)]);
            i = parent(i);
        }
        vertexIndeces[v] = i;
    }

    // Returns the minimum key (key at root) from min heap
    QueueItem getMin() { return harr[0]; }

    // Deletes a key stored at index i
    void deleteKey(int v)
    {
        int i = vertexIndeces[v];
        decreaseKey(i, DBL_MIN);
        extractMin();
    }

    // Inserts a new key 'k'
    void insertKey(int v, double k)
    {
        if (heap_size == capacity)
        {
            cout << "\nOverflow: Could not insertKey\n";
            return;
        }

        // First insert the new key at the end
        heap_size++;
        int i = heap_size - 1;
        harr[i] = {v, k};

        // Fix the min heap property if it is violated
        while (i != 0 && harr[parent(i)].distance > harr[i].distance)
        {
            swap_index(harr[i].v, harr[parent(i)].v);
            swap_items(&harr[i], &harr[parent(i)]);
            i = parent(i);
        }
        vertexIndeces[v] = i;
    }

private:
    static void swap_items(QueueItem *x, QueueItem *y)
    {
        QueueItem temp = *x;
        *x = *y;
        *y = temp;
    }

    void swap_index(int u, int v){
        int u_index = vertexIndeces[u];
        vertexIndeces[u] = vertexIndeces[v];
        vertexIndeces[v] = u_index;
    }
};

#endif //THESISTESTSOURCE_MINHEAP_HPP
