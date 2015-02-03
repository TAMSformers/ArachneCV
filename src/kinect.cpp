#include <iostream>

#include <opencv2/opencv.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>

namespace lf2 = libfreenect2;

lf2::Freenect2 freenect2;
lf2::Freenect2Device *device;
lf2::SyncMultiFrameListener *listener;
lf2::FrameMap frames;

bool open();
cv::Mat* getFrames();
void close();

int main()
{
    std::cout << "Hello world" << std::endl;
    open();
    return 0;
}

bool open()
{
    device = freenect2.openDefaultDevice();
    
    if (device == 0) {
        return false;
    }

    listener = new lf2::SyncMultiFrameListener(
            lf2::Frame::Color | lf2::Frame::Ir | lf2::Frame::Depth);
    
    device->setColorFrameListener(listener);
    device->setIrAndDepthFrameListener(listener);
    device->start();
}

cv::Mat* getFrames()
{
    listener->waitForNewFrame(frames);

    lf2::Frame *rgb = frames[lf2::Frame::Color];
    lf2::Frame *ir = frames[lf2::Frame::Ir];
    lf2::Frame *depth = frames[lf2::Frame::Depth];

    int size = (3 * rgb->height * rgb->width) + 
               (4 * ir->height * ir->width) +
               (4 * depth->height * depth->width);

    cv::Mat *mats = static_cast<cv::Mat*>(malloc(size));

    mats[0] = cv::Mat(rgb->height, rgb->width, CV_8UC3, rgb->data);
    mats[1] = cv::Mat(ir->height, ir->width, CV_32FC1, ir->data) / 20000.0f;
    mats[2] = cv::Mat(depth->height, depth->width, CV_32FC1, depth->data) / 4500.0f;

    listener->release(frames);

    return mats;
}

void close()
{
    device->stop();
    device->close();

    delete listener;
    delete device;
}
