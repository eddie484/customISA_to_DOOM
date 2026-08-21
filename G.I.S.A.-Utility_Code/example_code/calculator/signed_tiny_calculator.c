int scan_uart(void) {
    return 1;
}

int print_uart(int send_data) {
    return 1;
}


int print_int(int result_data) {
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
    int h = 0;
    int i = 0;

    if (result_data < 0) {
        result_data = -result_data;
        print_uart(45);
    }

    while (result_data >= 1000000000) {
        result_data -= 1000000000;
        i++;
    }

    while (result_data >= 100000000) {
        result_data -= 100000000;
        h++;
    }

    while (result_data >= 10000000) {
        result_data -= 10000000;
        g++;
    }

    while (result_data >= 1000000) {
        result_data -= 1000000;
        f++;
    }

    while (result_data >= 100000) {
        result_data -= 100000;
        e++;
    }

    while (result_data >= 10000) {
        result_data -= 10000;
        d++;
    }

    while (result_data >= 1000) {
        result_data -= 1000;
        c++;
    }

    while (result_data >= 100) {
        result_data -= 100;
        b++;
    }

    while (result_data >= 10) {
        result_data -= 10;
        a++;
    }

    


    if (i >= 1) {
        i += 48;
        print_uart(i);
    }

    if (h >= 1 || i >= 1) {
        h += 48;
        print_uart(h);
    }

    if (g >= 1 || h >= 1 || i >= 1) {
        g += 48;
        print_uart(g);
    }

    if (f >= 1 || g >= 1 || h >= 1 || i >= 1) {
        f += 48;
        print_uart(f);
    }

    if (e >= 1 || f >= 1 || g >= 1 || h >= 1 || i >= 1) {
        e += 48;
        print_uart(e);
    }

    if (d >= 1 || e >= 1 || f >= 1 || g >= 1 || h >= 1 || i >= 1) {
        d += 48;
        print_uart(d);
    }

    if (c >= 1 || d >= 1 || e >= 1 || f >= 1 || g >= 1 || h >= 1 || i >= 1) {
        c += 48;
        print_uart(c);
    }

    if (b >= 1 || c >= 1 || d >= 1 || e >= 1 || f >= 1 || g >= 1 || h >= 1 || i >= 1) {
        b += 48;
        print_uart(b);
    }

    if (a >= 1 || b >= 1 || c >= 1 || d >= 1 || e >= 1 || f >= 1 || g >= 1 || h >= 1 || i >= 1) {
        a += 48;
        print_uart(a);
    }

    result_data += 48;
    print_uart(result_data);


    i = 0;
    h = 0;
    g = 0;
    f = 0;
    e = 0;
    d = 0;
    c = 0;
    b = 0;
    a = 0;
    result_data = 0;

    return 0;
}

int scan_int(void) {
    int input = 0;
    int value = 0;
    int is_minus = 0;

    while (!((input == 45) || (input >= 48 && input <= 57))) {
        input = scan_uart();
        if (input == 45) {
            print_uart(input);
            is_minus = 1;
        } else if (input >= 48 && input <= 57) {
            print_uart(input);

            value *= 10;
            value += (input - 48);
        }
    }
    

    while (1) {
        input = scan_uart();
        if (input == 13 || input == 10) {
            if (is_minus == 1) {
                value = -value;
            }
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
