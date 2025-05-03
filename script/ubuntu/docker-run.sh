#! /bin/bash

set -euxo pipefail

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

# @see https://docs.docker.com/reference/cli/docker/container/run/
docker run \
	--hostname docker \
	--interactive \
	--rm \
	--user root \
	--tty \
	--name flex-bison-compiler \
	--volume $(pwd):/home/ubuntu/Flex-Bison-Compiler \
	--workdir=/home/ubuntu/Flex-Bison-Compiler \
	--env TEST=/home/ubuntu/Flex-Bison-Compiler/src/test/c \
	--env LOG_IGNORED_LEXEMES=true \
	--env LOGGING_LEVEL=ALL \
	flex-bison-compiler:latest

echo "All done."
