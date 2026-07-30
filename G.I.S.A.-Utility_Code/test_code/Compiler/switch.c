int main(void) {
    int a = 5;
    
    
    switch (a) {
        case 1:
        case 2:
            return 3;
        
        case 3: {
            a = a++ * 7;
            a /= 7;
            break;
        }

        default: a = a * a;
    }

    return a;
}
