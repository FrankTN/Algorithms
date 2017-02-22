/*
 * Filename:             SKY.c
 * Author:               Frank te Nijenhuis
 * Student Number:       S2462575
 * Challenge:            Challenge 2 SKY
 * Course:               Advanced Algorithms and Datastructures (AADS)
 * Description:
 * A program to record the biggest differences in height during a flight.
 *
 * Time Complexity:      O(n*m)
 * Let m denote the size of the heap and n the amount of entries.
 * Since the insert and removeMin operations in the heap structure run in
 * O(log(m)) time we have logarithmic runtime for each entry. There are n
 * different entries, each of which can be a new entry for the heap or a print
 * directive. If we receive a type 2 query, we print the values of the heap in
 * chronological order. We call countingSort, running in O(m) time.
 * Our conclusion is that the worst-time behavior will be O(n*m), n queries
 * which each require a countingSort.
 *
 * Space Complexity:      O(n+m)
 * We create a heap with a maximal size of m. The countingSort function creates
 * a histogram of at most n entries, since n can also be seen as the maximal
 * time. We obtain a worst case space complexity of O(n+m)
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//Each entry in our heap contains a heightdifference value and a timestamp.
typedef struct entry{
  int timestamp;
  int height;
}entry;

entry* countingSort(entry* arr, int n, int k);

void swapEntry(entry *a,entry *b){
  entry temp = *a;
  *a = *b;
  *b = temp;
}

/*
 * These functions are used for creating and maintaining a minheap of entries.
 * Insertion and removal of the minimum both have O(log(n)) complexities.
 */

typedef struct minHeap {
  entry* arr;
  int front;
  int size;
} minHeap;

void heapEmptyError() {
  printf("Heap is empty\n");
  abort();
}

void printHeap(minHeap hp,int maxTime){
  entry* out = countingSort(hp.arr,hp.front, maxTime);
  for(int i = 1; i < hp.front; i++){
    printf("%d %d\n",out[i].timestamp,out[i].height);
  }
  printf("\n");
  free(out);
}

int heapEmpty(minHeap h){
  return (h.front == 1);
}

minHeap createMinHeap(){
    minHeap h;
    h.arr = malloc(sizeof(entry));
    h.size = 1;
    h.front = 1;
    return h;
}

void doubleSize(minHeap* mhptr){
  int nextSize = 2*mhptr->size;
  mhptr->arr = realloc(mhptr->arr,nextSize*sizeof(entry));
  mhptr->size = nextSize;
}

void upheap(minHeap* mhptr,int front){
  if(front <= 1){
    return;
  }
  if(mhptr->arr[front].height < mhptr->arr[front/2].height){
    //We swap the entries so the smallest is always the parent
    swapEntry(&(mhptr->arr[front]),&(mhptr->arr[front/2]));
    upheap(mhptr,front/2);
  }
}

void addEntry(entry e, minHeap* mhptr){
  if(mhptr->front == mhptr->size){
    doubleSize(mhptr);
  }
  mhptr->arr[mhptr->front] = e;
  upheap(mhptr,mhptr->front);
  mhptr->front++;
}

// This version of downheap prioritizes height. If there is a tie, the new entry
// is discarded.
void downheap(minHeap* mhptr,int heapIndex){
  int front = mhptr->front;
  int indexMin = heapIndex;
  if(front < 2*heapIndex+1){
    return;
  }
  if(mhptr->arr[heapIndex].height > mhptr->arr[2*heapIndex].height ||
     (mhptr->arr[heapIndex].height == mhptr->arr[2*heapIndex].height &&
      mhptr->arr[heapIndex].timestamp > mhptr->arr[2*heapIndex].timestamp)){
    indexMin = 2*heapIndex;
  }
  if(mhptr->arr[indexMin].height > mhptr->arr[2*heapIndex+1].height ||
     (mhptr->arr[indexMin].height == mhptr->arr[2*heapIndex+1].height &&
      mhptr->arr[indexMin].timestamp > mhptr->arr[2*heapIndex+1].timestamp)){
    indexMin = 2*heapIndex+1;
  }
  if(indexMin != heapIndex){
    swapEntry(&(mhptr->arr[heapIndex]),&(mhptr->arr[indexMin]));
    downheap(mhptr,indexMin);
  }
}

entry removeMin(minHeap* mhptr){
  entry e;
  if(heapEmpty(*mhptr)){
    printf("Error, no minimum in empty heap\n");
    abort();
  }
  // We remove the current minimum and swap it with the newest element
  // This element is then moved down using downheap
  e = mhptr->arr[1];
  mhptr->front--;
  mhptr->arr[1] = mhptr->arr[mhptr->front];
  downheap(mhptr, 1);
  return e;
}

int heapSize(minHeap *mhptr){
  return mhptr->size;
}

void freeHeap(minHeap* mhptr){
  free(mhptr->arr);
}
 /*
  * Implementation of the countingSort algorithm, sorting on timestamps.
  * This is used to sort the heap array on timestamps(chronologically)
  * for printing.
  */

entry* countingSort(entry* aArr,int n,int k){
  int* cArr = calloc(k,sizeof(int));
  entry* bArr = malloc(n*sizeof(entry));
  for(int j = 1; j < n; j++){
    cArr[aArr[j].timestamp]++;
  }
  for(int i = 2; i < k; i++){
    cArr[i] += cArr[i-1];
  }

  for(int l = (n-1);l > 0; l--){
    bArr[cArr[aArr[l].timestamp]--] = aArr[l];
    cArr[aArr[l].timestamp]--;
  }
  free(cArr);
  return bArr;
}


int main(int argc, char const *argv[]){
  int m,n,minimum = 0;
  scanf("%d %d\n",&n,&m);
  int timeCount = 1;
  minHeap result = createMinHeap();
  //Loop over n input statements, recording the type of each.
  for(int i = 1; i <= n; i++){
    int type;
    scanf("%d",&type);

    if(type == 1){
      int a,b,diff;
      scanf("%d %d\n",&a, &b);

      diff = b - a;
      entry e;
      e.timestamp = timeCount;
      e.height = diff;
      //If the heap is not full yet, add the measurement regardless of its
      //value, otherwise add the measurement only if its better than the first
      //(and therefore smallest) element in the minheap.
      if(result.front < m+1){
        addEntry(e,&result);
        minimum = result.arr[1].height;
      }else if(diff >= minimum){
        addEntry(e,&result);
        removeMin(&result);
        minimum = result.arr[1].height;
      }
      timeCount++;
    }else{
      printHeap(result,timeCount);
    }
  }
  freeHeap(&result);
  return 0;
}
