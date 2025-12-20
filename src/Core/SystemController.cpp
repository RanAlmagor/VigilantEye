#include "Core/SystemController.h"
#include "Core/FrameResizePolicy.h"
#include "GeneralUtils/Logger.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <string>

static const std::string WINDOW_NAME = "VigilantEye - Tactical Feed";

SystemController::SystemController(std::unique_ptr<IFrameSource> sensor)
    : m_sensor(std::move(sensor))
{
    cv::namedWindow(WINDOW_NAME, cv::WINDOW_NORMAL);
    cv::resizeWindow(WINDOW_NAME, 1280, 720);

    auto dnnDetector = std::make_unique<DNNObjectDetector>();
    Logger::getInstance().log("[System] Initializing AI...");

    if (dnnDetector->init(MODEL_PATH, CONFIG_PATH, CLASSES_PATH)) {
        Logger::getInstance().log("[System] AI Model loaded successfully.");
        m_detector = dnnDetector.get();
        m_pipeline.addAlgorithm(std::move(dnnDetector));
    }
    else {
        Logger::getInstance().log("[System] [WARNING] AI failed to load. Running in Camera-Only mode.");
        m_detector = nullptr;
    }
}

void SystemController::run()
{
    m_running = true;
    Logger::getInstance().log("[System] Running. Press ESC to stop.");

    if (!m_sensor->initialize()) {
        Logger::getInstance().log("[System] [ERROR] Sensor initialization failed!");
        return;
    }

     // =========================
    // Resize policy (Option 1)
   // =========================
    FrameResizePolicy policy;
  

    // --- Performance knobs ---
    const int DETECT_EVERY_N_FRAMES = 3; // תעלה ל-4/5 אם עוד איטי
    const double srcFps = std::max(1.0, m_sensor->getFPS());
    const double frameBudgetMs = 1000.0 / srcFps;

    double fpsEma = 0.0;
    const double alpha = 0.12;

    cv::Mat frameFull, frameProc, frameDisp, dummy, outputProc;
    size_t frameIdx = 0;

    while (m_running)
    {
        const int64 t0 = cv::getTickCount();

        // 1) Capture (FULL / native)
        if (!m_sensor->capture(frameFull)) {
            Logger::getInstance().log("[System] Stream ended or capture failed.");
            break;
        }
        if (frameFull.empty()) {
            continue;
        }

        // 2) Build proc + display frames
        policy.makeProc(frameFull, frameProc);
        policy.makeDisplay(frameFull, frameDisp);

        // 3) DNN less frequently (big FPS win)
        if (m_detector != nullptr && (frameIdx % DETECT_EVERY_N_FRAMES == 0)) {
            m_pipeline.process(frameProc, outputProc);
        }

        // 4) Update tracker + HUD (draw on DISPLAY)
        if (m_detector != nullptr)
        {
            const auto& detsSmall = m_detector->getLatestDetections();

            // scale from proc -> display
            auto [sx, sy] = FrameResizePolicy::scaleFactors(frameProc, frameDisp);

            std::vector<Detection> detsDisp;
            detsDisp.reserve(detsSmall.size());

            std::vector<cv::Rect> trackerInput;
            trackerInput.reserve(detsSmall.size());

            for (const auto& d : detsSmall) {
                Detection dd = d;
                dd.box = FrameResizePolicy::scaleRect(d.box, sx, sy);
                detsDisp.push_back(dd);
                trackerInput.push_back(dd.box);
            }

            m_tracker.update(trackerInput);

            // FPS calc
            const int64 t1 = cv::getTickCount();
            const double procMs = (t1 - t0) * 1000.0 / cv::getTickFrequency();
            const double instFps = 1000.0 / std::max(1.0, procMs);
            fpsEma = (fpsEma == 0.0) ? instFps : (1.0 - alpha) * fpsEma + alpha * instFps;

            m_hud.update(frameDisp, detsDisp, fpsEma);

            // If late: skip frames (video feels real-time)
            if (procMs > frameBudgetMs * 1.5) {
                int skip = (int)((procMs - frameBudgetMs) / frameBudgetMs);
                skip = std::clamp(skip, 0, 5);

                for (int i = 0; i < skip; ++i) {
                    if (!m_sensor->capture(dummy)) { m_running = false; break; }
                }
            }
        }
        else
        {
            const int64 t1 = cv::getTickCount();
            const double procMs = (t1 - t0) * 1000.0 / cv::getTickFrequency();
            const double instFps = 1000.0 / std::max(1.0, procMs);
            fpsEma = (fpsEma == 0.0) ? instFps : (1.0 - alpha) * fpsEma + alpha * instFps;

            m_hud.update(frameDisp, {}, fpsEma);
        }

        // 5) Display
        cv::imshow(WINDOW_NAME, frameDisp);

        // 6) Delay based on source FPS
        const int64 t2 = cv::getTickCount();
        const double totalMs = (t2 - t0) * 1000.0 / cv::getTickFrequency();
        int delay = (int)std::round(frameBudgetMs - totalMs);
        if (delay < 1) delay = 1;

        int key = cv::waitKey(delay);
        if (key == 27) m_running = false;

        ++frameIdx;
    }

    m_sensor->stop();
    cv::destroyAllWindows();
    Logger::getInstance().log("[System] System shutdown complete.");
}
