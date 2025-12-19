#include "Core/SystemController.h"

#include "HAL/Sensors/WebcamSource.h"
#include "HAL/Sensors/VideoFileSource.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <chrono>
#include <iostream>
#include <utility>

using Clock = std::chrono::steady_clock;

SystemController::SystemController() = default;

void SystemController::setWebcamSource(int deviceId)
{
    switchSource(std::make_unique<WebcamSource>(deviceId));
}

void SystemController::setVideoFileSource(std::string videoPath)
{
    switchSource(std::make_unique<VideoFileSource>(std::move(videoPath)));
}

bool SystemController::switchSource(std::unique_ptr<IFrameSource> newSource)
{
    if (!newSource)
        return false;

    // Stop current source (if any)
    if (m_source)
        m_source->stop();

    m_source = std::move(newSource);

    if (!m_source->initialize())
    {
        std::cerr << "[Core] Failed to initialize source: " << m_source->getSourceName() << "\n";
        return false;
    }

    std::cout << "[Core] Active Source: " << m_source->getSourceName() << "\n";
    return true;
}

int SystemController::run()
{
    // Default source if none chosen
    if (!m_source)
        setWebcamSource(0);

    cv::namedWindow("VigilantEye", cv::WINDOW_NORMAL);

    cv::Mat frame;
    cv::Mat out;

    auto fpsTick = Clock::now();
    int framesCount = 0;

    while (true)
    {
        const auto t0 = Clock::now();

        // 1) Capture
        if (!m_source || !m_source->capture(frame))
        {
            std::cerr << "[Core] Capture failed or stream ended.\n";
            break;
        }

        // 2) Process
        m_pipeline.process(frame, out);

        const auto t1 = Clock::now();
        m_lastFrameMs = std::chrono::duration<double, std::milli>(t1 - t0).count();
        m_frameIndex++;

        // 3) FPS calc (update ~once/sec)
        framesCount++;
        auto now = Clock::now();
        double sec = std::chrono::duration<double>(now - fpsTick).count();
        if (sec >= 1.0)
        {
            m_fps = framesCount / sec;
            framesCount = 0;
            fpsTick = now;
        }

        // 4) HUD overlay
        HudData hd;
        hd.sourceName = m_source ? m_source->getSourceName() : "None";
        hd.frameWidth = out.cols;
        hd.frameHeight = out.rows;
        hd.fps = m_fps;
        hd.frameMs = m_lastFrameMs;
        hd.frameIndex = m_frameIndex;

        m_hud.draw(out, hd);

        // 5) Show
        cv::imshow("VigilantEye", out);

        // 6) Keys
        int key = cv::waitKey(1);
        if (key == 27) // ESC
            break;

        // Demo: press '1' to switch to webcam
        if (key == '1')
            setWebcamSource(0);

        // NOTE: for video switching by key, you'd store a path member and call setVideoFileSource(path)
    }

    // Cleanup
    if (m_source)
        m_source->stop();

    cv::destroyAllWindows();
    return 0;
}
