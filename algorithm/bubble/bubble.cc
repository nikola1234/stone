
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define SIZE 10
/*
    i = 0; 从最后两个开始比较，将最小值放在第一位，
    i = 1; 从最后两个开始比较，将最小值放在第二位，
    。。。。
*/

void BubbleSort(int* arr, int len)
{
    int i, j;
    for (i = 0; i < len - 1; i++)
    {
        for (j = len-1; j > i; j--)
        {
            if (arr[j-1] > arr[j])
                std::swap(arr[j-1], arr[j]);
        }
        std::cout << "第" <<  i << "步排序结果：";
        for (int k = 0; k < SIZE; k++) 
        {
            std::cout << arr[k] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    int arr[SIZE] = {0};
    srand(time(NULL)); // 用时间作为随机数的种子。
    for (int i = 0; i < SIZE; i++) 
    {
        arr[i] = rand()/100000;
    }
    std::cout << "排序前的数组： "<< std::endl;
    for (int i = 0; i < SIZE; i++) 
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    BubbleSort(arr, SIZE);
    std::cout << "排序后的数组： "<< std::endl;
    for (int i = 0; i < SIZE; i++) 
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}