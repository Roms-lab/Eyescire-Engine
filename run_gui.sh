#!/bin/bash
# EyescireEditor GUI launcher for Codespaces

# --- Kill old VNC server if running ---
echo "[1/5] Killing any old VNC servers..."
vncserver -kill :1 2>/dev/null || true

# --- Start VNC server ---
echo "[2/5] Starting new VNC server..."
vncserver :1 -geometry 1280x720 -depth 24

# --- Set DISPLAY for Qt ---
export DISPLAY=:1
export XDG_RUNTIME_DIR=/tmp/runtime-codespace

# --- Launch EyescireEditor ---
echo "[3/5] Launching EyescireEditor..."
# Run it in background so we can start noVNC too
./build/EyescireEditor &

# --- Start noVNC server ---
echo "[4/5] Starting noVNC on port 6080..."
cd ~/noVNC || { echo "noVNC not found at ~/noVNC"; exit 1; }
python3 -m websockify --web . 6080 localhost:5901 &

echo "[5/5] Done! Open the Codespaces forwarded URL for port 6080 in your browser to see the GUI."
echo "Example: https://<codespace-id>-6080.preview.app.github.dev"
