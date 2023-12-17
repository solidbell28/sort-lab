#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EL 1000    // All elements of the generated array lie in the segment [-MAX_EL / 2; MAX_EL / 2]

static void generate_random_array(int *a, int *b, int *c, int n);
static void swap(int *a, int i, int j, int *swap_counter, int *cmp_counter);
static void bubble_sort(int *a, int n);
static void shell_sort(int *a, int n);
static int check_array(int *src, int *res, int n);
static void print_array(int *arr, int n);
static void test_sort(int *a, int *b, int *c, int n);

int main(void) {
    srand(time(NULL));  // Pseudo-random seed (current time) used so that rand()
    // generate different numbers every run
    int n;
    printf("Enter array size: ");
    if (scanf("%d", &n) != 1) {            // Check of the input correctness
        printf("Error: incorrect input");
        exit(1);
    }
    printf("\n");
    int *a = malloc(n * sizeof(int));  // a, b, c are copies of one array
    int *b = malloc(n * sizeof(int));  // they are needed to compare 2 sorting methods
    int *c = malloc(n * sizeof(int));  // and check the correctness of sorting then

    int cur_el = 500;
    for (int i = 0; i < n; ++i) {  // 1 array (already sorted)
        if (cur_el) {
            --cur_el;
        }
        a[i] = cur_el;
        b[i] = cur_el;
        c[i] = cur_el;
    }
    printf("Array 1\n");
    test_sort(a, b, c, n);

    cur_el = 0;
    for (int i = 0; i < n; ++i) {  // 2 array (inverted)
        a[i] = cur_el;
        b[i] = cur_el;
        c[i] = cur_el;
        if (cur_el + 1 < 500) {
            ++cur_el;
        }
    }
    printf("Array 2\n");
    test_sort(a, b, c, n);

    for (int i = 3; i <= 4; ++i) {  // 3, 4 arrays (randomly generated)
        printf("Array %d\n", i);
        generate_random_array(a, b, c, n);
        test_sort(a, b, c, n);
    }

    free(a);
    free(b);
    free(c);
    return 0;
}

static void generate_random_array(int *a, int *b, int *c, int n) {  // Pseudo-random array generated and copied into
    // a, b, c arrays
    for (int i = 0; i < n; ++i) {
        a[i] = (rand() % MAX_EL) - MAX_EL / 2;  // This formula guarantees that all elements lie in the segment
        // [-MAX_EL / 2; MAX_EL / 2]
        b[i] = a[i];
        c[i] = a[i];
    }
}

static void swap(int *a, int i, int j, int *swap_counter, int *cmp_counter) {  // Two elements of the array are swapped
    // if needed and counters are updated
    ++*cmp_counter;
    if (abs(a[i]) < abs(a[j]) && i != j) {
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
        ++*swap_counter;
    }
}

static void bubble_sort(int *a, int n) {  // Bubble sort implementation
    int swap_counter = 0, cmp_counter = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            swap(a, i, j, &swap_counter, &cmp_counter);
        }
    }
    printf("Bubble Sort\n");
    printf("Number of comparisons conducted: %d\n", cmp_counter);
    printf("Number of swaps conducted: %d\n", swap_counter);
}

static void shell_sort(int *a, int n) {  // Shell sort implementation
    int swap_counter = 0, cmp_counter = 0;
    for (int step = n / 2; step > 0; step /= 2) {
        for (int i = step; i < n; ++i) {
            for (int j = i - step; j >= 0; j -= step) {
                swap(a, j, j + step, &swap_counter, &cmp_counter);
            }
        }
    }
    printf("Shell Sort\n");
    printf("Number of comparisons conducted: %d\n", cmp_counter);
    printf("Number of swaps conducted: %d\n", swap_counter);
}

static int check_array(int *src, int *res, int n) {  // Checks if the array res is sorted src
    int *check = malloc(MAX_EL * sizeof(int));  // check stores info about the quantity of every number in src
    for (int i = 0; i < MAX_EL; ++i) {
        check[i] = 0;
    }
    for (int i = 0; i < n; ++i) {
        ++check[src[i] + MAX_EL / 2];  // check[i] stores the quantity of number (i - MAX_EL / 2) in src
    }
    for (int i = 0; i < n; ++i) {  // Check is failed if res has elements src does not have
        // or if adjacent elements are sorted incorrectly
        if (check[res[i] + MAX_EL / 2] == 0 || (i < n - 1 && abs(res[i]) < abs(res[i + 1]))) {
            free(check);
            return 0;
        }
        --check[res[i] + MAX_EL / 2];
    }
    for (int i = 0; i < MAX_EL; ++i) {  // Check is failed if src has elements res does not have
        if (check[i] != 0) {
            free(check);
            return 0;
        }
    }
    free(check);
    return 1;
}

static void print_array(int *arr, int n) { // Prints all elements of the array in one string
    for (int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

static void test_sort(int *a, int *b, int *c, int n) {
    bubble_sort(a, n);                                             // Bubble sort
    if (check_array(c, a, n)) {
        printf("Bubble sort worked correctly\n");
    } else {
        printf("Error: bubble sort worked incorrectly\n");
        print_array(a, n);
    }
    shell_sort(b, n);                                           // Shell sort
    if (check_array(c, a, n)) {
        printf("Shell sort worked correctly\n");
    } else {
        printf("Error: shell sort worked incorrectly\n");
        print_array(b, n);
    }
    printf("\n");
}
