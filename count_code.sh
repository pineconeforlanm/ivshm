#!/bin/bash

# --- Configuration ---
EXCLUDE_DIRS="build,install,.venv,.vscode,.idea"
REPORT_DIR="build"
REPORT_CSV="$REPORT_DIR/code_stats.csv"

echo "=========================================="
echo "   🚀 Starting Code Statistics (cloc)"
echo "=========================================="

# 1. Check if cloc is installed
if ! command -v cloc &> /dev/null; then
    echo "❌ Error: cloc is not installed."
    exit 1
fi

# 2. Ensure the output directory exists
mkdir -p "$REPORT_DIR"

echo "Scanning directory: $(pwd)"
echo "------------------------------------------"

# 3. Display human-readable table to terminal
cloc . --exclude-dir=$EXCLUDE_DIRS --quiet

# 4. Generate CSV silently for Excel/Spreadsheets
cloc . --exclude-dir=$EXCLUDE_DIRS --csv --quiet --out="$REPORT_CSV"

# 5. Check execution result
if [ -f "$REPORT_CSV" ]; then
    echo "------------------------------------------"
    echo " ✅ Statistics Completed!"
    echo " 📊 CSV Data saved to: $REPORT_CSV"
    echo " 💡 Tip: You can open the CSV directly with Excel or WPS."
    echo "=========================================="
else
    echo " ❌ An error occurred during statistics."
fi