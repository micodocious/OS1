#!/bin/bash
#get-valid-results.sh
export BASE=/class/cs344/tree-tests

echo "Starting Test Collection Script..."

mkdir valids

echo "Test 0 of 19:"
mkdir ./valids/test0
#gcc -std=gnu99 -ltree_testbench -o tree tree.c > ./valids/test0/test0-results
echo"SKIPPED"

echo "Test 1 of 19:"
mkdir ./valids/test1
$BASE/tree -- $BASE/test_02 > ./valids/test1/test1-results
$BASE/tree $BASE/test_02 > ./valids/test1/test1-linux

echo "Test 2 of 19:"
mkdir ./valids/test2
$BASE/tree -- '' > ./valids/test2/test2-results
echo $? > ./valids/test2/test2-last-statusResults

echo "Test 3 of 19:"
mkdir ./valids/test3
$BASE/tree -r -- $BASE/test_02 > ./valids/test3/test3-results0
$BASE/tree -- $BASE/test_02 > ./valids/test3/test3-results1


echo "Test 4 of 19:"
mkdir ./valids/test4
$BASE/tree -d -- $BASE/test_03 > ./valids/test4/test4-results0
$BASE/tree -- $BASE/test_03 > ./valids/test4/test4-results1


echo "Test 5 of 19:"
mkdir ./valids/test5
$BASE/tree -a -- $BASE/test_07 > ./valids/test5/test5-results0
$BASE/tree  -- $BASE/test_07 > ./valids/test5/test5-results1
diff -y ./valids/test5/test5-results0 ./valids/test5/test5-results1 > ./valids/test5/test5-diff

echo "Test 6 of 19:"
mkdir ./valids/test6
$BASE/tree -s -- $BASE/test_08 > ./valids/test6/test6-results

echo "Test 7 of 19:"
mkdir ./valids/test7
gcc -std=c99 -ltree_testbench -o tree tree.c > ./valids/test7/test7-results

echo "Test 8 of 19:"
mkdir ./valids/test8
$BASE/tree -- $BASE/test_04 > ./valids/test8/test8-results

echo "Test 9 of 19:"
mkdir ./valids/test9
$BASE/tree -d -- $BASE/test_03 > ./valids/test9/test9-result0
$BASE/tree -a -- $BASE/test_07 > ./valids/test9/test9-result1
$BASE/tree    -- $BASE/test_07 > ./valids/test9/test9-result2

echo "Test 10 of 19:"
mkdir ./valids/test10
$BASE/tree -d -- $BASE/test_09 > ./valids/test10/test10-result0
$BASE/tree -a -- $BASE/test_09 > ./valids/test10/test10-result1
$BASE/tree    -- $BASE/test_09 > ./valids/test10/test10-result2

echo "Test 11 of 19:"
mkdir ./valids/test11
for OPTS in a d ad
    do
        $BASE/tree -$OPTS -- $BASE/test_10 >> ./valids/test11/test11-results0
        printf "\n\n" >> ./valids/test11/test11-results0
    done

echo "Test 12 of 19:"
mkdir ./valids/test12
for OPTS in p u g s
    do
        $BASE/tree -$OPTS -- $BASE/test_10 >> ./valids/test12/test12-results0
        printf "\n\n" >> ./valids/test12/test12-results0
    done

echo "Test 13 of 19:"
mkdir ./valids/test13
for OPTS in p u g s a d ad
    do
        $BASE/tree -$OPTS -- $BASE/test_10 >> ./valids/test13/test13-results
        printf "\n\n" >> ./valids/test13/test13-results
    done

echo "Test 14 of 19:"
mkdir ./valids/test14
$BASE/tree -pugs    -- $BASE/test_10 >> ./valids/test14/test14-results0
$BASE/tree -pugs -a -- $BASE/test_10 >> ./valids/test14/test14-results1

echo "Test 15 of 19:"
mkdir ./valids/test15
#gcc -std=c99 -ltree_testbench -Wall -Wextra -Wpedantic -Werror -o tree tree.c > ./valids/test15/test15-results
echo"SKIPPED"

echo "Test 16 of 19:"
mkdir ./valids/test16
$BASE/tree -pugs -a -- $BASE/test_10 > ./valids/test16/test16-results

echo "Test 17 of 19:"
mkdir ./valids/test17
$BASE/tree -pugs    -r -- $BASE/test_10 > ./valids/test17/test17-results0
$BASE/tree -pugs    -U -- $BASE/test_10 > ./valids/test17/test17-results1
$BASE/tree -pugs    -d -- $BASE/test_10 > ./valids/test17/test17-results2
$BASE/tree -pugs -a -r -- $BASE/test_10 > ./valids/test17/test17-results3
$BASE/tree -pugs -a -U -- $BASE/test_10 > ./valids/test17/test17-results4
$BASE/tree -pugs -a -d -- $BASE/test_10 > ./valids/test17/test17-results5

echo "Test 18 of 19:"
mkdir ./valids/test18
for OPTS in {p,}{u,}{g,}{s,}{a,}{d,}{U,r,t,}
    do
        $BASE/tree "${OPTS:+"-$OPTS"}" -- $BASE/test_10 >> ./valids/test18/test18-results
        printf "\n\n" >> ./valids/test18/test18-results
    done

echo "Test 19 of 19:"
mkdir ./valids/test19
#valgrind $BASE/tree -pugsa -r -- $BASE/test_10 >> ./valids/test19/test19-results
echo "SKIPPED"
