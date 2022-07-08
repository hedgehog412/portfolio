package edu.caltech.cs2.interfaces;

public interface ITrieMap<A, K extends Iterable<A>, V> extends IDictionary<K, V> {
    /**
     * Determines whether the given key is a prefix of a key in the trie.
     * @param key the prefix to search for in the trie
     * @return true if the trie has a key starting with the given key, false otherwise
     */
    public boolean isPrefix(K key);

    /**
     * Gets the collection of values whose keys start with the given prefix.
     * @param prefix the prefix to search for in the trie
     * @return the values corresponding to the keys starting with the given prefix
     */
    public ICollection<V> getCompletions(K prefix);

    /**
     * Removes all elements from the trie.
     */
    public void clear();
}
