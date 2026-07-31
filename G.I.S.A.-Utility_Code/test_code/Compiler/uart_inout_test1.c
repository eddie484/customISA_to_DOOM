int scar_uart(void) {
    return 1;
}

int print_uart(int send_data) {
    return 1;
}

int main(void) {
    int x = scar_uart();
    int y = x + 1;
    print_uart(y);

    return y;
}
