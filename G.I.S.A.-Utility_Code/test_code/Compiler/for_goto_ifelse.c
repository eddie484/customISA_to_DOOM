int main(void) {
    int a = 5;
    
    
    for(int i = 0; i < 10; i++) {
        if (i == 5) {
            continue;
        }

        goto BEL;
        LA:
        a = 0;
        BEL:
        HELLO:
        a += i;

        if (i == 8) break;
        else if (i == 9 && a < 75) goto HELLO;
        
    }

    return a;
}
