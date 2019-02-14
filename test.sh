#! /bin/bash

TASK_POINTS=10
NUMBER_OF_TESTS=9
TOTAL=10
EXE_NAME="hash_table"
RUN_BONUS=true

TEST_DIRECTORY="tests/"
REF_DIRECTORY="reference/"

function init {
    make build

    if [ $? -ne 0 ]; then
        echo -e "Makefile build rule failed!\n"
        exit 1
    fi

    echo "         TEMA 1"
    echo "       =HashTable="
}

function next_test {
    next_test="${TEST_DIRECTORY}test$1.in"

    cp "${next_test}" "input.in"
}

function run_test {
    timeout 60 ./${EXE_NAME} &> /dev/null
    if  [ $? -ne 0 ]; then
        echo -e "Run failed"
        return
    fi
}

function check_test {
    reference_test="${REF_DIRECTORY}ref$1.out"
    diff "${reference_test}" "output.out" > /dev/null

    if [ $? -eq 0 ]; then
        echo -e "Test ${1}...............OK"
        TOTAL=$((${TOTAL} + 10))
    else
        echo -e "Test ${1}...........FAILED"
    fi
}

function run_bonus {
    valgrind --leak-check=full --trace-children=yes --log-file="leak-hash" ./${EXE_NAME} &> /dev/null

    if [ $? -ne 0 ]; then
        echo -e "Problem running bonus test"
        rm -rf "leak-hash"
        return
    fi

    if grep -q "LEAK SUMMARY" leak-hash; then
        echo -e "Bonus.....Leak detected"
        return
    fi

    reference_test="${REF_DIRECTORY}ref9.out"
    diff "${reference_test}" "output.out" > /dev/null

    if [ $? -eq 0 ]; then
        echo -e "Bonus................OK"
        TOTAL=$((${TOTAL} + 20))
    else
        echo -e "Bonus..........FAILED"
    fi

}

function clean {
    rm -rf "input.in"
    rm -rf "output.out"
    #rm -rf "leak-hash"
}

init

if [ $# -eq 1 ]; then
    if [ "$1" == "no-bonus" ]; then
        RUN_BONUS=false
    else
        next_test "$1"
        run_test
        check_test "$1"
        exit
    fi
fi


# Run tests
for ((i=0; i<${NUMBER_OF_TESTS}; i++)); do
    next_test "$i"
    run_test
    check_test "$i"
done

# Run bonus

echo ""

if [ ${RUN_BONUS} == true ]; then
    next_test "9"
    run_bonus
fi

clean

echo -e "Total: ${TOTAL}"

if [ ${TOTAL} -eq 90 ]; then
    echo -e "Good job! Now go and make the bonus :)"
fi

if [ ${TOTAL} -eq 120 ]; then
    echo -e "Excelent!"
fi
