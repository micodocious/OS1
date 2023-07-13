#!/bin/bash
#run-tests.sh
export BASE=/class/cs344/tree-tests
echo "Starting Test Collection Script..."

mkdir tests

echo "Test 0 of 19:"
mkdir ./tests/test0
gcc -std=gnu99 -ltree_testbench -o tree tree.c > ./tests/test0/test0-results

echo "Test 1 of 19:"
mkdir ./tests/test1
./tree -- $BASE/test_02 > ./tests/test1/test1-results
tree $BASE/test_02 > ./tests/test1/test1-linux

echo "Test 2 of 19:"
mkdir ./tests/test2
./tree -- '' > ./tests/test2/test2-results
echo $? > ./tests/test2/test2-last-statusResults

echo "Test 3 of 19:"
mkdir ./tests/test3
./tree -r -- $BASE/test_02 > ./tests/test3/test3-results0
./tree -- $BASE/test_02 > ./tests/test3/test3-results1


echo "Test 4 of 19:"
mkdir ./tests/test4
./tree -d -- $BASE/test_03 > ./tests/test4/test4-results0
./tree -- $BASE/test_03 > ./tests/test4/test4-results1


echo "Test 5 of 19:"
mkdir ./tests/test5
./tree -a -- $BASE/test_07 > ./tests/test5/test5-results0
./tree  -- $BASE/test_07 > ./tests/test5/test5-results1
diff -y ./tests/test5/test5-results0 ./tests/test5/test5-results1 > ./tests/test5/test5-diff

echo "Test 6 of 19:"
mkdir ./tests/test6
./tree -s -- $BASE/test_08 > ./tests/test6/test6-results

echo "Test 7 of 19:"
mkdir ./tests/test7
gcc -std=c99 -ltree_testbench -o tree tree.c > ./tests/test7/test7-results

echo "Test 8 of 19:"
mkdir ./tests/test8
./tree -- $BASE/test_04 > ./tests/test8/test8-results

echo "Test 9 of 19:"
mkdir ./tests/test9
./tree -d -- $BASE/test_03 > ./tests/test9/test9-result0
./tree -a -- $BASE/test_07 > ./tests/test9/test9-result1
./tree    -- $BASE/test_07 > ./tests/test9/test9-result2

echo "Test 10 of 19:"
mkdir ./tests/test10
./tree -d -- $BASE/test_09 > ./tests/test10/test10-result0
./tree -a -- $BASE/test_09 > ./tests/test10/test10-result1
./tree    -- $BASE/test_09 > ./tests/test10/test10-result2

echo "Test 11 of 19:"
mkdir ./tests/test11
for OPTS in a d ad
    do
        ./tree -$OPTS -- $BASE/test_10 >> ./tests/test11/test11-results0
        printf "\n\n" >> ./tests/test11/test11-results0
    done

echo "Test 12 of 19:"
mkdir ./tests/test12
for OPTS in p u g s
    do
        ./tree -$OPTS -- $BASE/test_10 >> ./tests/test12/test12-results0
        printf "\n\n" >> ./tests/test12/test12-results0
    done

echo "Test 13 of 19:"
mkdir ./tests/test13
for OPTS in p u g s a d ad
    do
        ./tree -$OPTS -- $BASE/test_10 >> ./tests/test13/test13-results
        printf "\n\n" >> ./tests/test13/test13-results
    done

echo "Test 14 of 19:"
mkdir ./tests/test14
./tree -pugs    -- $BASE/test_10 >> ./tests/test14/test14-results0
./tree -pugs -a -- $BASE/test_10 >> ./tests/test14/test14-results1

echo "Test 15 of 19:"
mkdir ./tests/test15
gcc -std=c99 -ltree_testbench -Wall -Wextra -Wpedantic -Werror -o tree tree.c > ./tests/test15/test15-results

echo "Test 16 of 19:"
mkdir ./tests/test16
./tree -pugs -a -- $BASE/test_10 > ./tests/test16/test16-results

echo "Test 17 of 19:"
mkdir ./tests/test17
./tree -pugs    -r -- $BASE/test_10 > ./tests/test17/test17-results0
./tree -pugs    -U -- $BASE/test_10 > ./tests/test17/test17-results1
./tree -pugs    -d -- $BASE/test_10 > ./tests/test17/test17-results2
./tree -pugs -a -r -- $BASE/test_10 > ./tests/test17/test17-results3
./tree -pugs -a -U -- $BASE/test_10 > ./tests/test17/test17-results4
./tree -pugs -a -d -- $BASE/test_10 > ./tests/test17/test17-results5

echo "Test 18 of 19:"
mkdir ./tests/test18
for OPTS in {p,}{u,}{g,}{s,}{a,}{d,}{U,r,t,}
    do
        ./tree "${OPTS:+"-$OPTS"}" -- $BASE/test_10 >> ./tests/test18/test18-results
        printf "\n\n" >> ./tests/test18/test18-results
    done

echo "Test 19 of 19:"
mkdir ./tests/test19
valgrind ./tree -pugsa -r -- $BASE/test_10 >> ./tests/test19/test19-results
