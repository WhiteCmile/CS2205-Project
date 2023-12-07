#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include "lib.hpp"

long long build_nat(char * c, int len) {
    long long s = 0, i = 0;
    long long ll_max = LLONG_MAX;
    for (i = 0; i < len; ++i) {
        if (s > ll_max / 10) {
            printf("We cannot handle natural numbers greater than 0x7fffffffffffffff.\n");
            exit(0);
        }
        if (s == 922337203685477580ll && c[i] > '7') {
            printf("We cannot handle natural numbers greater than 0x7fffffffffffffff.\n");
            exit(0);
        }
        s = s * 10 + (c[i] - '0');
    }
    return s;
}

long long build_neg_nat(char * c, int len) {
    long long s = 0, i = 0;
    long long ll_min = LLONG_MIN;
    for (i = 1; i < len; ++i) {
        if (s < -922337203685477580ll) {
            printf("We cannot handle natural numbers less than 0x8000000000000000.\n");
            exit(0);
        }
        if (s == -922337203685477580ll && c[i] > '8') {
            printf("We cannot handle natural numbers less than 0x8000000000000000.\n");
            exit(0);
        }
        if (i == 1)
            s = - c[i] + '0';
        else s = s * 10 + (c[i] - '0');
    }
    return s;
}

char * new_str(char * str, int len) {
    char * res = (char *) malloc(sizeof(char) * (len + 1));
    if (res == NULL) {
        printf("Failure in malloc.\n");
        exit(0);
    }
    strcpy(res, str);
    return res;
}