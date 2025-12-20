#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>

namespace Vision {

  
    struct TrackedObject {
        int id;             
        cv::Rect bbox;     
        cv::Point center;   
        int disappearedFrames; 
    };

    class ObjectTracker {
    public:
      
        static constexpr int MAX_DISAPPEARED = 30; 
        static constexpr double MAX_DISTANCE = 50.0; 

        ObjectTracker();
        ~ObjectTracker() = default;

    
        void update(const std::vector<cv::Rect>& detections);

      
        void draw(cv::Mat& frame);

   
        const std::map<int, TrackedObject>& getObjects() const { return m_objects; }

    private:
      
        void registerObject(const cv::Rect& bbox);

       
        void deregisterObject(int objectId);

        int m_nextObjectID = 0; 
        std::map<int, TrackedObject> m_objects; 
    };

}
