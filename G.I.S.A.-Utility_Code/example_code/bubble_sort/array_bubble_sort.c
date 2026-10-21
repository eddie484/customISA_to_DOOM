int scan_uart(void) {
    return 1;
}

int print_uart(int send_data) {
    return 1;
}

int main(void) {
    int input[10];
    for (int i = 0; i < 10; i++) {
        input[i] = scan_uart();
    }

    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 9 - i; j++) {
            if (input[j] > input[j + 1]) {
                int temp = input[j + 1];
                input[j + 1] = input[j];
                input [j] = temp;
            }
        }
    }
    for(int i = 0; i < 10; i++) {
        print_uart(input[i]);
    }

    return 0;
}