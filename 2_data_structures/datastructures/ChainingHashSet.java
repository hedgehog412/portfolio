package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.ICollection;
import edu.caltech.cs2.interfaces.ISet;

public class ChainingHashSet<E> extends ISet<E> {
    public ChainingHashSet() {
        super(new ChainingHashDictionary<>(MoveToFrontDictionary::new));
    }

    public ChainingHashSet(ICollection<E> c) {
        this();
        for (E x : c) {
            this.add(x);
        }
    }
}
