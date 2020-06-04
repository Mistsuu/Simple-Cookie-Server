#include <stdio.h>
#include <string.h>
int main() {
    char tmp[] = "Set-cookie: abc=def\n\rSet-cookie: Something=efg\n\r";
    char key[100];
    char value[100];

    int pos = 0;
    int count = 0;
    while((count = sscanf(tmp + pos, "Set-cookie: %s=%s\n\r", key, value)) > 0) {
        printf("%s,", key);
        printf("%s\n", value);

        printf("[] Pos: %d\n", pos);
        printf("[] Count: %d\n", count);
        printf("[] strlen(tmp): %d\n", strlen(tmp));

        if (pos + count >= strlen(tmp))
            break;

        pos += count;
    }
}
