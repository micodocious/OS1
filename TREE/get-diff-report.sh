#!/bin/bash
#get-diff-report.sh
export BASE=/class/cs344/tree-tests
echo "Setup Starting..."
./run-tests.sh
./get-valid-results.sh
echo "Comapre Script..."

mkdir cmp

echo "Test 1 of 19:" >> ./cmp/report

cmp ./valids/test1/test1-results ./tests/test1/test1-results >> ./cmp/report

echo "Test 2 of 19:" >> ./cmp/report

cmp ./valids/test2/test2-results ./tests/test2/test2-results >> ./cmp/report
cmp ./valids/test2/test2-last-statusResults ./tests/test2/test2-last-statusResults >> ./cmp/report

echo "Test 3 of 19:" >> ./cmp/report

cmp ./valids/test3/test3-results0 ./tests/test3/test3-results0 >> ./cmp/report
cmp ./valids/test3/test3-results1 ./tests/test3/test3-results1 >> ./cmp/report


echo "Test 4 of 19:" >> ./cmp/report

cmp ./valids/test4/test4-results0 ./tests/test4/test4-results0 >> ./cmp/report
cmp ./valids/test4/test4-results1 ./tests/test4/test4-results1 >> ./cmp/report


echo "Test 5 of 19:" >> ./cmp/report

cmp ./valids/test5/test5-results0 ./tests/test5/test5-results0 >> ./cmp/report
cmp ./valids/test5/test5-results1 ./tests/test5/test5-results1 >> ./cmp/report


echo "Test 6 of 19:" >> ./cmp/report

cmp ./valids/test6/test6-results ./tests/test6/test6-results >> ./cmp/report


echo "Test 8 of 19:" >> ./cmp/report

cmp ./valids/test8/test8-results ./tests/test8/test8-results >> ./cmp/report

echo "Test 9 of 19:" >> ./cmp/report

cmp ./valids/test9/test9-result0 ./tests/test9/test9-result0 >> ./cmp/report
cmp ./valids/test9/test9-result1 ./tests/test9/test9-result1 >> ./cmp/report
cmp ./valids/test9/test9-result2 ./tests/test9/test9-result2 >> ./cmp/report

echo "Test 10 of 19:" >> ./cmp/report

cmp ./valids/test10/test10-result0 ./tests/test10/test10-result0 >> ./cmp/report
cmp ./valids/test10/test10-result1 ./tests/test10/test10-result1 >> ./cmp/report
cmp ./valids/test10/test10-result2 ./tests/test10/test10-result2 >> ./cmp/report

echo "Test 11 of 19:" >> ./cmp/report

cmp ./valids/test11/test11-results0 ./tests/test11/test11-results0 >> ./cmp/report


echo "Test 12 of 19:" >> ./cmp/report

cmp ./valids/test12/test12-results0 ./tests/test12/test12-results0 >> ./cmp/report

echo "Test 13 of 19:" >> ./cmp/report

cmp ./valids/test13/test13-results ./tests/test13/test13-results >> ./cmp/report


echo "Test 14 of 19:" >> ./cmp/report

cmp ./valids/test14/test14-results0 ./tests/test14/test14-results0 >> ./cmp/report
cmp ./valids/test14/test14-results1 ./tests/test14/test14-results1 >> ./cmp/report


echo "Test 16 of 19:" >> ./cmp/report

cmp ./valids/test16/test16-results ./tests/test16/test16-results >> ./cmp/report

echo "Test 17 of 19:" >> ./cmp/report

cmp ./valids/test17/test17-results0 ./tests/test17/test17-results0 >> ./cmp/report
cmp ./valids/test17/test17-results1 ./tests/test17/test17-results1 >> ./cmp/report
cmp ./valids/test17/test17-results2 ./tests/test17/test17-results2 >> ./cmp/report
cmp ./valids/test17/test17-results3 ./tests/test17/test17-results3 >> ./cmp/report
cmp ./valids/test17/test17-results4 ./tests/test17/test17-results4 >> ./cmp/report
cmp ./valids/test17/test17-results5 ./tests/test17/test17-results5 >> ./cmp/report

echo "Test 18 of 19:" >> ./cmp/report

cmp ./valids/test18/test18-results ./tests/test18/test18-results >> ./cmp/report
