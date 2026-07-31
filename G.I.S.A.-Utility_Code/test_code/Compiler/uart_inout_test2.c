int scar_uart(void) {
    return 1;
}

int print_uart(int send_data) {
    return 1;
}

int main(void) {
    int x;
    int y = 0;
    while (1) {
        x = scar_uart();
        print_uart(x);
        if (x == 13 || x == 10) {
            if (y >= 10) y = 10;
            y += 48;
            print_uart(y);
            y = 0;
        } else if (x >= 48 && x <= 57) {
            y += (x - 48);
        }
        
    }
    

    return 15;
}
