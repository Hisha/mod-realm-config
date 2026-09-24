#!/usr/bin/env bash
# Standalone test runner for mod-realm-config.
#
# Compiles the production translation unit against small AzerothCore API
# doubles (tests/api/*.h) and runs the feature/regression suite. Requires a
# C++17 compiler and no AzerothCore build. Deployment still requires validation
# of the migration and query strings on MySQL/MariaDB.
set -euo pipefail

CXX="${CXX:-g++}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TEST_DIR="$ROOT/tests"
BUILD_DIR="${TMPDIR:-/tmp}/mod-realm-config-tests-$$"

mkdir -p "$BUILD_DIR"
trap 'rm -rf "$BUILD_DIR"' EXIT

# Compile into a pristine temp directory so stale artifacts never mask failures.
"$CXX" -std=c++17 -Wall -Wextra -Werror \
    -I"$TEST_DIR/api" \
    "$TEST_DIR/mod_realm_config_tests.cpp" \
    -o "$BUILD_DIR/realm_config_tests"

"$BUILD_DIR/realm_config_tests"