CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
GNU = -D_GNU_SOURCE

s21_cat: cat/s21_cat.c
	$(CC) $(FLAGS) $(GNU) cat/s21_cat.c -o s21_cat

s21_grep: grep/s21_grep.c
	$(CC) $(FLAGS) $(GNU) grep/s21_grep.c -o s21_grep

clean:
	rm -f s21_cat s21_grep

.PHONY: s21_cat s21_grep clean