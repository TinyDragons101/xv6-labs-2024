#include "stdio.h"

int main() {
    char a[3][3] = {"ab", "fa", "ac"};
    printf("%s", *(a+1));
}