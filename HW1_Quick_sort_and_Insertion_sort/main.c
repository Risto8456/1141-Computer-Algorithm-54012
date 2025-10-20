#include <stdio.h>
#include <stdbool.h>

void swap(float *a, float *b){
   float temp = *a;
   *a = *b;
   *b = temp;
}

// 快速排序法
void QuickSort(float *arr, int l, int r){
   if(l >= r) return;
   
   int i = l, j = r;
   float p = arr[r];
   while(i < j){
      while(i < r && arr[i] <= p) i++;
      while(j > l && arr[j] >= p) j--;
      
      if(i < j) swap(arr+i, arr+j);
   }
   
   if(arr[i] > p) swap(arr+i,arr+r);
   
   QuickSort(arr, l, i-1);
   QuickSort(arr, i+1, r);
}

// 插入排序法
void InsertionSort(float *arr, int n){
   int i, j;
   float insertVal;
   for(i = 1; i < n; i++){
      insertVal = arr[i];
      j = i-1;
      while(j >= 0 && insertVal < arr[j]){
         arr[j+1] = arr[j];
         j--;
      }
      arr[j+1] = insertVal;
   }
}

int main(){
   FILE *DataIn;
   DataIn = fopen("test1.txt", "r");
   
   int n, i;
   fscanf(DataIn, "%d", &n);
   
   float arr[n];
   for(i = 0; i < n; i++) fscanf(DataIn, "%f", &arr[i]);
   
   int method = 3;
   while (true) {
      printf("===========================\n");
      printf("1) Quick Sort\n");
      printf("2) Insertion Sort\n");
      printf("3) End of program\n");
      printf("Choose a sorting method : ");
      scanf("%d",&method);
      
      printf("===========================\n");
      switch (method) {
            case 1 : printf("method 1) Quick Sort : \n");
                     QuickSort(arr, 0, n-1);
                     break;
            case 2 : printf("method 2) Insertion Sort :\n");
                     InsertionSort(arr, n);
                     break;
            default: printf("method 3) End of program\n");
                     return 0;
      }
      
      printf("1) The number of all numbers : %d\n", n);
      printf("2) The largest number : %6.2f\n", arr[n-1]);
      printf("3) The smallest number : %6.2f\n", arr[0]);
      printf("Sorted array :");
      for (i = 0; i < n; i++) {
         if (i % 10 == 0) printf("\n");
         printf("%6.2f ",arr[i]);
      }
      printf("\n");
   }
   return 0;
}