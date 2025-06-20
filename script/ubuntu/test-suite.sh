#!/usr/bin/env bash

# Automatic test-suite for build/Compiler.
# Each test case lives in its own directory containing:
#   - exactly one  *.when   (input)
#   - exactly one  *.expect (expected output)
# -----------------------------------------------------------------------------
set -uo pipefail
IFS=$'\n\t'; OFF=$'\033[0m'
GREEN=$'\033[0;32m';  RED=$'\033[0;31m';
YELLOW=$'\033[1;33m'; CYAN=$'\033[0;36m'
# portable color diff ---------------------------------------------------------
colour_diff() {

    awk '
        BEGIN {
            RED  = "\033[0;31m"; GREEN = "\033[0;32m";
            YEL  = "\033[1;33m"; CYA   = "\033[36m";
            OFF  = "\033[0m";
        }
        /^@@/          {print YEL  $0 OFF;  next}   # hunk header
        /^--- /        {print CYA  $0 OFF;  next}   # file-A header
        /^\+\+\+ /     {print CYA  $0 OFF;  next}   # file-B header
        /^-/  && !/^--- /      {print RED  $0 OFF;  next}   # deletion
        /^\+/ && !/^\+\+\+/    {print GREEN $0 OFF; next}   # addition
        {print}
    '
}
# ---- usage -------------------------------------------------------------------
usage() {
    cat <<EOF
NAME
     test-suite.sh – integration test suite for Flex-Bison-Compiler

SYNOPSIS
     test-suite.sh [-savediff] [-dir PATH]
     test-suite.sh -h | --help

DESCRIPTION
     test-suite.sh executes build/Compiler on every test case found in a
     directory tree (the “test root”) and checks that its output matches
     the expected results stored alongside each test.

     A test case is a directory that contains exactly two files sharing the
     same basename:

           <name>.when     input text that is piped to build/Compiler
           <name>.expect   the reference (expected) output

     By default the script looks for test directories under

           src/test/c/integration

     but a different location can be selected with the -dir option.

     After running all tests, the script prints a summary showing how
     many passed, failed, or were skipped, and exits with a status
     equal to the number of failures (0 means all tests passed).

DIRECTORY STRUCTURE
     One level of directories is expected beneath the test root:

           test-root/
           ├── test-case-1/
           │   ├── program.when
           │   └── program.expect
           ├── test-case-2/
           │   ├── other_program.when
           │   └── other_program.expect
           └── …

OPTIONS
     -dir PATH
             Path to the directory that contains the test folders.
             May be absolute or relative.  If omitted, the default is
             src/test/c/integration (relative to the repository root).

     -savediff
             When a test fails, write the unified diff between actual and
             expected output to  <basename>.diff  inside the test directory
             instead of displaying the colourised diff on standard output.

     -h, --help
             Display a short usage message and exit.

EXIT STATUS
     0       All tests passed.
     >0      The number of tests that failed.
     -2      Command-line usage error (unknown flag, missing argument, or
             the test root cannot be accessed).

EXAMPLES
     Run the default integration suite and show coloured diffs:

           ./test-suite.sh

     Run a different suite and store the diffs for later inspection:

           ./test-suite.sh -dir path/to/cases -savediff

     Show help:

           ./test-suite.sh --help

EOF
}
# ----- configuration ---------------------------------------------------------
BASE_PATH="$(cd "$(dirname "$0")/../.." && pwd)"
COMPILER="$BASE_PATH/build/Compiler"
TEST_ROOT="$BASE_PATH/src/test/c/integration"
# ---- argument parsing -------------------------------------------------------
SAVE_DIFF_TO_FILE=false
while [[ $# -gt 0 ]]; do
    case $1 in
        -savediff) SAVE_DIFF_TO_FILE=true; shift ;;
        -dir)
            if [[ $# -lt 2 ]]; then
                echo -e "${RED}[ERROR]${OFF} -dir requires a path." >&2
                echo; usage; exit -2
            fi
            TEST_ROOT=$2; shift 2 ;;
        -h|--help) usage; exit 0 ;;
        *) echo -e "${RED}[ERROR]${OFF} unknown option '$1'" >&2
           echo; usage; exit -2 ;;
    esac
done
TEST_ROOT="$(cd "$TEST_ROOT" 2>/dev/null && pwd)" || {
    echo -e "${RED}[ERROR]${OFF} Unable to access the test root directory '$TEST_ROOT'."; exit 2; }
# -----------------------------------------------------------------------------
total=0 pass=0 fail=0 skipped=0
failed_tests=()
# allow “for dir in $TEST_ROOT/*/” to yield empty list without error
shopt -s nullglob
echo "Running test-suite in: $TEST_ROOT"
for dir in "$TEST_ROOT"/*/; do
    (( total++ ))
    name=$(basename "$dir")
    when_file=("$dir"/*.when)
    expect_file=("$dir"/*.expect)

    if (( ${#when_file[@]} != 1 || ${#expect_file[@]} != 1 )) ||
       [[ $(basename "${when_file[0]}" .when) != $(basename "${expect_file[0]}" .expect) ]]; then
        echo -e "${YELLOW}Skipped '$name': .when and .expect filenames differ or missing${OFF}"
        (( skipped++ ))
        continue
    fi

    base=$(basename "${when_file[0]}" .when)
    output="$(LOGGING_LEVEL=ERROR LOG_IGNORED_LEXEMES=false "$COMPILER" < "${when_file[0]}" 2>&1)"
    diff_output=$(diff -u --label "${expect_file[0]}" \
                  "${expect_file[0]}" <(printf '%s\n' "$output") || true)

    if [[ -z $diff_output ]]; then
        echo -e "  $name … ${GREEN}PASS${OFF}"
        ((pass++))
    else
        echo -e "  $name … ${RED}FAIL${OFF}"
        ((fail++)); failed_tests+=("$name")

        if $SAVE_DIFF_TO_FILE; then
            out_file="$dir/${base}.diff"
            printf '%s\n' "$diff_output" > "$out_file"
            echo "      diff saved to ${out_file#$BASE_PATH/}"
        else
            printf '%s\n' "$diff_output" | colour_diff | sed 's/^/      /'
        fi
    fi
done
echo
printf "%0.s-" {1..49}; echo
printf "%0.s " {1..18}; echo "Test Results"
printf "%0.s-" {1..49}; echo
printf "%-10s | %-10s | %-10s | %-10s\n" "Passed" "Failed" "Skipped" "Total"
printf "%-10s | %-10s | %-10s | %-10s\n" "----------" "----------" "----------" "----------"
printf "${GREEN}%-10d${OFF} | ${RED}%-10d${OFF} | ${YELLOW}%-10d${OFF} | %-10d\n" "$pass" "$fail" "$skipped" "$total"
printf "%0.s-" {1..49}; echo

if (( fail )); then
    echo -e "\nFailed tests:"
    for test in "${failed_tests[@]}"; do
        printf "  • %s\n" "$test"
    done
fi

exit $fail # number of failures
