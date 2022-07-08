package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.IPriorityQueue;

public class TopKSort {
    /**
     * Sorts the largest K elements in the array in descending order. Modifies the array in place.
     * @param array - the array to be sorted; will be manipulated.
     * @param K - the number of values to sort
     * @param <E> - the type of values in the array
     * @throws IllegalArgumentException if K < 0
     */
    public static <E> void sort(IPriorityQueue.PQElement<E>[] array, int K) {
        if (K < 0) {
            throw new IllegalArgumentException("K cannot be negative!");
        }
        if(K == 0){
            for(int i = 0; i< array.length;i++){
                array[i] = null;
            }
        }
        else {
            MinFourHeap<E> sorter = new MinFourHeap<>();
            for (int i = 0; i < array.length; i++) {
                if (sorter.size() < K) {
                    sorter.enqueue(array[i]);
                }
                else if (sorter.size() == K) {
                    if(sorter.peek().priority <= array[i].priority){
                        sorter.dequeue();
                        sorter.enqueue(array[i]);
                    }
                }
            }
            if(K < array.length){
                for(int i = array.length -1; i>=K;i--){
                    array[i] = null;
                }
                for(int i = K-1; i>= 0;i--){
                    array[i] = sorter.dequeue();
                }
            }
            else{
                for(int i = array.length-1;i>=0; i--){
                    array[i] = sorter.dequeue();
                }
            }
        }
    }
}
