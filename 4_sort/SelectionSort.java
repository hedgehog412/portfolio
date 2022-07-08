
public class SelectionSort {
	public static void sort(int[] array){
		int min=array[0];
		int idx=0;
		int stidx=0;
		
		while(stidx<array.length){
			idx=stidx;
		for(int i=stidx+1;i<array.length;i++)
		{
			if(min>array[i])
				{
					idx=i;
					min=array[i];
				}
		}
		int temp=array[stidx];
		array[stidx]=array[idx];
		array[idx]=temp;
		if(stidx<array.length-1) min=array[stidx+1];
		stidx++;
		}
	}
}