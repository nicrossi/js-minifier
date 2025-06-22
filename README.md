[![✗](https://img.shields.io/badge/Release-v1.1.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

[![✗](https://github.com/agustin-golmar/Flex-Bison-Compiler/actions/workflows/pipeline.yaml/badge.svg?branch=production)](https://github.com/agustin-golmar/Flex-Bison-Compiler/actions/workflows/pipeline.yaml)

# A not-so-smart Javascript minifier

A Javascript minifier written in C, and leavering a Flex-Bison Compiler. \
While simple, this minifier will aim to reduce code size through a series of rules, progressing from basic textual
transformations to more advanced semantic optimizations.

* [Output file](#writing-output-to-a-file)
* [Integration-like test](#minification-when--expect-test)
* Coverage report log
  * [Frontend](src/main/c/frontend/syntactic-analysis/log.md)
  * [Backend](src/main/c/backend/log.md)

## Writing output to a file

| Name          | Default  | Description                                                                      |
|---------------|:--------:|----------------------------------------------------------------------------------|
| `OUTPUT_FILE` | `stdout` | Instead of writing to `stdout`, set a filename to write the output (Eg. `out.js`) |

## Minification *When / Expect* test
Using the [test-suite.sh](script/ubuntu/test-suite.sh). You can run integration-like tests to check if the minifier is working as expected.

**SYNOPSIS**\
```bash
  test-suite.sh [-savediff] [-dir PATH]
  test-suite.sh -h | --help
```

**DESCRIPTION**\
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

**DIRECTORY STRUCTURE**\
One level of directories is expected beneath the test root:

           test-root/
           ├── test-case-1/
           │   ├── program.when
           │   └── program.expect
           ├── test-case-2/
           │   ├── other_program.when
           │   └── other_program.expect
           └── …

# Flex-Bison-Compiler

A base compiler example, developed with Flex and Bison.

* [Environment](#environment)
* [CI/CD](#cicd)
* [Recommended Extensions](#recommended-extensions)
* Installation
  * [Docker](doc/readme/Docker.md)
  * [Microsoft Windows](doc/readme/Windows.md)
  * [Ubuntu](doc/readme/Ubuntu.md)

## Environment

Set the following environment variables to control and configure the behaviour of the application:

|Name|Default|Description|
|-|:-:|-|
|`LOG_IGNORED_LEXEMES`|`true`|When `true`, logs all of the ignored lexemes found with Flex at DEBUGGING level. To remove those logs from the console output set it to `false`.|
|`LOGGING_LEVEL`|`INFORMATION`|The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`.|

## CI/CD

To trigger an automatic integration on every push or PR (_Pull Request_), you must activate _GitHub Actions_ in the _Settings_ tab. Use the following configuration:

|Key|Value|
|-|-|
|Actions permissions|Allow all actions and reusable workflows|
|Artifact and log retention|`30 days`|
|Fork pull request workflows from outside collaborators|Require approval for all outside collaborators|
|Workflow permissions|Read repository contents and packages permissions|
|Allow GitHub Actions to create and approve pull requests|`false`|

After integration is done, change the badges of this `README.md` to point to the new repository.

## Recommended Extensions

* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
* [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)
