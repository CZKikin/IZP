# IZP
Just another school project.
## Simple commands: 
- irow R - insert-row R: Row
- arow - append-row
- drow R - delete-row R: Row
- drows N M - delete-rows N: beginning row M: end 
- icol C - insert-column 
- acol - append-column
- dcol C - delete-col C: Column
- dcols N M - delete-cols N: beginning column M: end

## Commands for cell manipulation:
- cset C STR - in column C will be set to STR 
- tolower C - column C will be set to lower case 
- toupper C - column C will be set to higher case
- round C - nums in column C will be rounded 
- int C - nums in column C will be converted to int 
- copy N M - copies column N to M 
- swap N M - swaps columns N and M 
- move N M - moves N before M 
- csum C N M - column C will have a sum of lines between N and M
- cavg C N M - column C will have an average of lines between N and M
- cmin C N M - column C will have the minimal value of lines between N and M
- cmax C N M - column C will have the maximal value of lines between N and M
- ccount C N M - column C will have the count of empty cells in lines between N and M 
- cseq N M B - to cells in column N to M will have sequence of numbers beggining from B
- rseq C N M B - column C will in lines N to M will sequence of numbers beggining from B
- rsum C N M - column C on line M+1 will have sum of values from lines N to M
- ravg C N M - column C on line M+1 will have average of values from lines N to M
- rmin C N M - column C on line M+1 will have minimum of values from lines N to M
- rmax C N M - column C on line M+1 will have maximum of values from lines N to M
- rcount C N M - column C on line M+1 will have the count of epmty cells from lines N to M

## Cell selection:
- rows N M - select rows from N to M
- beginswith C STR - work only with colums C which have STR as a begining of the string 
- contains C STR - work only with colums C which have STR as the string 
