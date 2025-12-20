#include "Vision/ObjectTracker.h"
#include <cmath>
#include <limits>

namespace Vision {

    ObjectTracker::ObjectTracker() : m_nextObjectID(0) {}

    void ObjectTracker::registerObject(const cv::Rect& bbox) {
        cv::Point center(bbox.x + bbox.width / 2, bbox.y + bbox.height / 2);

        TrackedObject newObj;
        newObj.id = m_nextObjectID;
        newObj.bbox = bbox;
        newObj.center = center;
        newObj.disappearedFrames = 0;

        m_objects[m_nextObjectID] = newObj;
        m_nextObjectID++;
    }

    void ObjectTracker::deregisterObject(int objectId) {
        m_objects.erase(objectId);
    }

    void ObjectTracker::update(const std::vector<cv::Rect>& detections) {
      
        if (detections.empty()) {
           
            for (auto& pair : m_objects) {
                pair.second.disappearedFrames++;
            }

            
            auto it = m_objects.begin();
            while (it != m_objects.end()) {
                if (it->second.disappearedFrames > MAX_DISAPPEARED) {
                    it = m_objects.erase(it);
                }
                else {
                    ++it;
                }
            }
            return;
        }

        if (m_objects.empty()) {
            for (const auto& bbox : detections) {
                registerObject(bbox);
            }
            return;
        }


        
        std::vector<cv::Point> inputCentroids;
        for (const auto& rect : detections) {
            inputCentroids.emplace_back(rect.x + rect.width / 2, rect.y + rect.height / 2);
        }

       
        std::vector<int> objectIDs;
        std::vector<cv::Point> objectCentroids;
        for (const auto& pair : m_objects) {
            objectIDs.push_back(pair.first);
            objectCentroids.push_back(pair.second.center);
        }

        
        std::vector<bool> usedRows(objectIDs.size(), false);
        std::vector<bool> usedCols(detections.size(), false);

        

        for (size_t i = 0; i < objectIDs.size(); ++i) {
            double minDist = std::numeric_limits<double>::max();
            int bestMatchIdx = -1;

            for (size_t j = 0; j < detections.size(); ++j) {
                if (usedCols[j]) continue;

                double dist = cv::norm(objectCentroids[i] - inputCentroids[j]);

                if (dist < minDist && dist < MAX_DISTANCE) {
                    minDist = dist;
                    bestMatchIdx = j;
                }
            }

            if (bestMatchIdx != -1) {
             
                usedRows[i] = true;
                usedCols[bestMatchIdx] = true;

                // ????? ???????? ?????
                int objID = objectIDs[i];
                m_objects[objID].bbox = detections[bestMatchIdx];
                m_objects[objID].center = inputCentroids[bestMatchIdx];
                m_objects[objID].disappearedFrames = 0;
            }
        }

        // 4. ????? ???????

        // ????????? ????? ??? ???? ??? ????? -> ?????
        for (size_t i = 0; i < objectIDs.size(); ++i) {
            if (!usedRows[i]) {
                int objID = objectIDs[i];
                m_objects[objID].disappearedFrames++;
                if (m_objects[objID].disappearedFrames > MAX_DISAPPEARED) {
                    deregisterObject(objID);
                }
            }
        }

     
        for (size_t j = 0; j < detections.size(); ++j) {
            if (!usedCols[j]) {
                registerObject(detections[j]);
            }
        }
    }

    void ObjectTracker::draw(cv::Mat& frame) {
        for (const auto& pair : m_objects) {
            const auto& obj = pair.second;

            
            cv::Scalar color = (obj.disappearedFrames > 0) ? cv::Scalar(100, 100, 100) : cv::Scalar(0, 255, 0);

            
            cv::rectangle(frame, obj.bbox, color, 2);

        
            std::string text = "ID: " + std::to_string(obj.id);
            cv::putText(frame, text, cv::Point(obj.bbox.x, obj.bbox.y - 5),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 2);

            
            cv::circle(frame, obj.center, 4, cv::Scalar(0, 0, 255), -1);
        }
    }
}