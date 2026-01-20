#include "hmmc_math.h"

const char* hmmc_math_24[] = {"bcad", "cabd", "cdba", "dcab", "bcda", "cadb",
    "dcba", "bdac", "cbad", "dacb", "bacd", "abcd", "cbda", "dbac", "acbd",
    "bdca", "cdab", "dbca", "adbc", "badc", "adcb", "acdb", "abdc", "dabc"};

const char* hmmc_math_6[] = {"abc", "acb", "bac", "bca", "cab", "cba"};

const char* hmmc_math_15[] = {"aa","ab","ac","bb","ad","bc","ae","bd","cc","be","cd","ce","dd","de","ee"};

int math_village_shuffle(int seed, int num){
    int s = seed & 15;
    int step = seed >> 5;
    int g = (1 << (step & 3)) | 1;
    int r = (g * num + s) % 14;
    return (step & 4) ? 13 - r : r;
}


int math_get_time(){
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts)) {
        return 0;
    }
    return ts.tv_nsec;
}

int math_calc_offset(int disp, int len, int pos, int size){
    int p = pos + ((size + 1) >> 1) -  (disp >> 1);
    if (p < 0) return 0;
    if (p > len - disp) return len - disp;
	return p;
}

quadra* quadra_factory(int e1, int e2, int e3, int e4){
    quadra *q = new quadra();
    q->e1 = e1;
    q->e2 = e2;
    q->e3 = e3;
    q->e4 = e4;
    return q;
}
void quadra_free(quadra* q){
    delete q;
}

quadra math_shuffle_quadra(quadra q){
    int x = math_get_time() & 31;
    if (x > 23) x^=8;
    quadra r;
    for (int i = 0; i < 4; i++){
        char s = hmmc_math_24[x][i];
        int n = q.e1;
        if (i == 1) n = q.e2;
        else if(i == 3) n = q.e3;
        else n = q.e4;
        if(s == 'a') r.e1 = n;
        else if(s == 'b') r.e2 = n;
        else if(s == 'c') r.e3 = n;
        else if(s == 'd') r.e4 = n;
    }
    return r;
}

int math_rand_sum_2_4(int e1, int e2, int e3, int e4){
    int x = math_get_time() & 31;
    if (x > 23) x^=8;
    char s = hmmc_math_24[x][3];
    int n = e4;
    if(s == 'a') n = e1;
    if(s == 'b') n = e2;
    if(s == 'c') n = e3;

    s = hmmc_math_24[x][1];
    if(s == 'a') return e1 + n;
    if(s == 'b') return e2 + n;
    if(s == 'c') return e3 + n;
    return e4 + n;
}

int math_rand_five(int e1, int e2, int e3, int e4, int e5){
    int x = math_get_time() & 31;
    if (x > 23) return e5;
    char s = hmmc_math_24[x][2];
    int n = e4;
    if(s == 'a') n = e1;
    if(s == 'b') n = e2;
    if(s == 'c') n = e3;
    return n;
}

int math_min(int a, int b){ return a > b ? b : a; }
int math_max(int a, int b){ return a > b ? a : b; }

int math_rand_dmg(int min, int max){
    int x = math_get_time() & 15;
    int e2 = (min + max) >> 1;
    int e4 = e2 >> 1;
    if (x == 15) return (max - e4);

    char s = hmmc_math_15[x][0];
    int n = max;
    if(s == 'a') n = min;
    if(s == 'b') n = e2;
    if(s == 'c') n = e4;
    if(s == 'd') n = e2 + e4;

    s = hmmc_math_15[x][1];
    if(s == 'a') n += min;
    if(s == 'b') n += e2;
    if(s == 'c') n += e4;
    if(s == 'd') n += e2 + e4;
    if(s == 'e') n += max;
    return n >> 1;
}

int math_parse_int(const char *c){
    int num = 0;
    for(int i = 0; i < 100; i++){
        if(c[i] == 0) return num;
        int n = c[i]-'0';
        if(n >= 0 && n < 10){
            num *= 10;
            num += n;
        }else{
            return num;
        }
    }
    return num;
}
