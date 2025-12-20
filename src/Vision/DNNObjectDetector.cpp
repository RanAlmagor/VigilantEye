#include "Vision/DNNObjectDetector.h"
#include <iostream>
#include <fstream> 

using namespace cv;
using namespace cv::dnn;
using namespace std;


bool DNNObjectDetector::init(const std::string& modelPath, const std::string& configPath, const std::string& classesPath)
{
  
    ifstream ifs(classesPath.c_str());
    if (!ifs.is_open()) {
        cerr << "[DNN] Error: File " << classesPath << " not found!" << endl;
        return false;
    }

    string line;
    while (getline(ifs, line)) {
        m_classNames.push_back(line);
    }

    if (m_classNames.empty()) {
        cerr << "[DNN] Error: No classes loaded from " << classesPath << endl;
        return false;
    }

   
    cout << "[DNN] Loading model..." << endl;
    try {
        m_net = readNetFromDarknet(configPath, modelPath);
    }
    catch (const cv::Exception& e) {
        cerr << "[DNN] Exception: " << e.what() << endl;
        return false;
    }

    if (m_net.empty()) return false;

    // הגדרות האצה (CPU)
    m_net.setPreferableBackend(DNN_BACKEND_OPENCV);
    m_net.setPreferableTarget(DNN_TARGET_CPU);
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

  
    vector<Mat> outs;
    m_net.forward(outs, m_net.getUnconnectedOutLayersNames());

  
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

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

 
    vector<int> indices;
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