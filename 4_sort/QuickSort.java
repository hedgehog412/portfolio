
public class QuickSort {
	public static void sort(int[] arr, int start, int end){
		int pivot=partition(arr, start, end);
		swap(arr,start,pivot);
		print(arr);
		 if(pivot-start>=2){
		sort(arr, start, pivot);
		 }
		if(end-pivot-1>=2){
			sort(arr, pivot+1, end);
		}
	}
	public static int partition(int[] arr, int start, int end){
		int pidx;
		int pivot=arr[start];
		int up=start;
		int down=end;
		
		while(up<down){
			if(pivot<arr[up]){
				while(up<down){
					if(pivot>arr[down]){
						swap(arr,up,down);
						break;
					}
					else{
						down--;
					}
				}
			}
			else{
				up++;
			}
		}
		if(pivot>arr[up]){
		pidx=up;
		}
		else{
			pidx=up-1;
		}
		return pidx;
	}
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
	public static void main(String[] args){
		int[] test={4,1,2,7,5,-1,8,0,6};
		print(test);
		partition(test,0,8);
		print(test);
		sort(test,0,8);
	}
}
