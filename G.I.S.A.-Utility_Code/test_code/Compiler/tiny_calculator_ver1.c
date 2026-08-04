int scan_uart(void) {
    return 1;
}

int print_uart(int send_data) {
    return 1;
}


int print_int(int result_data) {
    int ten = 0;
    int hun = 0;

    while (result_data >= 100) {
        result_data -= 100;
        hun++;
    }

    while (result_data >= 10) {
        result_data -= 10;
        ten++;
    }

    if (hun >= 1) {
        hun += 48;
        print_uart(hun);
    }

    if (ten >= 1 || hun >= 1) {
        ten += 48;
        print_uart(ten);
    }

    result_data += 48;
    print_uart(result_data);

    hun = 0;
    ten = 0;
    result_data = 0;

    return 0;
}

int scan_int(void) {
    int input;
    int value = 0;

    while (1) {
        input = scan_uart();
        if (input == 13 || input == 10) {
            return value;
        } else if (input >= 48 && input <= 57) {
            print_uart(input);

            value *= 10;
            value += (input - 48);
        }
    }
}


int main(void) {
    int value_A;
    int value_B;
    int cal = 0;
    int result;

    print_uart(42);
    print_uart(42);
    print_uart(42);
    print_uart(99);
    print_uart(97);
    print_uart(108);
    print_uart(99);
    print_uart(117);
    print_uart(108);
    print_uart(97);
    print_uart(116);
    print_uart(101);
    print_uart(114);
    print_uart(42);
    print_uart(42);
    print_uart(42);
    print_uart(13);
    print_uart(10);     

    while (1) {
        value_A = scan_int();
        print_uart(32);

        while (cal != 42 && cal != 43 && cal != 45 && cal != 47 && cal != 37) {
            cal = scan_uart();
        }
        print_uart(cal);
        print_uart(32);

        value_B = scan_int();
        print_uart(32);
        print_uart(61);
        print_uart(32);

        if (cal == 43) {
            result = value_A + value_B;
        } else if (cal == 45) {
            result = value_A - value_B;
        } else if (cal == 42) {
            result = value_A * value_B;
        } else if (cal == 47) {
            result = value_A / value_B;
        } else if (cal == 37) {
            result = value_A % value_B;
        } 

        print_int(result);
        print_uart(13);
        print_uart(10);        
        cal = 0;
    }
    

    return 15;
}
