/*
 * Filename:             SORT.c
 * Author:               Frank te Nijenhuis
 * Student Number:       S2462575
 * Challenge:            Challenge 4 SORT
 * Course:               Advanced Algorithms and Datastructures (AADS)
 * Description:
 * An implementation of the countingSort algorithm which generates its own list according to some parameters and then
 * sorts it.
 *
 * Time Complexity:      O(n)
 * The countingSort algorithm has a linear time complexity. The generateArray function also runs through a for-loop of
 * length n. This leads to a time complexity of O(2*n) -> O(n)
 * Space Complexity:      O(n+c)
 * To perform the counting sort, two arrays of size n have to be created, for the input and the output. Furthermore, the
 * histogram has a size of c, which is an inputted value. We have O(2*n+c) -> O(n+c) since n can be bigger than c and
 * the other way around. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * The function to generate the array to be sorted, as defined in the assignment.
 */
int* generateArray(int n,int a, int b, int c){
  int* arr = malloc(n*sizeof(int));
  arr[0] = b;
  for(int i = 1; i < n; i++){
    arr[i] = (a*arr[i-1] + b)%c;
  }
  return arr;
}
/*
 * An implementation of countingSort in Java. aArr is the inputted array, bArr is the output, and cArr is the histogram.
 */
int* countingSort(int* aArr,int* bArr,int n,int k){
  //Create cArr using calloc to ensure initialization with zeroes.
  int* cArr = calloc(k,sizeof(int));
  //Increment the value at the location of aArr[j] in cArr
  for(int j = 0; j < n; j++){
    cArr[aArr[j]]++;
  }
  //Translate the values in the histogram to indices in the output array by adding previous values.
  for(int i = 1; i < k; i++){
    cArr[i] += cArr[i-1];
  }
  //Finally, assign the value in aArr[l] to bArr at the location of cArr[aArr[l]] and decrement this last value to
  //ensure correct indexation.
  for(int l = (n-1);l >= 0; l--){
    bArr[cArr[aArr[l]]] = aArr[l];
    cArr[aArr[l]]--;
  }
  free(cArr);
  return bArr;
}

int main(int argc, char const *argv[]) {
  int n, a, b, c;
  scanf("%d ",&n);
  scanf("%d ",&a);
  scanf("%d ",&b);
  scanf("%d ",&c);
  //Create the array which is to be sorted.
  int* arr = generateArray(n,a,b,c);
  //Create a secondary array which will contain the sorted list.
  int* secArr = malloc((n+1)*sizeof(int));
  //The out array contains a pointer to the same array pointed to by secArr.
  int* out = countingSort(arr,secArr,n,c);
  for(int i = 1; i < n; i += 100){
    printf("%d ",out[i]);
  }
  printf("\n");
  free(secArr);
  free(arr);
  return 0;
}
