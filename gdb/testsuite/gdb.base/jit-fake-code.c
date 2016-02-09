#include <stdio.h>

void ** jit_code_fake(void) {
    static void *addresses[7];
    line_1:
    printf("jit-fake: jit_code_fake line_1\n");
    addresses[0] = &&line_1;
    line_2:
    printf("jit-fake: jit_code_fake line_2\n");
    addresses[1] = &&line_2;
    line_3:
    printf("jit-fake: jit_code_fake line_3\n");
    addresses[2] = &&line_3;
    line_4:
    printf("jit-fake: jit_code_fake line_4\n");
    addresses[3] = &&line_4;
    line_5:
    printf("jit-fake: jit_code_fake line_5\n");
    addresses[4] = &&line_5;
    line_6:
    printf("jit-fake: jit_code_fake line_6\n");
    addresses[5] = &&line_6;
    line_7:
    printf("jit-fake: jit_code_fake line_7\n");
    addresses[6] = &&line_7;
    return addresses;
}