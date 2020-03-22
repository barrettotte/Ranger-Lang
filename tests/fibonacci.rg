/* Compute fibonacci number using iterative method 
     since my language does not have functions.
*/

fib := 1;
read n;

if(n > 1);
  prev := 1;
  i := 2;

  dow(i < n);
    temp := fib;
    fib := fib + prev;
    prev := temp;
    i := i + 1;
  enddo;
else;
  fib := n;
endif;

write fib;
