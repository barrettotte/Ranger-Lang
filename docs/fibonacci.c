// Sanity check...I thought I was coding this wrong in RangerLang

int fibonacci_iter(int n){
    int fib=1;
    if(n > 1){
        int temp = 0;
        int prev = 1;
        int i = 2;
        while(i < n){
            temp = fib;
            fib = fib + prev;
            prev = temp;
            i = i + 1;
        }
    } else{
        fib = n;
    }
    return fib;
}

int main(){
    printf("Iterative fibonacci: %d\n", fibonacci_iter(10));
}
