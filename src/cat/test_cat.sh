set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

make s21_cat
LEAK='valgrind --tool=memcheck --leak-check=yes'

{
    echo "apple banana cherry"
    echo "Apple Banana Cherry"
    printf "123 456 error:\t\tfailed to connect\n"
    echo ""
    echo ""
    echo ""
    printf "    \n"      
    printf "\t\t\n"    
    printf "This is a very long line created specifically for buffering tests of the s21_cat utility implementation sizes.\n"
    printf "Text with BEL character:\a at the end\n"
    printf "Overwrite\rNextLine\n"
    printf "Last line without newline"
} > test_cat.txt
printf "apple banana cherry\nApple Banana Cherry\n123 456 error: failed to connect\n" > test_cat2.txt

run_test() {
    local name_test=$1
    shift

    if /bin/cat "$@" | diff - <(./s21_cat "$@"); then
        echo -e "TEST $name_test ${GREEN}[OK]${NC}"
    else 
        echo -e "TEST $name_test ${RED}[FAIL]${NC}"

        echo -e "${GREEN} Ожидаемый результат$"
        /bin/cat "$@" || true
        echo -e "${RED} Полученный результат$"
        ./s21_cat "$@" || true

        exit 1
    fi
}

run_test "-b" -b test_cat.txt
run_test "-v" -v test_cat.txt
run_test "-e" -e test_cat.txt
run_test "-E" -E test_cat.txt
run_test "-n" -n test_cat.txt
run_test "-s" -s test_cat.txt
run_test "-t" -t test_cat.txt
run_test "-T" -T test_cat.txt
echo " "
run_test "-nb" -nb test_cat.txt
run_test "-se" -se test_cat.txt
run_test "-vET" -vET test_cat.txt
echo " "
run_test "multi -s" -s test_cat.txt test_cat2.txt
run_test "multi -b" -b test_cat.txt test_cat2.txt
run_test "multi -nb" -nb test_cat.txt test_cat2.txt
echo " "
run_test "multi --number-nonblank" --number-nonblank test_cat.txt
run_test "multi --number" --number test_cat.txt
run_test "multi --squeeze-blank" --squeeze-blank test_cat.txt

rm -f s21_cat test_cat.txt test_cat2.txt