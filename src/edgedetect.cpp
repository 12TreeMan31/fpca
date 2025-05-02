#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>

#include "includes/channels.h"
#include "includes/constants.h"

using namespace std;
using namespace cv;


const char *window_name = "Live";
Mat frame;
double lowThreshold = 50;

void edging() {
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    blur(gray, frame, Size(3, 3));
    Canny(frame, frame, lowThreshold, lowThreshold * 3, 3);
}

void to_array(Mat *input, unsigned char *output) {
    // unsigned char *newarr = (unsigned char *) calloc(input->cols * input->rows, sizeof(char));
    for (int i = 0; i < input->cols; i++) {
        for (int j = 0; j < input->rows; j++) {
            output[i + j * input->cols] = input->at< unsigned char >(j, i);
        }
    }
}

int main() {
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    namedWindow(window_name, WINDOW_AUTOSIZE);
    cap.read(frame);
    size_t size = frame.rows * frame.cols;
    unsigned char *proxy = (unsigned char *) malloc(size);
    ring_buffer *buffer = ring_new(size);

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl << "Press any key to terminate" << endl;
    for (;;) {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        edging();
        to_array(&frame, proxy);
        ring_write(buffer, proxy);
        Mat neee(frame.rows, frame.cols, CV_8U, proxy);
        // show live and wait for a key with timeout long enough to show images
        imshow(window_name, neee);
        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor

    return 0;
}