/* Compute fibonacci number
   using iterative method since
   my language doesn't have functions.
*/

read n;

fib := 1;


if(n > 0);
  prevFib := 1;
  i := 2;

  dow(i < n);
    temp := fib;
    fib := fib + prevFib;
    prevFib := temp;

    i := i + 1;
  enddo;
else;
  fib := n;
endif;

write fib;
