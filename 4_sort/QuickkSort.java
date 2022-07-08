
public class QuickkSort {
	/*public static void quickSort(int[] array,int start, int end){
		int up=start;
		int down=end;
		int key=array[start];
		int keyidx=0;
		int utemp=-1;
		
		while(up<down){
			if(key<=array[up]){
				up++;
			}
			else{
				utemp=up;
			}
			while(utemp>start&&down>up){
				if(key<=array[down]){
					down--;
				}
				else{
					swap(array,utemp, down);
					utemp=-1;
					break;
				}
			}
		}
		if(array[up]<=key) {
			swap(array,keyidx,up);
			keyidx=up;
		}
		else {
			swap(array, keyidx, up-1);
			keyidx=up-1;
		}
		print(array);
		if(keyidx-1-start>1){
		quickSort(array, start, keyidx-1);
		}
		if(end-keyidx-1>1){
			quickSort(array,keyidx+1,end);
		}
	}
	//*/
	public static void swap(int[] array, int idx1, int idx2){
		int temp;
		temp=array[idx1];
		array[idx1]=array[idx2];
		array[idx2]=temp;
	}
	public static void print(int[]array){
		System.out.print("[");
		for(int a:array){
			System.out.print(a+", ");
		}
		System.out.println("]");
	}
	//*/
	public static void quicksort(int[] arr, int start, int end){
		int up=start-1;
		int down=end;
		int pivot=arr[start];
		int keyidx=start;
		int i=start;
		
		while(i<down){
			if(pivot<arr[i]){
				up++;
				swap(arr,up,i);
			}
			i++;
		}
		if(arr[i]<=pivot) {
			swap(arr,keyidx,i);
			keyidx=i;
		}
		else {
			swap(arr, keyidx,i-1);
			keyidx=i-1;
		}
		if(keyidx-1-start>1){
			quicksort(arr, start, keyidx-1);
			}
			if(end-keyidx-1>1){
				quicksort(arr,keyidx+1,end);
			}
	}
	public static int partition(int arr[], int low, int high)
    {
        int pivot = arr[high]; 
        int i = (low-1); // index of smaller element
        for (int j=low; j<high; j++)
        {
            // If current element is smaller than or
            // equal to pivot
            if (arr[j] <= pivot)
            {
                i++;
 
                // swap arr[i] and arr[j]
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
 
        // swap arr[i+1] and arr[high] (or pivot)
        int temp = arr[i+1];
        arr[i+1] = arr[high];
        arr[high] = temp;
 
        return i+1;
    }
 
 
    /* The main function that implements QuickSort()
      arr[] --> Array to be sorted,
      low  --> Starting index,
      high  --> Ending index */
   public static void sort(int arr[], int low, int high)
    {
        if (low < high)
        {
            /* pi is partitioning index, arr[pi] is 
              now at right place */
            int pi = partition(arr, low, high);
 
            // Recursively sort elements before
            // partition and after partition
            sort(arr, low, pi-1);
            sort(arr, pi+1, high);
        }
    }
}