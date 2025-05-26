#include "edgedetect.hpp"
#include "channels.h"
#include "constants.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

double lowThreshold = 50;
const char *window_name = "debug";

void camera_init(VideoCapture *cam, int deviceID) {
    int apiID = cv::CAP_ANY;

    cam->open(deviceID, apiID);
    if (!cam->isOpened()) {
        cerr << "ERROR: Unable to open camera " << deviceID << endl;
        throw std::runtime_error("camera");
    }
}

void detect_edge(Mat *frame, Mat *result_edges) {
    Mat gray;
    cvtColor(*frame, gray, COLOR_BGR2GRAY);
    blur(gray, gray, Size(3, 3));
    Canny(gray, *result_edges, lowThreshold, lowThreshold * 3, 3);
}

void mat_to_array(Mat *input, unsigned char *output) {
    // unsigned char *newarr = (unsigned char *) calloc(input->cols * input->rows, sizeof(char));
    for (int i = 0; i < input->cols; i++) {
        for (int j = 0; j < input->rows; j++) {
            output[i + j * input->cols] = input->at< unsigned char >(j, i);
        }
    }
}

int edges(ring_buffer *buffer) {
    Mat frame, edges;
    VideoCapture cam;
    camera_init(&cam, 0);

    namedWindow(window_name, WINDOW_AUTOSIZE);
    cam.read(frame);
    size_t size = frame.rows * frame.cols;
    unsigned char *proxy = (unsigned char *) malloc(size);
    // ring_buffer *buffer = ring_new(size);

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl << "Press any key to terminate" << endl;
    for (;;) {
        // wait for a new frame from camera and store it into 'frame'
        cam.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        detect_edge(&frame, &edges);
        mat_to_array(&edges, proxy);
        Mat neee(edges.rows, edges.cols, CV_8U, proxy);
        proxy = ring_write(buffer, proxy);
        // show live and wait for a key with timeout long enough to show images
        imshow(window_name, neee);
        if (waitKey(5) == 27)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor

    return 0;
}