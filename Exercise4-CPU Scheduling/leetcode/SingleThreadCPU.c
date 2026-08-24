#include <stdlib.h>

struct Task {
    int enqueueTime;
    int processingTime;
    int index;
};

struct HeapNode {
    int processingTime;
    int index;
};

int compareTasks(const void* a, const void* b) {
    struct Task* t1 = (struct Task*)a;
    struct Task* t2 = (struct Task*)b;
    if (t1->enqueueTime != t2->enqueueTime) {
        return (t1->enqueueTime > t2->enqueueTime) - (t1->enqueueTime < t2->enqueueTime);
    }
    return (t1->index > t2->index) - (t1->index < t2->index);
}

void heapPush(struct HeapNode* heap, int* heapSize, int procTime, int idx) {
    int i = (*heapSize)++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].processingTime < procTime || 
           (heap[p].processingTime == procTime && heap[p].index <= idx)) {
            break;
        }
        heap[i] = heap[p];
        i = p;
    }
    heap[i].processingTime = procTime;
    heap[i].index = idx;
}

struct HeapNode heapPop(struct HeapNode* heap, int* heapSize) {
    struct HeapNode root = heap[0];
    (*heapSize)--;
    if (*heapSize > 0) {
        struct HeapNode last = heap[*heapSize];
        int i = 0;
        while (i * 2 + 1 < *heapSize) {
            int left = i * 2 + 1;
            int right = i * 2 + 2;
            int child = left;
            if (right < *heapSize && 
               (heap[right].processingTime < heap[left].processingTime || 
               (heap[right].processingTime == heap[left].processingTime && heap[right].index < heap[left].index))) {
                child = right;
            }
            if (last.processingTime < heap[child].processingTime || 
               (last.processingTime == heap[child].processingTime && last.index <= heap[child].index)) {
                break;
            }
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return root;
}

int* getOrder(int** tasks, int tasksSize, int* tasksColSize, int* returnSize) {
    struct Task* extTasks = (struct Task*)malloc(tasksSize * sizeof(struct Task));
    for (int i = 0; i < tasksSize; i++) {
        extTasks[i].enqueueTime = tasks[i][0];
        extTasks[i].processingTime = tasks[i][1];
        extTasks[i].index = i;
    }

    qsort(extTasks, tasksSize, sizeof(struct Task), compareTasks);

    struct HeapNode* minHeap = (struct HeapNode*)malloc(tasksSize * sizeof(struct HeapNode));
    int heapSize = 0;

    int* result = (int*)malloc(tasksSize * sizeof(int));
    *returnSize = tasksSize;

    long long currTime = 0;
    int taskIdx = 0;
    int resIdx = 0;

    while (taskIdx < tasksSize || heapSize > 0) {
        if (heapSize == 0 && currTime < extTasks[taskIdx].enqueueTime) {
            currTime = extTasks[taskIdx].enqueueTime;
        }

        while (taskIdx < tasksSize && extTasks[taskIdx].enqueueTime <= currTime) {
            heapPush(minHeap, &heapSize, extTasks[taskIdx].processingTime, extTasks[taskIdx].index);
            taskIdx++;
        }

        struct HeapNode bestTask = heapPop(minHeap, &heapSize);
        currTime += bestTask.processingTime;
        result[resIdx++] = bestTask.index;
    }

    free(extTasks);
    free(minHeap);

    return result;
}
