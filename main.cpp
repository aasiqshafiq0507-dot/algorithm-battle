/*
 * ALGORITHM BATTLE - Which Data Structure Wins?
 * Application : Product ID Search (key = Product ID)
 * Structures  : Binary Search Tree, AVL Tree, Hash Table (Linear Probing)
 *
 * Compile : gcc algorithm_battle.c -o battle
 * Run     : ./battle
 *
 * "Comparison" = one key compared against a stored record's key
 *                (a node visited in BST/AVL, an occupied slot checked in hashing).
 * "Height"     = number of levels (a single root node has height 1).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N          40     /* dataset size (30-50)                    */
#define TABLE_SIZE 53     /* prime table size for hashing            */

typedef struct {
    int   id;
    char  name[30];
    char  category[15];
    float price;
} Product;

static long cmp_count = 0;   /* comparisons in the current operation */
static long rotations = 0;   /* AVL rotations                        */
static int  trace     = 0;   /* 1 = print the search path (viva mode)*/

/* ===================== BINARY SEARCH TREE ===================== */
typedef struct BST {
    Product d;
    struct BST *l, *r;
} BST;

static BST *bst_new(Product s) {
    BST *n = (BST *)malloc(sizeof(BST));
    n->d = s; n->l = n->r = NULL;
    return n;
}

/* returns 1 if inserted, 0 if duplicate */
int bst_insert(BST **root, Product s) {
    cmp_count = 0;
    if (!*root) { *root = bst_new(s); return 1; }
    BST *cur = *root;
    while (1) {
        cmp_count++;
        if (s.id == cur->d.id) return 0;
        if (s.id < cur->d.id) {
            if (!cur->l) { cur->l = bst_new(s); return 1; }
            cur = cur->l;
        } else {
            if (!cur->r) { cur->r = bst_new(s); return 1; }
            cur = cur->r;
        }
    }
}

BST *bst_search(BST *root, int key) {
    cmp_count = 0;
    BST *cur = root;
    while (cur) {
        cmp_count++;
        if (trace) printf("%d ", cur->d.id);
        if (key == cur->d.id) return cur;
        cur = (key < cur->d.id) ? cur->l : cur->r;
    }
    return NULL;
}

int bst_height(BST *n) {
    if (!n) return 0;
    int a = bst_height(n->l), b = bst_height(n->r);
    return 1 + (a > b ? a : b);
}
void bst_inorder(BST *n)   { if (n) { bst_inorder(n->l);   printf("%d ", n->d.id); bst_inorder(n->r); } }
void bst_preorder(BST *n)  { if (n) { printf("%d ", n->d.id); bst_preorder(n->l);  bst_preorder(n->r); } }
void bst_postorder(BST *n) { if (n) { bst_postorder(n->l); bst_postorder(n->r); printf("%d ", n->d.id); } }
void bst_free(BST *n)      { if (n) { bst_free(n->l); bst_free(n->r); free(n); } }

/* ========================== AVL TREE ========================== */
typedef struct AVL {
    Product d;
    int height;
    struct AVL *l, *r;
} AVL;

static int  ah(AVL *n)            { return n ? n->height : 0; }
static int  amax(int a, int b)    { return a > b ? a : b; }
static void aupdate(AVL *n)       { n->height = 1 + amax(ah(n->l), ah(n->r)); }
static int  abal(AVL *n)          { return n ? ah(n->l) - ah(n->r) : 0; }

static AVL *avl_new(Product s) {
    AVL *n = (AVL *)malloc(sizeof(AVL));
    n->d = s; n->height = 1; n->l = n->r = NULL;
    return n;
}
static AVL *rot_right(AVL *y) {
    AVL *x = y->l;
    y->l = x->r; x->r = y;
    aupdate(y); aupdate(x);
    rotations++;
    return x;
}
static AVL *rot_left(AVL *x) {
    AVL *y = x->r;
    x->r = y->l; y->l = x;
    aupdate(x); aupdate(y);
    rotations++;
    return y;
}

static int avl_dup = 0;   /* set when a duplicate is rejected */

