#! /bin/bash

set -u

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

GREEN='\033[0;32m'
RED='\033[0;31m'
OFF='\033[0m'
STATUS=0

ACCEPT_PASSED=0
ACCEPT_TOTAL=0
REJECT_PASSED=0
REJECT_TOTAL=0

echo "Compiler should accept..."
echo ""

for test in $(ls src/test/c/accept/); do
	cat "src/test/c/accept/$test" | build/Compiler >/dev/null 2>&1
	RESULT="$?"
	if [ "$RESULT" == "0" ]; then
		((ACCEPT_PASSED++))
		echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT)"
	else
		STATUS=1
		echo -e "    $test, ${RED}but it rejects${OFF} (status $RESULT)"
	fi
	((ACCEPT_TOTAL++))
done
echo ""

echo "Compiler should reject..."
echo ""

for test in $(ls src/test/c/reject/); do
	cat "src/test/c/reject/$test" | build/Compiler >/dev/null 2>&1
	RESULT="$?"
	if [ "$RESULT" != "0" ]; then
		((REJECT_PASSED++))
		echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT)"
	else
		STATUS=1
		echo -e "    $test, ${RED}but it accepts${OFF} (status $RESULT)"
	fi
	((REJECT_TOTAL++))
done
echo ""

printf "%-20s | %-10s | %-10s\n" "--------------------" "----------" "----------"
printf "%-20s | %-10s | %-10s\n" "Test Type" "Passed" "Total"
printf "%-20s | %-10s | %-10s\n" "--------------------" "----------" "----------"

if [ "$ACCEPT_PASSED" -eq "$ACCEPT_TOTAL" ]; then
  printf "%-20s | ${GREEN}%-10s${OFF} | %-10s\n" "Accept" "$ACCEPT_PASSED" "$ACCEPT_TOTAL"
else
  printf "%-20s | ${RED}%-10s${OFF} | %-10s\n" "Accept" "$ACCEPT_PASSED" "$ACCEPT_TOTAL"
fi

if [ "$REJECT_PASSED" -eq "$REJECT_TOTAL" ]; then
  printf "%-20s | ${GREEN}%-10s${OFF} | %-10s\n" "Reject" "$REJECT_PASSED" "$REJECT_TOTAL"
else
  printf "%-20s | ${RED}%-10s${OFF} | %-10s\n" "Reject" "$REJECT_PASSED" "$REJECT_TOTAL"
fi
printf "%-20s | %-10s | %-10s\n" "--------------------" "----------" "----------"
echo ""

echo "All done."
exit $STATUS
