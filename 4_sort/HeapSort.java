
public class HeapSort {
	/*
	public static void sort(int[]arr){
		for(int i=1;i<arr.length;i++){
			if(arr[(i-1)/2]>arr[i]) {
				swap(arr, (i-1)/2, i);
				print(arr);
				i=1;
			}
		}
		int idx=1;
		while(idx<arr.length){
			int stidx=pop(arr,idx);
			int par=stidx;
			int left=par*2-(par-1);
			int right=left+1;
			
			System.out.println("------"+par+" "+left+" "+right+"-----");
			print(arr);
			while(right<arr.length){
				
				System.out.println(par+" "+left+" "+right);
				if(arr[left]<arr[right]){
					if(arr[par]>arr[left]){
						swap(arr,par,left);
						par=left;
					}
					else if(arr[par]>arr[right]){
						swap(arr,par,right);
						par=right;
						
					}
					else{
						break;
					}
				}
				else{
					if(arr[par]>arr[right]){
						swap(arr,par,right);
						par=right;
					}
					else if(arr[par]>arr[left]){
						swap(arr,par,left);
						par=left;
					}
					else{
						break;
					}
				}
				left=par*2-(stidx-1);
				right=left+1;
			}
			idx++;
		}
		if(arr[arr.length-1]<arr[arr.length-2]){
			swap(arr,arr.length-1,arr.length-2);
		}
		/*
		int idx2=1;
		int idx;
		while(idx2<arr.length){
			pop(arr,idx2);
			System.out.println("******");
			
			idx2++;
			idx=1;
			
			System.out.println(idx2);
			print(arr);
			while(idx*2+1+idx2-1<arr.length){
				if(arr[idx*2+idx2-1]<arr[idx*2+1+idx2-1]){
					if(arr[idx+idx2-1]>arr[idx*2+idx2-1]){
						swap(arr,idx+idx2-1,idx*2+idx2-1);
						idx=idx*2;
					}
					else if(arr[idx+idx2-1]>arr[idx*2+1+idx2-1]){
						swap(arr,idx+idx2-1,idx*2+1+idx2-1);
						idx=idx*2+1;
					}
					else{
						break;
					}
				}
				else{
					if(arr[idx+idx2-1]>arr[idx*2+1+idx2-1]){
						swap(arr,idx+idx2-1,idx*2+1+idx2-1);
						idx=idx*2+1;
					}
					else if(arr[idx+idx2-1]>arr[idx*2+idx2-1]){
						swap(arr,idx+idx2-1,idx*2+idx2-1);
						idx=idx*2;
					}
					else{
						break;
					}
				}
				print(arr);
			}
			
		}	}
		//*/
	public static void swap(int[] array, int idx1, int idx2){
		int temp;
		temp=array[idx1];
		array[idx1]=array[idx2];
		array[idx2]=temp;
		print(array);
	}
	public static void print(int[]array){
		System.out.print("[");
		for(int a:array){
			System.out.print(a+", ");
		}
		System.out.println("]");
	}
	public static int pop(int[]array, int idx){
		int temp=array[array.length-1];
		for(int i=array.length-1;i>idx;i--){
			int temp2=array[i-1];
			array[i-1]=array[i];
			array[i]=temp2;
		}
		array[idx]=temp;
		return idx;
	}
	 public static void sort(int arr[])
	    {
	        int n = arr.length;
	 
	        // Build heap (rearrange array)
	        for (int i = n / 2 - 1; i >= 0; i--)
	            heapify(arr, n, i);
	 
	        // One by one extract an element from heap
	        for (int i=n-1; i>=0; i--)
	        {
	            // Move current root to end
	            int temp = arr[0];
	            arr[0] = arr[i];
	            arr[i] = temp;
	 
	            // call max heapify on the reduced heap
	            heapify(arr, i, 0);
	        }
	    }
	 
	    // To heapify a subtree rooted with node i which is
	    // an index in arr[]. n is size of heap
	    public static void heapify(int arr[], int n, int i)
	    {
	        int largest = i;  // Initialize largest as root
	        int l = 2*i + 1;  // left = 2*i + 1
	        int r = 2*i + 2;  // right = 2*i + 2
	 
	        // If left child is larger than root
	        if (l < n && arr[l] > arr[largest])
	            largest = l;
	 
	        // If right child is larger than largest so far
	        if (r < n && arr[r] > arr[largest])
	            largest = r;
	 
	        // If largest is not root
	        if (largest != i)
	        {
	            int swap = arr[i];
	            arr[i] = arr[largest];
	            arr[largest] = swap;
	 
	            // Recursively heapify the affected sub-tree
	            heapify(arr, n, largest);
	        }
	    }
}
