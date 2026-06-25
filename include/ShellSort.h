#ifndef SHELLSORT_H
#define SHELLSORT_H


template <typename T>
void shellSort(T* arr, int size, int formula) {
    // formula 0 = Shell (gap = gap/2), formula 1 = Knuth (gap = 3*gap + 1)
    if (formula == 0) {
        // start with half the array size, halve gap each iteration
        for (int odstep = size / 2; odstep > 0; odstep /= 2) {
            for (int i = odstep; i < size; i++) {
                T temp = arr[i];
                int j = i;
                // shift elements by gap until correct position found
                while (j >= odstep && arr[j - odstep] > temp) {
                    arr[j] = arr[j - odstep];
                    j -= odstep;
                }
                arr[j] = temp;
            }
        }

    } else {
        // calculate initial gap using Knuth sequence
        int odstep = 1;
        while (odstep < size / 3) {
            odstep = 3 * odstep + 1;
        }
        while (odstep > 0) {
            for (int i = odstep; i < size; i++) {
                T temp = arr[i];
                int j = i;
                // shift elements by gap until correct position found
                while (j >= odstep && arr[j - odstep] > temp) {
                    arr[j] = arr[j - odstep];
                    j -= odstep;
                }
                arr[j] = temp;
            }
            // shrink gap using reverse Knuth formula
            odstep = (odstep - 1) / 3;
        }

    }
}

#endif
