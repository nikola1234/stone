
#include <iostream>
//  [30] [27] [84] [65] [73] [69] [12] [33] [92] [45]
//    i=0                                           j=9    临界值 X = arr[i]

void PrintArray(int* arr, int len) 
{
    for(int k = 0; k < len; k++)
        std::cout << arr[k] << " ";
    std::cout << std::endl;
}

//不断的选取临界值
void QuickSort(int* arr, int left, int right)
{
    //std::cout << "arr size : " << sizeof(arr)<< std::endl;
    int  cri_value = 0 , tmp = 0; // critical临界值 交换的临时值
    int i = left;
    int j = right;
    cri_value = arr[i];
    if (left < right) {
        // 左右分治
        while (i < j) {
            // 1、i = 0, j = 9
            while (i < j && arr[j] >= cri_value)  //从右向左循环
                j--;
            // 1、i = 0, j = 6 
            if (i < j)
                arr[i++] = arr[j];
            // 1、i = 0, j = 6 
            while (i < j && arr[i] <= cri_value)
                i++;
            // 1、i = 2, j = 6 
            if (i < j)
                arr[j--] = arr[i];
        }
        arr[i] = cri_value;
        std::cout << "left : " << left << std::endl;
        std::cout << "right : " << right << std::endl;
        PrintArray(arr, 10);
        QuickSort(arr, left, i - 1); // 递归调用  
        QuickSort(arr, i + 1, right);  
    }
}

int main(int argc, char const *argv[])
{
    int array[]={30, 27, 84, 65, 73, 69, 12, 33, 92, 45};
    int len= sizeof(array) / sizeof(int); // 40/4
    std::cout << "The orginal arrayare:" << std::endl;
    PrintArray(array, len);
    QuickSort(array, 0, len-1);
    std::cout<<"The sorted arrayare:"<<std::endl;
    PrintArray(array, len); 
    return 0;
}
