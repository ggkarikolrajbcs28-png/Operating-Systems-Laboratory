#include <stdlib.h>

struct FreeServer {
    int weight;
    int index;
};

struct BusyServer {
    long long freeTime;
    int weight;
    int index;
};

void pushFree(struct FreeServer* heap, int* size, int weight, int index) {
    int i = (*size)++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].weight < weight || (heap[p].weight == weight && heap[p].index <= index)) {
            break;
        }
        heap[i] = heap[p];
        i = p;
    }
    heap[i].weight = weight;
    heap[i].index = index;
}

struct FreeServer popFree(struct FreeServer* heap, int* size) {
    struct FreeServer root = heap[0];
    (*size)--;
    if (*size > 0) {
        struct FreeServer last = heap[*size];
        int i = 0;
        while (i * 2 + 1 < *size) {
            int left = i * 2 + 1;
            int right = i * 2 + 2;
            int child = left;
            if (right < *size && (heap[right].weight < heap[left].weight || 
               (heap[right].weight == heap[left].weight && heap[right].index < heap[left].index))) {
                child = right;
            }
            if (last.weight < heap[child].weight || 
               (last.weight == heap[child].weight && last.index <= heap[child].index)) {
                break;
            }
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return root;
}

void pushBusy(struct BusyServer* heap, int* size, long long freeTime, int weight, int index) {
    int i = (*size)++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].freeTime < freeTime || 
           (heap[p].freeTime == freeTime && heap[p].weight < weight) ||
           (heap[p].freeTime == freeTime && heap[p].weight == weight && heap[p].index <= index)) {
            break;
        }
        heap[i] = heap[p];
        i = p;
    }
    heap[i].freeTime = freeTime;
    heap[i].weight = weight;
    heap[i].index = index;
}

struct BusyServer popBusy(struct BusyServer* heap, int* size) {
    struct BusyServer root = heap[0];
    (*size)--;
    if (*size > 0) {
        struct BusyServer last = heap[*size];
        int i = 0;
        while (i * 2 + 1 < *size) {
            int left = i * 2 + 1;
            int right = i * 2 + 2;
            int child = left;
            if (right < *size && (heap[right].freeTime < heap[left].freeTime || 
               (heap[right].freeTime == heap[left].freeTime && heap[right].weight < heap[left].weight) ||
               (heap[right].freeTime == heap[left].freeTime && heap[right].weight == heap[left].weight && heap[right].index < heap[left].index))) {
                child = right;
            }
            if (last.freeTime < heap[child].freeTime || 
               (last.freeTime == heap[child].freeTime && last.weight < heap[child].weight) ||
               (last.freeTime == heap[child].freeTime && last.weight == heap[child].weight && last.index <= heap[child].index)) {
                break;
            }
            heap[i] = heap[child];
            i = child;
        }
        heap[i] = last;
    }
    return root;
}

int* assignTasks(int* servers, int serversSize, int* tasks, int tasksSize, int* returnSize) {
    struct FreeServer* freeHeap = (struct FreeServer*)malloc(serversSize * sizeof(struct FreeServer));
    struct BusyServer* busyHeap = (struct BusyServer*)malloc(serversSize * sizeof(struct BusyServer));
    int freeSize = 0;
    int busySize = 0;

    for (int i = 0; i < serversSize; i++) {
        pushFree(freeHeap, &freeSize, servers[i], i);
    }

    int* ans = (int*)malloc(tasksSize * sizeof(int));
    *returnSize = tasksSize;

    long long currTime = 0;

    for (int j = 0; j < tasksSize; j++) {
        if (currTime < j) {
            currTime = j;
        }

        while (busySize > 0 && busyHeap[0].freeTime <= currTime) {
            struct BusyServer dynamicServer = popBusy(busyHeap, &busySize);
            pushFree(freeHeap, &freeSize, dynamicServer.weight, dynamicServer.index);
        }

        if (freeSize == 0) {
            currTime = busyHeap[0].freeTime;
            while (busySize > 0 && busyHeap[0].freeTime <= currTime) {
                struct BusyServer dynamicServer = popBusy(busyHeap, &busySize);
                pushFree(freeHeap, &freeSize, dynamicServer.weight, dynamicServer.index);
            }
        }

        struct FreeServer targetServer = popFree(freeHeap, &freeSize);
        ans[j] = targetServer.index;
        pushBusy(busyHeap, &busySize, currTime + tasks[j], targetServer.weight, targetServer.index);
    }

    free(freeHeap);
    free(busyHeap);

    return ans;
}
