#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// Added heapify function:
void heapify(int pData[], int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
	int temp;
  
    if (l < n && pData[l] > pData[largest])
      largest = l;
  
    if (r < n && pData[r] > pData[largest])
      largest = r;
  
    if (largest != i) {
      	temp = pData[i];
    	pData[i] = pData[largest];
    	pData[largest] = temp;
     	heapify(pData, n, largest);
    }
  }

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int temp;
	for (int i = n / 2 - 1; i >= 0; i--)
      heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--) {
      temp = arr[0];
	  arr[0] = arr[i];
	  arr[i] = temp;
      heapify(arr, i, 0);
    }
}

//Added Merge function to complete Merge Sorting Algorithm
void Merge(int Data[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	int *L = (int*) Alloc(n1*sizeof(int));
	int *R = (int*) Alloc(n2*sizeof(int));

	for (i = 0; i < n1; i++)
		L[i] = Data[l + i];
	for (j = 0; j < n2; j++)
		R[j] = Data[m + 1+ j];
	
	i = 0;
	j = 0;
	k = l;

	while (i < n1 && j < n2){
		if (L[i] <= R[j]){
			Data[k] = L[i];
			i++;
		}
		else{
			Data[k] = R[j];
			j++;
		}
		k++;
	}
	
	while (i < n1){
		Data[k] = L[i];
		i++;
		k++;
	}
	
	while (j < n2){
		Data[k] = R[j];
		j++;
		k++;
	}

	DeAlloc(L);
	DeAlloc(R);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r){
	if (l < r){
		int middle = (l+r)/2;
		mergeSort(pData, l, middle);
		mergeSort(pData, middle+1, r);
		Merge(pData, l, middle, r);
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n){
	int i, item, j;
	for (i = 1; i < n; i++){
		item = pData[i];
		
		for(j = i-1; j >= 0; j--){
			if(pData[j] > item)
			pData[j+1] = pData[j];
			else
			break;
		}
		pData[j+1] = item;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n){
	int i, j,temp;
	for (i = 0; i < n-1; i++) {
		for (j = 0; j < n-i-1; j++){
			if (pData[j] > pData[j+1]) {
				temp=pData[j];
				pData[j]=pData[j+1];
				pData[j+1]=temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n){
	int i, j, min, temp;
	
	for (i = 0; i < n-1; i++){
		min = i;
		for (j = i+1; j < n; j++)
		if (pData[j] < pData[min])
		min = j;

		temp = pData[i];
		pData[i] = pData[min];
		pData[min] = temp;
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int *data, n;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL){
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (int i = 0; i < dataSz; i++)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}
		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
	int firstHundred = (dataSz < 100 ? dataSz : 100);

	printf("\tData:\n\t");
	for (i = 0; i < firstHundred; i++)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	if (dataSz > 100){
		for (i=sz;i<dataSz;++i)
		{
			printf("%d ",pData[i]);
		}
		printf("\n\n");
	}
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
