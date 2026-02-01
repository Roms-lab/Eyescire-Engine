#!/bin/bash
set -e

# --- 1. Install MinGW if missing ---
if ! command -v x86_64-w64-mingw32-g++ &>/dev/null; then
    echo "[1/6] Installing mingw-w64..."
    sudo apt update
    sudo apt install -y mingw-w64
fi

# --- 2. Download prebuilt Qt6 for Windows MinGW ---
QT_DIR="$HOME/Qt6_win_mingw"
if [ ! -d "$QT_DIR" ]; then
    echo "[2/6] Downloading prebuilt Qt6 (Windows MinGW)..."
    mkdir -p "$QT_DIR"
    wget -O /tmp/qt6_mingw.zip "https://download.qt.io/official_releases/qt/6.4/6.4.2/qt-opensource-windows-x86_64-mingw820-6.4.2.zip"
    echo "[2/6] Extracting..."
    unzip /tmp/qt6_mingw.zip -d "$QT_DIR"
fi

# Find extracted folder
QT_PREFIX=$(find "$QT_DIR" -maxdepth 1 -type d -name "Qt*" | head -n1)

# --- 3. Create MinGW toolchain file ---
TOOLCHAIN_FILE="$PWD/mingw-toolchain.cmake"
echo "[3/6] Writing MinGW toolchain file..."
cat > "$TOOLCHAIN_FILE" <<EOL
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
EOL

# --- 4. Create Windows build folder ---
BUILD_DIR="$PWD/build_win"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# --- 5. Configure CMake for Windows ---
echo "[4/6] Configuring CMake for Windows..."
cmake -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" -DCMAKE_PREFIX_PATH="$QT_PREFIX/mingw_64" ..

# --- 6. Build the project ---
echo "[5/6] Building EyescireEditor.exe..."
cmake --build . --config Release

echo "[6/6] Done! Windows build should be in $BUILD_DIR"
echo "Copy the .exe to your Windows PC and run it."
