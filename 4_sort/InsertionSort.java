
public class InsertionSort {
	public static void sort(int[] array){
		int idx=0;
		while(idx<array.length){
		for(int i=idx;i>=0;i--){
			if(array[idx]<array[i]){
				swap(array,idx,i);
				idx=i;
			}
		}
		idx++;
		}
	}
	public static void sort2(int[] array){
		int idx=0;
		int temp=1;
		while(idx<array.length){
		for(int i=idx-1;i>=0;i--){
			if(array[idx]<array[i]){
				temp=0;
			}
			else if(temp==0){
					insert(array,idx,i+1);
					temp=1;
				}
		}
		if(temp==0) {
			insert(array,idx,0);
			temp=1;
		}
		idx++;
		}
	}
	public static void swap(int[] array, int idx1, int idx2){
		int temp;
		temp=array[idx1];
		array[idx1]=array[idx2];
		array[idx2]=temp;
	}
	public static void printArray(int[] a)
	{
		for(int i = 0; i < a.length; i++)
		{
			System.out.print("[" + i + "," + a[i] + "] ");
			if((i + 1) % 10 == 0) System.out.println();
		}
	}
	public static void insert(int[] array, int idx1, int idx2){
		int temp=array[idx1];
		for(int i=idx1-1;i>=idx2;i--){
			array[i+1]=array[i];
		}
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
		int[] test={13,80,93,90,46,56,97,88,81,14};
		printArray(test);
		insert(test,3,1 );
		printArray(test);
	}
}