AVL *avl_insert_rec(AVL *n, Product s) {
    if (!n) return avl_new(s);
    cmp_count++;
    if (s.id == n->d.id) { avl_dup = 1; return n; }
    if (s.id < n->d.id) n->l = avl_insert_rec(n->l, s);
    else                    n->r = avl_insert_rec(n->r, s);

    aupdate(n);
    int b = abal(n);
    if (b > 1  && s.id < n->l->d.id) return rot_right(n);            /* LL */
    if (b < -1 && s.id > n->r->d.id) return rot_left(n);             /* RR */
    if (b > 1  && s.id > n->l->d.id) { n->l = rot_left(n->l);  return rot_right(n); } /* LR */
    if (b < -1 && s.id < n->r->d.id) { n->r = rot_right(n->r); return rot_left(n); }  /* RL */
    return n;
}
/* returns 1 if inserted, 0 if duplicate */
int avl_insert(AVL **root, Product s) {
    cmp_count = 0; avl_dup = 0;
    *root = avl_insert_rec(*root, s);
    return !avl_dup;
}

AVL *avl_search(AVL *root, int key) {
    cmp_count = 0;
    AVL *cur = root;
    while (cur) {
        cmp_count++;
        if (trace) printf("%d ", cur->d.id);
        if (key == cur->d.id) return cur;
        cur = (key < cur->d.id) ? cur->l : cur->r;
    }
    return NULL;
}
void avl_inorder(AVL *n)   { if (n) { avl_inorder(n->l);   printf("%d ", n->d.id); avl_inorder(n->r); } }
void avl_preorder(AVL *n)  { if (n) { printf("%d ", n->d.id); avl_preorder(n->l);  avl_preorder(n->r); } }
void avl_postorder(AVL *n) { if (n) { avl_postorder(n->l); avl_postorder(n->r); printf("%d ", n->d.id); } }
void avl_free(AVL *n)      { if (n) { avl_free(n->l); avl_free(n->r); free(n); } }

/* ================= HASH TABLE (LINEAR PROBING) ================= */
typedef struct {
    Product d;
    int used;
} Slot;

typedef struct {
    Slot t[TABLE_SIZE];
    int  count;
    long collisions;          /* occupied slots skipped during insertions   */
    int  inserts_with_collision;
} Hash;

static int hash_fn(int key) { return key % TABLE_SIZE; }

void hash_init(Hash *h) { memset(h, 0, sizeof(Hash)); }

/* returns 1 if inserted, 0 if duplicate, -1 if table full */
int hash_insert(Hash *h, Product s) {
    cmp_count = 0;
    if (h->count == TABLE_SIZE) return -1;
    int idx = hash_fn(s.id), hit = 0;
    while (h->t[idx].used) {
        cmp_count++;
        if (h->t[idx].d.id == s.id) return 0;
        h->collisions++;
        hit = 1;
        idx = (idx + 1) % TABLE_SIZE;           /* linear probing */
    }
    h->t[idx].d = s; h->t[idx].used = 1; h->count++;
    if (hit) h->inserts_with_collision++;
    return 1;
}

int hash_search(Hash *h, int key) {   /* returns slot index or -1 */
    cmp_count = 0;
    int idx = hash_fn(key), probes = 0;
    if (trace) printf("h(%d)=%d : ", key, idx);
    while (h->t[idx].used && probes < TABLE_SIZE) {
        cmp_count++;
        if (trace) printf("[%d]=%d ", idx, h->t[idx].d.id);
        if (h->t[idx].d.id == key) return idx;
        idx = (idx + 1) % TABLE_SIZE;
        probes++;
    }
    return -1;
}

/* ========================= DATASET ============================ */
/* 40 unique product IDs, entered in catalogue (ascending) order.
   Replace with your own dataset if needed; try a shuffled order too. */
static const int IDS[N] = {
    3001,3003,3005,3006,3009,3010,3012,3015,3017,3019,
    3022,3024,3025,3028,3030,3031,3034,3037,3039,3041,
    3044,3046,3047,3050,3052,3053,3056,3059,3062,3066,
    3069,3072,3075,3078,3081,3085,3088,3091,3094,3097
};
static const int MISSING[]  = {3000,3008,3021,3036,3049,3060,3070,3083,3099,3150};
static const int NEWKEYS[]  = {3007,3043,3076,3093,3100};
#define NM (int)(sizeof(MISSING)/sizeof(MISSING[0]))
#define NK (int)(sizeof(NEWKEYS)/sizeof(NEWKEYS[0]))

