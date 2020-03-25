/* Compute fibonacci number using iterative method 
     since my language does not have functions.
*/

read n;
fib := 1;

if(n > 1);
  temp := 0;
  prev := 1;
  i := 2;

  dow(i < (n-1));
    temp := fib;
    fib := fib + prev;
    prev := temp;
    i := i + 1;
  enddo;
endif;

write fib;
