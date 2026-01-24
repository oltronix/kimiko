#!/usr/bin/env bash
set -euo pipefail

# Configuration - override via environment variables
KEYBOARD="${KEYBOARD:-keycapsss/kimiko/rev1}"
KEYMAP="${KEYMAP:-kimiko}"
CONVERT_TO="${CONVERT_TO:-helios}"
QMK_FIRMWARE="${QMK_FIRMWARE:-../qmk_firmware}"

# Resolve script directory for reliable paths
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
OUTPUT_DIR="${SCRIPT_DIR}/output"

echo "=== QMK Kimiko Build ==="
echo "Keyboard: ${KEYBOARD}"
echo "Keymap: ${KEYMAP}"
echo "Convert to: ${CONVERT_TO}"

# Validate required files exist
if [[ ! -f "${SCRIPT_DIR}/keymap.json" ]]; then
    echo "Error: keymap.json not found" >&2
    exit 1
fi

if [[ ! -f "${SCRIPT_DIR}/custom.c" ]]; then
    echo "Error: custom.c not found" >&2
    exit 1
fi

# Create build directories
mkdir -p "${BUILD_DIR}"
mkdir -p "${OUTPUT_DIR}"

# Step 1: Convert JSON keymap to C
echo "Converting keymap.json to C..."
qmk json2c "${SCRIPT_DIR}/keymap.json" -o "${BUILD_DIR}/keymap.c"

# Step 2: Extract keymap array from generated code
echo "Extracting keymap array..."
BREAK_LINE=$(grep -n "const uint16_t PROGMEM keymaps" "${BUILD_DIR}/keymap.c" | cut -f1 -d:)

if [[ -z "${BREAK_LINE}" ]]; then
    echo "Error: Could not find keymaps definition in generated code" >&2
    exit 1
fi

BREAK_LINE=$((BREAK_LINE - 1))
tail -n +"${BREAK_LINE}" "${BUILD_DIR}/keymap.c" > "${BUILD_DIR}/keys.c"

# Step 3: Combine custom code with keymap
echo "Building final keymap.c..."
mkdir -p "${BUILD_DIR}/pub"
cp -R "${SCRIPT_DIR}/src/." "${BUILD_DIR}/pub/"
cat "${SCRIPT_DIR}/custom.c" "${BUILD_DIR}/keys.c" > "${BUILD_DIR}/pub/keymap.c"

# Step 4: Copy to QMK firmware directory
DEST="${QMK_FIRMWARE}/keyboards/${KEYBOARD%%/*}/keymaps/${KEYMAP}"
echo "Copying to ${DEST}..."
mkdir -p "${DEST}"
cp -R "${BUILD_DIR}/pub/." "${DEST}/"

# Step 5: Compile
echo "Compiling firmware..."
cd "${QMK_FIRMWARE}"
qmk compile -e CONVERT_TO="${CONVERT_TO}" -km "${KEYMAP}" -kb "${KEYBOARD}" -c

# Step 6: Copy output firmware
echo "Copying firmware to output directory..."
cd "${SCRIPT_DIR}"
find "${QMK_FIRMWARE}" -maxdepth 1 -name "*.uf2" -exec cp {} "${OUTPUT_DIR}/" \;
find "${QMK_FIRMWARE}" -maxdepth 1 -name "*.hex" -exec cp {} "${OUTPUT_DIR}/" \;

# List outputs
echo "=== Build complete ==="
echo "Firmware files:"
ls -la "${OUTPUT_DIR}"/*.uf2 "${OUTPUT_DIR}"/*.hex 2>/dev/null || echo "No firmware files found"
