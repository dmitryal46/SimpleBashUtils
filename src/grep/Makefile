SHELL := /bin/bash
FLAG =-Wall -Wextra -Werror
FILE =*.txt

all: s21_grep tests

s21_grep: clean
	gcc $(FLAG) grep.c -o s21_grep

tests:
	-diff <(./s21_grep main $(FILE)) <(grep main $(FILE))
	-diff <(./s21_grep -ve main $(FILE)) <(grep -ve main $(FILE))
	-diff <(./s21_grep -i MAIN $(FILE)) <(grep -i MAIN $(FILE))
	-diff <(./s21_grep -iv MAIN $(FILE)) <(grep -iv MAIN $(FILE))
	-diff <(./s21_grep -in MAIN $(FILE)) <(grep -in MAIN $(FILE))
	-diff <(./s21_grep -ie "^Void" $(FILE)) <(grep -ie "^Void" $(FILE))
	-diff <(./s21_grep -ei $(FILE)) <(grep -ei $(FILE))
	-diff <(./s21_grep -ince "^Void" $(FILE)) <(grep -ince "^Void" $(FILE))
	-diff <(./s21_grep -lh "^int" $(FILE)) <(grep -lh "^int" $(FILE))
	-diff <(./s21_grep -hc flag $(FILE)) <(grep -hc flag $(FILE))
	-diff <(./s21_grep -hc int $(FILE)) <(grep -hc int $(FILE))

	-diff <(./s21_grep -e main -e int $(FILE)) <(grep -e main -e int $(FILE))
	-diff <(./s21_grep -s main nofile.txt) <(grep -e main -s main nofile.txt)

	-diff <(./s21_grep -f regfile.txt $(FILE)) <(grep -f regfile.txt $(FILE))
	-diff <(./s21_grep -f regfile2.txt $(FILE)) <(grep -f regfile2.txt $(FILE))
	
	-diff <(./s21_grep -oi MAIN $(FILE)) <(grep -oi MAIN $(FILE))
	-diff <(./s21_grep -ohcf regfile.txt $(FILE)) <(grep -ohcf regfile.txt $(FILE))
	-diff <(./s21_grep -oinc Int $(FILE)) <(grep -oinc Int $(FILE))
	-diff <(./s21_grep -oihln -e int -e Ma $(FILE)) <(grep -oihln -e int -e Ma $(FILE))


clean:
	rm -f s21_grep