static const char *CATS[] = {"Grocery","Electronics","Stationery","Clothing","Toys"};

Product make_product(int id) {
    Product s;
    s.id = id;
    sprintf(s.name, "Product_%d", id);
    strcpy(s.category, CATS[id % 5]);
    s.price = 49.0f + (float)(id % 40) * 25.0f;   /* sample price in INR */
    return s;
}

/* ========================= EXPERIMENT ========================= */
void run_experiment(BST **bst, AVL **avl, Hash *h) {
    long tb = 0, ta = 0, th = 0;       /* totals                   */
    int  mb = 0, ma = 0, mh = 0;       /* maximums                 */
    int i;

    printf("\n--- Building all three structures (insertion order = dataset order) ---\n");
    long ib = 0, ia = 0, ih = 0;
    for (i = 0; i < N; i++) {
        Product s = make_product(IDS[i]);
        bst_insert(bst, s);        ib += cmp_count;
        avl_insert(avl, s);        ia += cmp_count;
        hash_insert(h, s);         ih += cmp_count;
    }
    printf("Total comparisons while building (%d records):\n", N);
    printf("  BST = %ld   AVL = %ld   Hash = %ld\n", ib, ia, ih);
    printf("AVL rotations during build = %ld\n", rotations);

    printf("\n--- Traversals ---\n");
    printf("BST inorder  : "); bst_inorder(*bst);   printf("\n");
    printf("BST preorder : "); bst_preorder(*bst);  printf("\n");
    printf("BST postorder: "); bst_postorder(*bst); printf("\n");
    printf("AVL inorder  : "); avl_inorder(*avl);   printf("\n");
    printf("AVL preorder : "); avl_preorder(*avl);  printf("\n");
    printf("AVL postorder: "); avl_postorder(*avl); printf("\n");

    printf("\n--- Structure statistics ---\n");
    printf("BST height : %d\n", bst_height(*bst));
    printf("AVL height : %d\n", ah(*avl));
    printf("Hash table size = %d, records = %d, load factor = %.2f\n",
           TABLE_SIZE, h->count, (double)h->count / TABLE_SIZE);
    printf("Hash collisions (occupied slots skipped) = %ld\n", h->collisions);
    printf("Insertions that had at least one collision = %d\n", h->inserts_with_collision);

    /* ---- successful search: every stored key ---- */
    for (i = 0; i < N; i++) {
        bst_search(*bst, IDS[i]);  tb += cmp_count; if (cmp_count > mb) mb = cmp_count;
        avl_search(*avl, IDS[i]);  ta += cmp_count; if (cmp_count > ma) ma = cmp_count;
        hash_search(h, IDS[i]);    th += cmp_count; if (cmp_count > mh) mh = cmp_count;
    }
    printf("\n--- Successful search (all %d stored keys) ---\n", N);
    printf("%-8s %10s %10s %10s\n", "", "Total", "Average", "Max");
    printf("%-8s %10ld %10.2f %10d\n", "BST",  tb, (double)tb / N, mb);
    printf("%-8s %10ld %10.2f %10d\n", "AVL",  ta, (double)ta / N, ma);
    printf("%-8s %10ld %10.2f %10d\n", "Hash", th, (double)th / N, mh);

    /* ---- unsuccessful search ---- */
    tb = ta = th = 0; mb = ma = mh = 0;
    for (i = 0; i < NM; i++) {
        bst_search(*bst, MISSING[i]);  tb += cmp_count; if (cmp_count > mb) mb = cmp_count;
        avl_search(*avl, MISSING[i]);  ta += cmp_count; if (cmp_count > ma) ma = cmp_count;
        hash_search(h, MISSING[i]);    th += cmp_count; if (cmp_count > mh) mh = cmp_count;
    }
    printf("\n--- Unsuccessful search (%d keys not in the dataset) ---\n", NM);
    printf("%-8s %10s %10s %10s\n", "", "Total", "Average", "Max");
    printf("%-8s %10ld %10.2f %10d\n", "BST",  tb, (double)tb / NM, mb);
    printf("%-8s %10ld %10.2f %10d\n", "AVL",  ta, (double)ta / NM, ma);
    printf("%-8s %10ld %10.2f %10d\n", "Hash", th, (double)th / NM, mh);

    /* ---- insertion of new keys ---- */
    printf("\n--- Insertion of %d new keys (comparisons per key) ---\n", NK);
    printf("%-8s %8s %8s %8s\n", "Key", "BST", "AVL", "Hash");
    long sb = 0, sa = 0, sh = 0, rot_before = rotations;
    for (i = 0; i < NK; i++) {
        Product s = make_product(NEWKEYS[i]);
        bst_insert(bst, s);  int cb = cmp_count;
        avl_insert(avl, s);  int ca = cmp_count;
        hash_insert(h, s);   int ch = cmp_count;
        printf("%-8d %8d %8d %8d\n", NEWKEYS[i], cb, ca, ch);
        sb += cb; sa += ca; sh += ch;
    }
    printf("%-8s %8ld %8ld %8ld\n", "Total", sb, sa, sh);
    printf("AVL rotations during these insertions = %ld\n", rotations - rot_before);
    printf("Height after insertions -> BST: %d, AVL: %d\n", bst_height(*bst), ah(*avl));
    printf("Hash collisions so far = %ld\n", h->collisions);
}

