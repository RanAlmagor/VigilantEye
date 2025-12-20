# VigilantEye 👁️‍🗨️
**Real-Time Vision Pipeline (C++17 + OpenCV)**

![Status](https://img.shields.io/badge/status-active%20development-success)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![Build](https://img.shields.io/badge/build-CMake-informational)
![Deps](https://img.shields.io/badge/deps-vcpkg%20manifest-informational)
![OpenCV](https://img.shields.io/badge/OpenCV-required-brightgreen)

A modular real-time computer-vision system that captures frames from interchangeable sources, runs **YOLO detection + lightweight tracking**, and renders a tactical **HUD overlay**.

---

## Screenshots

<img src="assets/screenshots/Shot1.jpeg" width="49%"/> <img src="assets/screenshots/Shot2.png" width="49%"/>

---

## Key Features

- **Pipeline-based design**: vision stages share a unified `IVisionAlgorithm` interface and are composed in `Pipeline`.
- **Explicit ownership (Modern C++)**: algorithms are stored as `std::unique_ptr` (clear lifetime, no leaks).
- **Multiple frame sources** via `IFrameSource` (webcam / video file) with a clean init/capture/stop contract.
- **YOLOv4-tiny detection (OpenCV DNN)** with confidence gating + NMS and structured `Detection` results.
- **Object tracking with stable IDs** (centroid matching + disappearance handling).
- **Motion detection** (frame-diff + contours) to highlight movement regions.
- **HUD overlay**: bracket-style boxes, per-target crosshair, and a semi-transparent status bar (objects + FPS).
- **Thread-safe logging**: singleton `Logger` with an internal `std::mutex` for safe multi-threaded console logs.
- **Real-time runtime loop**: processing/display separation, detection decimation, FPS smoothing, and frame skipping under load.

---

## Frame Sources (HAL / Sensors)

### `IFrameSource`
A small interface that keeps the vision pipeline independent of the input device:
- `initialize()`, `capture(cv::Mat&)`, `stop()`
- `getWidth() / getHeight() / getFPS() / getSourceName()`

### `WebcamSource`
- Wraps `cv::VideoCapture` with **lazy initialization** (camera opens in `initialize()`, not in the ctor).
- Requests target width/height/FPS via:
  - `CAP_PROP_FRAME_WIDTH`, `CAP_PROP_FRAME_HEIGHT`, `CAP_PROP_FPS`
- Reads back the **actual** values from the device and exposes them via getters.
- **Rule of Five** style: copy disabled, move enabled, and the destructor calls `stop()` for clean release.

### `VideoFileSource`
- Opens video files via `cv::VideoCapture` and exposes a stable source name (`VideoFile_<path>`).
- Optional looping mode:
  - On end-of-stream, rewinds to the start using `CAP_PROP_POS_FRAMES` (fallback: `CAP_PROP_POS_MSEC`) and continues.
- FPS handling:
  - Reads `CAP_PROP_FPS` when available, otherwise uses a fallback (target FPS or 30).
- **Rule of Five** style: copy disabled, move enabled, and the destructor calls `stop()`.

---

## Vision Modules (Implementation Details)

### `IVisionAlgorithm`
A minimal interface for pluggable stages:
- `detect(const cv::Mat& input, cv::Mat& output)`

### `DNNObjectDetector` (YOLOv4-tiny)
- Loads Darknet cfg + weights via `readNetFromDarknet(...)`
- Preprocess:
  - `blobFromImage(..., 1/255.0, Size(416, 416), ..., swapRB=true)`
- Inference:
  - `net.forward(outs, net.getUnconnectedOutLayersNames())`
- Postprocess:
  - Keeps detections with **confidence > 0.65**
  - Applies `NMSBoxes(..., scoreThreshold=0.5, nmsThreshold=0.4)`
- Stores results as:
  - `std::vector<Detection> { classId, confidence, box, label }`
  - Accessible via `getLatestDetections()`

### `MotionDetector`
A fast movement highlighter:
- Converts to grayscale + `GaussianBlur(21x21)`
- `absdiff(prev, current)` → `threshold(delta, 25)`
- `dilate` with `15x15` kernel (2 iterations)
- `findContours` and filters small areas (**area < 1000** ignored)
- Draws bounding boxes + `"MOTION"` label on the output frame

### `ObjectTracker`
Lightweight ID tracking without heavy dependencies:
- Tracks `TrackedObject { id, bbox, center, disappearedFrames }`
- Matching:
  - Greedy nearest-centroid match using `cv::norm(...)`
  - Match accepted only if distance < **MAX_DISTANCE (50.0)**
- Disappearance:
  - Removed after **MAX_DISAPPEARED (30)** frames
- Rendering:
  - `"ID: <num>"` + centroid dot + bbox (active vs temporarily missing)

---

## UI Overlay (HUD)

- **Resolution-aware UI scaling**: a scale factor derived from `frame.cols / 1280.0` (clamped with a minimum) keeps the HUD readable across resolutions.
- **Bracket-style boxes**: instead of full rectangles, the HUD draws “corner brackets” (cleaner, tactical look). Line thickness scales with resolution.
- **Per-target crosshair**: drawn at the center of each detection (4 short lines + a small filled dot).
- **Confidence-based color**:
  - Normal targets use the standard HUD color.
  - High-confidence targets (**> 0.75**) switch to an alert color.
- **Readable labels**: `"label XX%"` is rendered near the box with a dark background rectangle for contrast.
- **Status bar**: a semi-transparent black strip at the bottom shows:
  - `ARMED | TGT: <count> | FPS: <fps>`

---



## Utilities (GeneralUtils)

### `Logger`
- Implemented as a **singleton** (`Logger::getInstance()`), so the whole app logs through a single shared instance.
- `log(const std::string&)` provides a simple, centralized logging entry point.
- Copy/assignment are **deleted** to enforce single-instance semantics.
- Contains an internal `std::mutex` to support **thread-safe logging** when multiple threads are introduced.


## Core Runtime Notes (Real-Time Behavior)

- **Two-resolution strategy**: processing frame (DNN) and display frame (HUD) are created separately.
- **Detection decimation**: DNN runs every *N* frames for a big FPS win, while tracking/HUD updates every frame.
- **Scale-safe overlays**: detections are scaled from processing-space to display-space (so boxes stay aligned).
- **Smooth FPS**: exponential moving average (EMA) is used for stable FPS display.
- **Frame skipping**: when processing falls behind, extra frames can be skipped to keep the system responsive.
- **Clean shutdown**: `ESC` exits and resources are released.

---

## Prerequisites

- **Git** (recommended: with submodules)
- **CMake** (>= 3.10)
- **C++17 toolchain** (Windows recommended: Visual Studio 2022, x64)
- **vcpkg** (manifest mode)
  - Current dependency in `vcpkg.json`: `opencv`

---

## Build (Windows / Visual Studio + vcpkg preset)

```bash
git clone --recurse-submodules <YOUR_REPO_URL>
cd VigilantEye

cmake --preset vs-debug
cmake --build --preset build-vs-debug
```

---

## Run

- The VS preset uses the project root as the working directory (so relative paths work).
- The build also copies the runtime `config/` folder next to the executable for portable runs.

---

## Author
**Ran Almagor**
