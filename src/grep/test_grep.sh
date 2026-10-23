set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

make s21_grep
LEAK='valgrind --tool=memcheck --leak-check=yes'

{
    echo "apple banana cherry"
    echo "Apple Banana Cherry"
    echo "123 456 error: failed to connect"
    echo ""
    echo ""
    echo "    "
    echo -e "\t\t"
    echo "This is a very long line created specifically for buffering tests of the s21_grep utility implementation sizes."
    echo "Text with BEL character: at the end"
    echo "Overwrite"
    echo "Last line without newline" | tr -d '\n'
    echo
    echo "error again"
    echo "APPLE is here"
    echo "banana"
} > test_grep.txt

{
    echo "apple pie"
    echo "Banana split"
    echo "cherry on top"
    echo "no match here"
    echo "ERROR: something"
    echo ""
    echo "apple"
} > test_grep2.txt

run_test() {
    local name_test=$1
    shift

    if /bin/grep "$@" | diff - <(./s21_grep "$@"); then
        echo -e "TEST $name_test ${GREEN}[OK]${NC}"
    else 
        echo -e "TEST $name_test ${RED}[FAIL]${NC}"

        echo -e "${GREEN} Ожидаемый результат$"
        /bin/grep "$@" || true
        echo -e "${RED} Полученный результат$"
        ./s21_grep "$@" || true

        exit 1
    fi
}

run_test "apple" "apple" test_grep.txt
run_test "-e" -e "apple" test_grep.txt
run_test "-i" -i "apple" test_grep.txt
run_test "-v" -v "apple" test_grep.txt
run_test "-c" -c "apple" test_grep.txt
run_test "-l" -l "apple" test_grep.txt
run_test "-n" -n "apple" test_grep.txt
echo " "
run_test "-ie" -ie "APPLE" test_grep.txt
run_test "-iv" -iv "apple" test_grep.txt
run_test "-e multiple" -e "apple" -e "error" test_grep.txt
run_test "-ie multiple" -ie "APPLE" -e "error" test_grep.txt
echo " "
run_test "multi simple" "apple" test_grep.txt test_grep2.txt
run_test "multi -n" -n "apple" test_grep.txt test_grep2.txt
run_test "multi -ic" -ic "apple" test_grep.txt test_grep2.txt
run_test "multi -il" -il "apple" test_grep.txt test_grep2.txt

rm -f s21_grep test_grep.txt test_grep2.txt