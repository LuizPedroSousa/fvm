#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="buildtools"
PYTHON_SCRIPT="$SCRIPT_DIR/build.py"

echo "=== Project Generator ==="
echo "Available presets (from CMakePresets.json):"

cmake --list-presets | grep -v "Available" || true

if [ $# -eq 0 ]; then
	echo ""
	read -rp "Enter preset name (or number if shown above): " PRESET
else
	PRESET="$1"
fi

if [ -z "$PRESET" ]; then
	echo "Error: No preset selected."
	exit 1
fi

echo ""
echo "Generating build using preset: $PRESET"
echo ""

python3 "$PYTHON_SCRIPT" "$PRESET"
