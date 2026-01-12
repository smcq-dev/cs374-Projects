#include <stdio.h>


int main(void) {
int x = 12;
float y = 3.14;
char* s = "Hello, world!";

printf("x is %d, y is %f \n%s\n", x, y, s);

int multipland = 5;

for (int i = 0; i<5; i++) {
    int result = multipland * i;
    printf("%d x %d = %d\n", i, multipland, result);
}

return 0;
}
