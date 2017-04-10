# SUDOKU(9x9) Solver

-Sudoku solver for 9x9 problems.  
-Brute force implementation.  
-Does guessing and if wrong backtracks.  

### To Execute:-

Download the source code or copy paste it.

```
> gcc -o <binary_file_name> <source_code_filename>
> ./<binary_file_name>
```

Give the numbers as inputs and give -1 for the blanks (from left to right, top to bottom).

### To Do:-
- The possible values in each blank is now calculated in every iteration. This can be optimized by only updating the possible values on the row, column and square grid(3x3) when a blank is filled.
