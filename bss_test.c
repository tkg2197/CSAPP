#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>

#define SIZE (256 * 1024 * 1024) // 256MB
char a[SIZE]; // 一个没有被初始化的大数组

int main(void) {
    int temp;
    printf("pid = %d\n", getpid()); // 获取进程的pid方便查看内存分布情况
    printf("1.\n");
    getchar(); // 暂停
    for (int i = 0; i < SIZE; i++) {
        temp = a[i]; // 读取数组
        if (i < 20) {
            printf("%d", a[i]); // 打印出数组的前20位
        }
    }
    printf("2.\n");
    getchar(); // 暂停
    memset(a, 1, SIZE); // 在数组中写入内容
    printf("3.\n");
    getchar();
    // 对齐数组内存，可以不用在意
    long pagesize = sysconf(_SC_PAGESIZE);            
    uintptr_t start = (uintptr_t)a;
    uintptr_t aligned = (start + pagesize - 1) & ~(pagesize - 1); 
    size_t len = SIZE - (aligned - start);
    len &= ~(pagesize - 1);                            
    // 强制把数组从主存中换出
    if (madvise((void *)aligned, len, MADV_PAGEOUT) != 0)
        perror("madvise");   
    getchar();
    return 0;
}
