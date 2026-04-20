#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
PROGRAMS_DIR="$SCRIPT_DIR/programs"

TREE_WALKING="$PROJECT_ROOT/out/RelWithDebInfo/project/interpreters/tree-walking-interpreter/repl/tree_walking_interpreter_repl"
BYTECODE_VM="$PROJECT_ROOT/out/RelWithDebInfo/project/interpreters/bytecode-based-vm-interpreter/repl/bytecode_vm_interpreter_repl"

RED='\033[0;31m'; YEL='\033[1;33m'; GRN='\033[0;32m'; CYN='\033[1;36m'; BLD='\033[1m'; NC='\033[0m'

dsep() { echo "════════════════════════════════════════════════════════════════"; }
ssep() { echo "────────────────────────────────────────────────────────────────"; }

echo -e "${BLD}${CYN}"
dsep
echo "  Skrif Interpreter Benchmark vs V8 (Node.js)"
dsep
echo -e "${NC}"

# ── Check requirements ──────────────────────────────────────────────────────

if ! command -v node &>/dev/null; then
    echo -e "${RED}ERROR:${NC} node not found. Install Node.js to run this benchmark."
    exit 1
fi
echo -e "  Node.js          : ${GRN}$(node --version)${NC}"

HAVE_TW=false
HAVE_BVM=false

if [ -f "$TREE_WALKING" ]; then
    HAVE_TW=true
    echo -e "  Tree-Walking     : ${GRN}found${NC}"
else
    echo -e "  Tree-Walking     : ${YEL}NOT FOUND${NC} — build the project first"
    echo    "    cmake -B \"$PROJECT_ROOT/out/Debug\" \"$PROJECT_ROOT\""
    echo    "    cmake --build \"$PROJECT_ROOT/out/Debug\""
fi

if [ -f "$BYTECODE_VM" ]; then
    HAVE_BVM=true
    echo -e "  Bytecode VM      : ${GRN}found${NC}"
else
    echo -e "  Bytecode VM      : ${YEL}NOT FOUND${NC} — build the project first"
fi

if ! $HAVE_TW && ! $HAVE_BVM; then
    echo ""
    echo -e "${RED}No custom interpreters available. Aborting.${NC}"
    exit 1
fi
echo ""

# ── Timing helpers ──────────────────────────────────────────────────────────

# Measure elapsed milliseconds for a command; stdout+stderr discarded
time_ms() {
    local t0 t1
    t0=$(date +%s%N)
    "$@" >/dev/null 2>&1 || true
    t1=$(date +%s%N)
    echo $(( (t1 - t0) / 1000000 ))
}

# Run Node.js with a `var print = console.log;` shim prepended to the file
time_v8() {
    local file="$1" tmp
    tmp=$(mktemp /tmp/skrif_bench_XXXXXX.js)
    { printf 'var print = console.log;\n'; cat "$file"; } >"$tmp"
    local t0 t1
    t0=$(date +%s%N)
    node "$tmp" >/dev/null 2>&1 || true
    t1=$(date +%s%N)
    rm -f "$tmp"
    echo $(( (t1 - t0) / 1000000 ))
}

# Format milliseconds into a human-readable fixed-width string
fmt() {
    awk -v ms="$1" 'BEGIN {
        if (ms < 1000)       printf "%7d ms", ms
        else if (ms < 60000) printf "%7.2f s ", ms/1000
        else                 printf "%7.1f s ", ms/1000
    }'
}

# Compute ratio (val/base) with one decimal place
ratio() {
    awk -v val="$1" -v base="$2" 'BEGIN {
        if (base == 0) print "N/A"
        else           printf "%.1f", val/base
    }'
}

# Print a color-coded "NNNx" ratio: green < 10x, yellow 10-99x, red ≥ 100x
cratio() {
    awk -v r="$1" -v RED='\033[0;31m' -v YEL='\033[1;33m' \
                  -v GRN='\033[0;32m' -v NC='\033[0m' 'BEGIN {
        n = int(r)
        if (n < 10)       printf GRN "%6sx" NC, r
        else if (n < 100) printf YEL "%6sx" NC, r
        else              printf RED "%6sx" NC, r
    }'
}

# ── Run benchmarks ──────────────────────────────────────────────────────────

mapfile -t PROGRAMS < <(find "$PROGRAMS_DIR" -name "*.js" -type f | sort)

if (( ${#PROGRAMS[@]} == 0 )); then
    echo -e "${RED}ERROR:${NC} No *.js files found in $PROGRAMS_DIR"
    exit 1
fi

declare -A T_V8 T_BVM T_TW

for prog in "${PROGRAMS[@]}"; do
    name=$(basename "$prog")
    dsep
    echo -e "${BLD}  $name${NC}"
    ssep

    printf "  %-28s" "V8 (Node.js)"
    v8_ms=$(time_v8 "$prog")
    T_V8["$name"]=$v8_ms
    printf "%s   %6s  baseline\n" "$(fmt "$v8_ms")" "1.0x"

    if $HAVE_BVM; then
        printf "  %-28s" "Bytecode VM Interpreter"
        ms=$(time_ms "$BYTECODE_VM" "$prog")
        T_BVM["$name"]=$ms
        r=$(ratio "$ms" "$v8_ms")
        printf "%s   " "$(fmt "$ms")"
        cratio "$r"
        printf "  slower than V8\n"
    fi

    if $HAVE_TW; then
        printf "  %-28s" "Tree-Walking Interpreter"
        ms=$(time_ms "$TREE_WALKING" "$prog")
        T_TW["$name"]=$ms
        r=$(ratio "$ms" "$v8_ms")
        printf "%s   " "$(fmt "$ms")"
        cratio "$r"
        printf "  slower than V8\n"
    fi
    echo ""
done

# ── Summary table ────────────────────────────────────────────────────────────

dsep
echo -e "${BLD}  SUMMARY  —  slowdown factor relative to V8 (higher = slower)${NC}"
dsep

printf "${BLD}  %-24s  %10s" "Benchmark" "V8"
$HAVE_BVM && printf "  %13s" "Bytecode VM"
$HAVE_TW  && printf "  %15s" "Tree-Walking"
printf "${NC}\n"
ssep

for prog in "${PROGRAMS[@]}"; do
    name=$(basename "$prog")
    printf "  %-24s  %s" "$name" "$(fmt "${T_V8[$name]}")"

    if $HAVE_BVM; then
        r=$(ratio "${T_BVM[$name]}" "${T_V8[$name]}")
        printf "     "
        cratio "$r"
        printf "       "
    fi

    if $HAVE_TW; then
        r=$(ratio "${T_TW[$name]}" "${T_V8[$name]}")
        printf "   "
        cratio "$r"
    fi
    printf "\n"
done

dsep
echo -e "${BLD}${CYN}  Interpretation:${NC}"
echo    "  Green  < 10x  — competitive"
echo    "  Yellow 10-99x — moderate overhead"
echo -e "  ${RED}Red   ≥ 100x  — significant overhead${NC}"
dsep
