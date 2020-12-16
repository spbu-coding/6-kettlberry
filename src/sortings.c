#include "sortings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIFFERENT_VALUES 256

void swap(char **str1, char **str2) {
    char *tmp = *str1;
    *str1 = *str2;
    *str2 = tmp;
}

void bubble(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    for (unsigned int i = 0; i < size; i++)
        for (unsigned int j = 0; j < size - 1; j++) {
            if (cmp(array[j], array[j + 1]) > 0)
                swap(&array[j], &array[j + 1]);
        }
}

void insertion(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    for (unsigned int i = 1; i < size; i++)
        for (unsigned int j = i; j > 0; j--)
            if (cmp(array[j - 1], array[j]) > 0)
                swap(&array[j - 1], &array[j]);
}

void merge(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    size_t step = 1;
    char** tmp = (char**)malloc(size * (MAX_INPUT_STRING_SIZE + 2));
    while (step < size)  {
        size_t index = 0;
        size_t left = 0;
        size_t mid = left + step;
        size_t right = left + step * 2;
        do	{
            mid = mid < size ? mid : size;
            right = right < size ? right : size;
            size_t i1 = left, i2 = mid;
            for (; i1 < mid && i2 < right; ) {
                if (cmp(array[i1], array[i2]) < 0)
                    tmp[index++] = array[i1++];
                else  tmp[index++] = array[i2++];
            }
            while (i1 < mid)
                tmp[index++] = array[i1++];
            while (i2 < right)
                tmp[index++] = array[i2++];
            left += step * 2;
            mid += step * 2;
            right += step * 2;
        } while (left < size);
        for (size_t i = 0; i < size; i++)
            array[i] = tmp[i];
        step *= 2;
    }
}

void quick(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    if (size <= 1)
        return;
    unsigned int left = 0, right = size -1;
    char *mid;
    mid = array[size / 2];
    do {
        while (cmp(array[left], mid) < 0) left++;
        while (cmp(array[right], mid) > 0) right--;
        if (left <= right) {
            swap(&array[left], &array[right]);
            left++;
            right--;
        }
    } while (left <= right);
    if (right > 0)
        quick(array, right + 1, cmp);
    if (left < size)
        quick(&array[left], size - left, cmp);
}

void radix(strings_array_t array, array_size_t size, comparator_func_t cmp) {
    if (size == 0)
        return;
    size_t array_lengths[size], max_array_length = 0;
    for (unsigned int i = 0; i < size; i++) {
        array_lengths[i] = strlen(array[i]) - 1;
        if (array_lengths[i] > max_array_length)
            max_array_length = array_lengths[i];
    }
    for (int i = (int) max_array_length - 1; i >= 0; i--) {
        unsigned int count[DIFFERENT_VALUES] = {0};
        for (unsigned int j = 0; j < (unsigned int) size; j++) {
            if ((int)array_lengths[j] - 1 >= i)
                count[(unsigned int)array[j][i]]++;
            else
                count[0]++;
        }
        if (cmp("a", "b") < 0) {
            for (unsigned int j = 1; j < DIFFERENT_VALUES; j++)
                count[j] += count[j - 1];
        }
        else {
            for (int j = DIFFERENT_VALUES - 2; j >= 0; j--)
                count[j] += count[j + 1];
        }
        char *buffer[size];
        size_t buffer_length[size];
        for (int j = (int) size - 1; j >= 0; j--) {
            if ((int) array_lengths[j] - 1 >= i) {
                buffer[(count[(unsigned int) array[j][i]]) - 1] = array[j];
                buffer_length[(count[(unsigned int) array[j][i]]--) - 1] = array_lengths[j];
            }
            else {
                buffer[(count[0]) - 1] = array[j];
                buffer_length[(count[0]--) - 1] = array_lengths[j];
            }
        }
        memcpy(array, buffer, size * sizeof(char*));
        memcpy(array_lengths, buffer_length, size * sizeof(size_t));
    }
}