/* ===================== VIVA / INTERACTIVE ===================== */
void show_product(Product *s) {
    printf("Found -> ID: %d, Name: %s, Category: %s, Price: Rs %.2f\n", s->id, s->name, s->category, s->price);
}

void viva_search(BST *bst, AVL *avl, Hash *h, int key) {
    trace = 1;
    printf("\nBST path : ");  BST *b = bst_search(bst, key);
    printf("\n  %s, comparisons = %ld\n", b ? "FOUND" : "NOT FOUND", cmp_count);
    printf("AVL path : ");    AVL *a = avl_search(avl, key);
    printf("\n  %s, comparisons = %ld\n", a ? "FOUND" : "NOT FOUND", cmp_count);
    printf("Hash     : ");    int i = hash_search(h, key);
    printf("\n  %s, comparisons = %ld\n", i >= 0 ? "FOUND" : "NOT FOUND", cmp_count);
    trace = 0;
    if (b) show_product(&b->d);
}

int main(void) {
    BST *bst = NULL;
    AVL *avl = NULL;
    Hash h;
    hash_init(&h);

    printf("=== ALGORITHM BATTLE: Product ID Search ===\n");
    run_experiment(&bst, &avl, &h);

    int choice, key;
    do {
        printf("\n--- Viva Menu ---\n");
        printf("1. Search a key in all three\n");
        printf("2. Insert a new key in all three\n");
        printf("3. Show traversals and heights\n");
        printf("0. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
        case 1:
            printf("Enter product ID: "); scanf("%d", &key);
            viva_search(bst, avl, &h, key);
            break;
        case 2: {
            printf("Enter product ID: "); scanf("%d", &key);
            Product s = make_product(key);
            long rb = rotations;
            int r1 = bst_insert(&bst, s); long c1 = cmp_count;
            int r2 = avl_insert(&avl, s); long c2 = cmp_count;
            int r3 = hash_insert(&h, s);  long c3 = cmp_count;
            if (!r1) printf("Duplicate key - rejected.\n");
            else printf("Inserted. Comparisons -> BST: %ld, AVL: %ld, Hash: %ld\n", c1, c2, c3);
            if (r2) printf("AVL rotations for this insert: %ld\n", rotations - rb);
            if (r3 == -1) printf("Hash table is full!\n");
            break;
        }
        case 3:
            printf("BST inorder : "); bst_inorder(bst); printf("\n");
            printf("AVL inorder : "); avl_inorder(avl); printf("\n");
            printf("BST height = %d, AVL height = %d\n", bst_height(bst), ah(avl));
            break;
        }
    } while (choice != 0);

    bst_free(bst);
    avl_free(avl);
    return 0;
}
