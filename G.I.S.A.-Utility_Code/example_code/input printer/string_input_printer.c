int scan_uart(void) {
    return 0;
}

int print_uart(void) {
    return 0;
}

int string_printer(char * input) {
    return 0;
}

int string_scanner(char * data_place) {
    int i = 0;
    while(1) {
        char input = scan_uart();
        if (input == 13) {
            data_place[i] = 0;
            return 0;
        } else if (input == 8 || input == 127) {
            if (i != 0) {
                i -= 1;
                data_place[i] = 0;
            }
        } else {
            data_place[i] = input;
            i += 1;
        }
    }
    
    return 1;
}


int main(void) {
    char input[50];
    string_printer("Hello! Input receiving!\n");
    while(1) {
        string_scanner(input);
        string_printer("Your input is: ");
        string_printer(input);
        string_printer("\n\n");
    }

    return 0;
}
