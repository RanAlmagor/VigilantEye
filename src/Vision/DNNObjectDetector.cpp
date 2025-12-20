#include "Vision/DNNObjectDetector.h"
#include "GeneralUtils/Logger.h" 
#include <fstream> 

using namespace cv;
using namespace cv::dnn;

bool DNNObjectDetector::init(const std::string& modelPath, const std::string& configPath, const std::string& classesPath)
{
    
    std::ifstream ifs(classesPath.c_str());
    if (!ifs.is_open()) {
        Logger::getInstance().log("[DNN] [ERROR] File " + classesPath + " not found!");
        return false;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        m_classNames.push_back(line);
    }

    if (m_classNames.empty()) {
        Logger::getInstance().log("[DNN] [ERROR] No classes loaded from " + classesPath);
        return false;
    }

   
    Logger::getInstance().log("[DNN] Loading model...");
    try {
        m_net = readNetFromDarknet(configPath, modelPath);
    }
    catch (const cv::Exception& e) {
        Logger::getInstance().log(std::string("[DNN] [EXCEPTION] ") + e.what());
        return false;
    }

    if (m_net.empty()) {
        Logger::getInstance().log("[DNN] [ERROR] Failed to load network.");
        return false;
    }

 
    m_net.setPreferableBackend(DNN_BACKEND_OPENCV);
    m_net.setPreferableTarget(DNN_TARGET_CPU);

    Logger::getInstance().log("[DNN] Model loaded successfully.");
    return true;
}

void DNNObjectDetector::detect(const cv::Mat& input, cv::Mat& output)
{
    if (m_net.empty()) return;

    m_latestDetections.clear();

    if (output.empty()) {
        input.copyTo(output);
    }


    Mat blob;
    blobFromImage(input, blob, 1 / 255.0, Size(416, 416), Scalar(0, 0, 0), true, false);
    m_net.setInput(blob);

   
    std::vector<Mat> outs;
    m_net.forward(outs, m_net.getUnconnectedOutLayersNames());

   
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<Rect> boxes;

    for (const auto& out : outs) {
        float* data = (float*)out.data;
        for (int j = 0; j < out.rows; ++j, data += out.cols) {
            Mat scores = out.row(j).colRange(5, out.cols);
            Point classIdPoint;
            double confidence;
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            if (confidence > 0.65) { 
                int centerX = (int)(data[0] * input.cols);
                int centerY = (int)(data[1] * input.rows);
                int width = (int)(data[2] * input.cols);
                int height = (int)(data[3] * input.rows);

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(centerX - width / 2, centerY - height / 2, width, height));
            }
        }
    }

    std::vector<int> indices;
    NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

    for (int idx : indices)
    {
        Detection det;
        det.classId = classIds[idx];
        det.confidence = confidences[idx];
        det.box = boxes[idx];

        if (classIds[idx] < m_classNames.size()) {
            det.label = m_classNames[classIds[idx]];
        }
        else {
            det.label = "Unknown";
        }

        m_latestDetections.push_back(det);
    }
}