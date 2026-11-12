# Algorithm Battle: Which Data Structure Wins?

**Application:** Product ID Search
**Data structures compared:** Binary Search Tree (BST), AVL Tree, Hash Table with Linear Probing
**Language:** C
**Course assessment:** CLA-2, SRM Institute of Science and Technology, Tiruchirappalli

## Part A: Problem Selection

**What is the problem?**
A retail store or warehouse keeps thousands of products, each identified by a unique product ID. The system must store new products and find a product's details (name, category, price) from its ID.

**Why is efficient searching required?**
Product lookup happens at every billing scan, stock check and price query. If search is slow, every transaction is delayed, and the delay grows as the catalogue grows.

**Why are BST, AVL and Hashing suitable for comparison?**
- **BST:** the simple baseline. Search is fast when the tree is balanced, but it degrades to a linear chain when IDs arrive in sorted order.
- **AVL Tree:** a self-balancing BST. It keeps the height logarithmic, so the worst case is controlled, and it still supports sorted (inorder) traversal.
- **Hashing:** gives very fast average lookup by computing the position directly from the key. It does not keep records in sorted order, and its performance depends on collisions and load factor.

Together they show the trade-off between simplicity, guaranteed performance and raw speed.

## Part B: Implementation

| Structure | Insertion | Search | Traversal | Other |
|---|---|---|---|---|
| BST | iterative | iterative | inorder, preorder, postorder | height |
| AVL | recursive with LL, RR, LR, RL rotations | iterative | inorder, preorder, postorder | height, rotation count |
| Hash table | `id % 53`, linear probing | probe until match or empty slot | none | collision count, load factor |

**Record:** `Product { id, name, category, price }`
**Dataset:** 40 unique product IDs.

**Counting rules (used consistently for all three):**
- A *comparison* is one search key compared against one stored key (a node visited in a tree, or an occupied slot checked in the hash table).
- *Height* is the number of levels (a single root node has height 1).
- A *collision* is counted each time an insertion skips an occupied slot.

## How to Run

```
gcc product_search_battle.c -o battle
./battle
```

The program builds all three structures from the same dataset, prints the experiment results, then opens a menu for the viva:

1. Search a key in all three (shows the path or probe sequence)
2. Insert a new key in all three
3. Show traversals and heights

## Part C: Mini Experiment

Same dataset for all three structures:

- Successful search: all 40 stored keys
- Unsuccessful search: 10 keys not in the dataset
- Insertion: 5 new keys

Results are printed by the program. Record them below from your own run.

## Part D: Algorithm Battle (fill from your own output)

| Parameter | BST | AVL | Hashing |
|---|---|---|---|
| Search comparisons, successful (avg / max) | | | |
| Search comparisons, unsuccessful (avg / max) | | | |
| Insertion observations | | | |
| Height | | | not applicable |
| Collisions | not applicable | not applicable | |
| Overall observation | | | |

## Part E: Final Verdict (write from your own observations)

- **Winner for Product ID Search:**
- **Supporting evidence from the experiment:**
- **Trade-offs to mention:** sorted order, worst case, collisions, load factor.

## Files

- `product_search_battle.c`: complete source code
- `README.md`: this description
