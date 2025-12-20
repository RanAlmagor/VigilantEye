# VigilantEye 👁️‍🗨️  
**Real-Time Vision Pipeline (C++17 + OpenCV)**  
![Status](https://img.shields.io/badge/status-active%20development-success)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![Build](https://img.shields.io/badge/build-CMake-informational)
![Deps](https://img.shields.io/badge/deps-vcpkg%20manifest-informational)
![OpenCV](https://img.shields.io/badge/OpenCV-required-brightgreen)

A modular, real-time computer-vision pipeline that captures frames from interchangeable sources, runs detection/tracking, and renders a tactical HUD overlay.

---

## Screenshots

<img src="assets/screenshots/Shot1.jpeg" width="49%"/> <img src="assets/screenshots/Shot2.png" width="49%"/>

---

## What’s inside

### Core
- **SystemController** orchestrates the run loop, connects the sensor → pipeline → tracker → HUD.
- **Pipeline** composes vision stages via `std::vector<std::unique_ptr<IVisionAlgorithm>>` for clean ownership and extensibility.

### HAL / Sensors
- **IFrameSource** interface (`initialize`, `capture`, `stop`, `getFPS`, `getWidth/Height`, `getSourceName`).
- **WebcamSource** and **VideoFileSource** implementations (copy disabled, move enabled; `stop()` called on destruction).

### Vision
- **IVisionAlgorithm** interface (`detect(const cv::Mat&, cv::Mat&)`).
- **DNNObjectDetector** (OpenCV DNN, YOLOv4-tiny) producing `Detection { classId, confidence, box, label }`.
- **MotionDetector** (frame-diff style motion detection; keeps previous frame).
- **ObjectTracker** (lightweight ID-based tracking using bbox/centroid + disappearance counter).

### UI
- **HUD** overlays detection brackets, crosshair, and status bar (object count + FPS).

### Utilities
- **FrameResizePolicy** helpers for processing/display sizing and scaling rectangles between spaces.
- **Logger** thread-safe singleton for structured console logging.

---

## Build (Windows / Visual Studio + vcpkg preset)

```bash
git clone --recurse-submodules <YOUR_REPO_URL>
cd VigilantEye

cmake --preset vs-debug
cmake --build --preset build-vs-debug
```

---

## Notes on Modern C++ usage
- Interface-driven design (`IFrameSource`, `IVisionAlgorithm`) to keep modules decoupled.
- Ownership is explicit with **smart pointers** (`std::unique_ptr`) in the pipeline and controller layer.
- Several components are **move-enabled** and **copy-disabled** (Rule-of-Five style) to avoid accidental expensive copies.

---

## Author
Ran Almagor
