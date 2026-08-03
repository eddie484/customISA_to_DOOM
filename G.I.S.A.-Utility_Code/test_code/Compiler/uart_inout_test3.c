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
        print_uart(105);
        print_uart(110);
        print_uart(112);
        print_uart(117);
        print_uart(116);
        print_uart(58);
        print_uart(32);
        print_uart(x);
        print_uart(13);
        print_uart(10);
        if (x == 13 || x == 10) {
            print_uart(111);
            print_uart(117);
            print_uart(116);
            print_uart(112);
            print_uart(117);
            print_uart(116);
            print_uart(58);
            print_uart(32);
            if (y >= 10) y = 10;
            y += 48;
            print_uart(y);
            y = 0;
            print_uart(13);
            print_uart(10);
        } else if (x >= 48 && x <= 57) {
            y += (x - 48);
        }
        
    }
    

    return 15;
}
