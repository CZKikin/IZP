sheet: sheet.c
	cc sheet.c -std=c99 -Wall -Wextra -Werror -o sheet
clean:
	rm -rf sheet
