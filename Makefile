sheet: sheet.c
	gcc sheet.c -std=c99 -Wall -Wextra -Werror -o sheet -lm
clean:
	rm -rf sheet test_file
debug:
	gcc sheet.c -g -std=c99 -Wall -Wextra -Werror -o sheet -lm

dbm:
	gcc sheet.c -g3 -std=c99 -Wall -Wextra -Werror -o sheet -lm
