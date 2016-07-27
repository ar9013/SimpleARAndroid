/*
 *    Copyright 2016 Anand Muralidhar
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef SIMPLEARCLASS_H
#define SIMPLEARCLASS_H

#include "myLogger.h"
#include "myGLFunctions.h"
#include "myGLCamera.h"
#include "assimpLoader.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <mutex>
#include <backTexture.h>

#define MIN_KPS_IN_FRAME            300
#define MIN_INLIER_COUNT            25
#define CAM_HEIGHT_FROM_FLOOR       75
#define NN_MATCH_RATIO              0.8f    // Nearest-neighbour matching ratio
#define RANSAC_THRESH               2.5f    // RANSAC inlier threshold

class SimpleARClass {
public:
    SimpleARClass();
    ~SimpleARClass();
    void    PerformGLInits();
    void    Render();
    void    SetViewport(int width, int height);
    void    ProcessCameraImage(cv::Mat cameraRGBImage, int mPreview_width, int mPreview_height);
    void    SetCameraPreviewDims(int cameraPreviewWidth, int cameraPreviewHeight);
    void    DoubleTapAction();
    void    UpdateGravity(float gx, float gy, float gz);

private:
    void    DetectAndHighlightCorners();
    bool    DetectKeypointsInReferenceImage();
    bool    DetectKeypointsInQueryImage();
    void    TrackKeypointsAndUpdatePose();
    cv::Mat ConstructCameraIntrinsicMatForCV(glm::mat4 projectionViewMat,
                                             float imageWidth, float imageHeight);

    cv::Ptr<cv::DescriptorMatcher> matcher;
    cv::Mat referenceDescriptors;
    std::vector<cv::KeyPoint> referenceKeypoints;

    bool    initsDone;
    int     screenWidth, screenHeight;
    cv::Mat cameraImageForBack;
    BackTexture * back;
    bool    newCameraImage;
    int     cameraPreviewWidth, cameraPreviewHeight;
    bool    trackingIsOn;
    bool    pnpResultIsValid;

    std::mutex  cameraMutex;
    cv::Ptr<cv::Feature2D> cornerDetector;
    std::vector<cv::KeyPoint> keyPoints, sourceInlierKeypoints, queryInlierKeypoints;
    std::vector<cv::Point2f> sourceInlierPoints, queryInlierPoints;
    cv::Mat     translationVector, rotationVector;
    cv::Mat translationVectorCopy, rotationVectorCopy;
    std::mutex  pnpMutex;

    std::mutex  gravityMutex;
    std::vector <float> gravity;
    glm::vec3   sourceGravityVector;

    std::vector <float> modelDefaultPosition;
    MyGLCamera  * myGLCamera;
    AssimpLoader * modelObject;

};

#endif //SIMPLEARCLASS_H
