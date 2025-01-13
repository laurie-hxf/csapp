#include "cachelab.h"
#include <getopt.h> /* getopt */
#include <stdio.h>
#include <stdlib.h> /* atoi */

void print_usage();
void printSummary(int hits, int misses, int evictions);

typedef struct line {
    int time;
    int tag;
} line;

void simulate_cache(line *cache, char op, unsigned long address, int b, int s, int E, int verbose,
                    int *hits, int *misses, int *evictions, int time, int size);

int main(int argc, char *argv[]) {
    int verbose = 0; /* 是否启用详细模式 */
    int s = -1, E = -1, b = -1; /* 参数初始化为无效值 */
    int S = -1;
    char *tracefile = NULL;
    int hits = 0;
    int misses = 0;
    int evictions = 0;
    int time = 0;
    int option;

    while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (option) {
        case 'h':
            print_usage();
            exit(0);
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            S = 1 << s;
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
    } /* 读取指令 */

    line *cache = (line *)calloc(S * E, sizeof(line));

    FILE *file = fopen(tracefile, "r");
    char op;
    unsigned long address;
    int size;
    while (fscanf(file, " %c %lx,%d", &op, &address, &size) == 3) {
        time++;
        if (op == 'I') continue; /* 忽略指令加载 */
        simulate_cache(cache, op, address, b, s, E, verbose, &hits, &misses, &evictions, time, size);
    }
    fclose(file);
    free(cache);

    return 0;
}

void simulate_cache(line *cache, char op, unsigned long address, int b, int s, int E, int verbose,
                    int *hits, int *misses, int *evictions, int time, int size) {
    int s_ = address >> b & ((1 << s) - 1);
    int tag_ = address >> (b + s);
    switch (op) {
    case 'L': {
        char judge = 1;
        for (int i = 0; i < E; i++) {
            if (!cache[s_ + i].time) {
                (*misses)++;
                cache[s_ + i].tag = tag_;
                cache[s_ + i].time = time;
                judge = 0;
                if (verbose) {
                    printf("%c %lx,%d miss\n", op, address, size);
                }
                break;
            }
            if (cache[s_ + i].tag == tag_) {
                (*hits)++;
                judge = 0;
                cache[s_ + i].time = time;
                if (verbose) {
                    printf("%c %lx,%d hit\n", op, address, size);
                }
            }
        }
        if (judge) { /* 缓存未命中 */
            (*misses)++;
            int record = cache[s_].time;
            int change = 0;
            for (int i = 1; i < E; i++) {
                if (cache[s_ + i].time < record) {
                    change = i;
                    record = cache[s_ + i].time;
                }
            }
            cache[s_ + change].time = time;
            cache[s_ + change].tag = tag_;
            if (verbose) {
                printf("%c %lx,%d miss eviction\n", op, address, size);
            }
        }
        break;
    }
    case 'M':
        break;

    case 'S':
        break;
    }
}

void printSummary(int hits, int misses, int evictions) {
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
}

void print_usage() {
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
