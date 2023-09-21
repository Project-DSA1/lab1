#include "test.h"
#include<assert.h>
#include <string.h> // for testing generate_splits()

/*
 * Generate k-selections of a[0..n-1] in lexicographic order and call process_selection to process them.
 *
 * The array b[] will have enough space to hold k elements.
 * For a selection i1, ..., ik, you should set b[0] = a[i1], ..., b[k-1] = a[ik].
 * Selections should be generated in lexicographic order.
 * a[0..k-1] is the smallest selection and a[n-k..n-1] is the largest.
 */

void generate_selections_self(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data), int pos, int l)
{
    // Array is always call by reference in c
    if(l==k){
        process_selection(b,k,data);
        return;
    }
    if(pos==n){
        return;
    }
        b[l] = a[pos];
        generate_selections_self(a,n,k,b,data,process_selection, pos+1, l+1);
        generate_selections_self(a,n,k,b,data,process_selection, pos+1, l);
    return;
}

void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data))
{
    assert(k<n);
    generate_selections_self(a,n,k,b,data,process_selection,0,0);
}

/*
 * See Exercise 2 (a), page 94 in Jeff Erickson's textbook.
 * The exercise only asks you to count the possible splits.
 * In this assignment, you have to generate all possible splits into buf[]
 * and call process_split() to process them.
 * The dictionary parameter is an array of words, sorted in dictionary order.
 * nwords is the number of words in this dictionary.
 */


int check_word(const char *source, int start, int j, const char *dictionary[], int nwords){
    char ch[j - start + 2];
    strncpy(ch, source + start, j - start + 1);
    ch[j - start + 1] = '\0';
    for(int i=0;i<nwords;i++){
        if(strcmp(dictionary[i], ch)==0){
            return 1;
        }
    }
    return 0;
}

void generate_splits_self(const char *source, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data), int start, int bufl){
    char w[2500];
    if(source[start]=='\0'){
        process_split(buf, data);
        return;
    }

    for(int i=start; source[i]!='\0';i++){
        buf[bufl] = source[i];
        ++bufl;

        if(check_word(source, start, i, dictionary, nwords)){
            buf[bufl] = ' ';
            ++bufl;
            generate_splits_self(source, dictionary, nwords, buf, data, process_split, i+1, bufl);
            --bufl;
        }

    }
}

void generate_splits(const char *source, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data))
{
    generate_splits_self(source, dictionary, nwords, buf, data, process_split, 0, 0);
}


/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */
void previous_permutation(int a[], int n)
{
    int b[n]; // b stores {4,2,1} if array is 3 1 2 4, its size is atmost n
    b[0] = a[n-1]; 
    int i = n-2;
    int pos = 1;
    int l=1; // used to keep track of actual length of b

    while(i>=0){
        if(a[i]<=a[i+1]){
            b[pos] = a[i];
            pos++;
            l++;
            i--;
        }
        else{
            break;
        }
    }
    // i is the index of 3 in 3 1 2 4
    // if i is -1, then the array is already the smallest premutation
    if(i==-1){
        for(int j=0;j<n;j++){
            printf("%d ",a[j]);
        }
        printf("\n");
        return;
    }
    // c is the new array containing the largest smaller permutation
    // 0 to i-1 elements of c are same as a
    int c[n];
    for(int j=0;j<i;j++){
        c[j] = a[j];
    }
    pos = l-1; // pos keeps track of which index of b is in consideration
    int bool = 0;
    for(int j=n-1;j>i;j--){
        // if the prev element of b (b[pos-1]) is also smaller than a[i], then add b[pos] to c[j]
        // else, we add b[pos] to c[i] and a[i] to c[j]. Then we add all other elements of b to c as shown
        // However, we need to check if there is a pos-1 element
        if(pos==0){
            if(b[pos]<a[i]){
                c[j] = a[i];
                c[i] = b[pos];
            }
        }
        else if(b[pos-1]<a[i]){
            c[j] = b[pos];
            pos--;
        }
        
        else{
            c[i] = b[pos];
            c[j] = a[i];
            pos--;
            j--;
            for(;j>i;j--){
                c[j] = b[pos];
                pos--;
            }
            break;
        }
    }
    for(int j=0;j<n;j++){
        a[j] = c[j];
    }
}

/* Write your tests here. Use the previous assignment for reference. */

typedef struct {
    int index;
    int err;
} state_t;

static void test_selections_2165(int b[], int k, void *data)
{
    // for(int i=0;i<k;i++){
    //     printf("%d ",b[i]);
    // }
    // printf("\n");
    state_t *s = (state_t *)data;
    s->err = 0;
    switch (s->index) {
    case 0:
        if ((b[0] != 2) || (b[1] != 1)) {
            s->err = 1;
        }
        break;
    case 1:
        if ((b[0] != 2) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 2:
        if ((b[0] != 2) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 3:
        if ((b[0] != 1) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 4:
        if ((b[0] != 1) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 5:
        if ((b[0] != 6) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

BEGIN_TEST(generate_selections) {
    int a[] = { 2, 1, 6, 5 };
    int b[2];
    state_t s2165 = { .index = 0, .err = 1 };
    generate_selections(a, 4, 2, b, &s2165, test_selections_2165);
    ASSERT(!s2165.err, "Failed on 2 1 6 5.");
} END_TEST

void test_splits_art(char buf[], void *data)
{
    // printf("%s\n",buf);
    state_t *s = (state_t*)data;
    s->err = 0;
    switch (s->index) {
    case 0:
        if (!strcmp(buf, "art is toil")) {
            s->err = 1;
        }
        break;
    case 1:
        if (!strcmp(buf, "artist oil")) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
}

BEGIN_TEST(generate_splits) {
    const char *a = "artistoil";
    const char *dict[] = {
        "art",
        "artist",
        "is",
        "oil",
        "toil"
    };
    int nwords = 5;
    state_t s = { .index = 0, .err = 1 };
    char buf[256];
    generate_splits(a, dict, nwords, buf, &s, test_splits_art);
    ASSERT(!s.err, "Failed on 'artistoil'.");
} END_TEST

BEGIN_TEST(previous_permutation) {
    int a[] = { 1, 5, 6, 2, 3, 4 };
    previous_permutation(a, 6);
    // for(int i=0;i<6;i++){
    //     printf("%d ",a[i]);
    // }
    // printf("\n");
    ASSERT_ARRAY_VALUES_EQ(a, 6, "Failed on 1 5 6 2 3 4.", 1, 5, 4, 6, 3, 2);
} END_TEST

int main()
{
    run_tests((test_t[]) {
            TEST(generate_selections),
            TEST(generate_splits),
            TEST(previous_permutation),
            0
        });
    return 0;
}