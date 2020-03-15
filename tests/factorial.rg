/* Compute a factorial
  in RangerLang
*/

read x; /* input an integer */

if(x > 0);
  fact := 1;

  dow(x = 0);
    fact := fact * x;
    x := x - 1;
  enddo;

  write fact;
  
endif;
