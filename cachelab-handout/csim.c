#include "cachelab.h"
#include <getopt.h> // getopt
#include <stdio.h>
#include <stdlib.h> // atoi


void print_usage();
void printSummary(int hits, int misses, int evictions);

typedef struct line
{
    int S;
    int time;
    int tag;
}line;
void simulate_cache(line *cache, char op, unsigned long address);
int main(int argc, char* argv[])
{
    int verbose = 0; // 是否启用详细模式
    int s = -1, E = -1, b = -1; // 参数初始化为无效值
    int S=-1;
    char* tracefile = NULL;
    int option;
    while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (option)
        {
        case 'h':
            print_usage();
            exit(0);
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            S = 1<<s;
            printf("%d\n", s);
            break;
        case 'E':
            E = atoi(optarg);
            printf("%d\n", E);
            break;
        case 'b':
            b = atoi(optarg);
            printf("%d\n", b);
            break;
        case 't':
            tracefile = optarg;
            break;
        default:
            print_usage();
            exit(0);
        }
    }//读取指令

    line *cache = (line *)calloc(S * E , sizeof(line));

    FILE *file = fopen(tracefile, "r");
    char op;
    unsigned long address;
    int size;
    while (fscanf(file, " %c %lx,%d", &op, &address, &size) == 3) {
        if (op == 'I') continue;  // 忽略指令加载
        simulate_cache(cache, op, address);
    }
    fclose(file);
    free(cache);


    return 0;
}
void simulate_cache(line *cache, char op, unsigned long address)
{

}

void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d", hits, misses, evictions);
}

void print_usage()
{
    printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
    printf("Options:\n");
    printf("  -h: Optional help flag that prints usage info\n");
    printf("  -v: Optional verbose flag that displays trace info\n");
    printf("  -s <s>: Number of set index bits (S = 2^s is the number of sets)\n");
    printf("  -E <E>: Associativity (number of lines per set)\n");
    printf("  -b <b>: Number of block bits (B = 2^b is the block size)\n");
    printf("  -t <tracefile>: Name of the valgrind trace to replay\n");
    exit(0);
}
