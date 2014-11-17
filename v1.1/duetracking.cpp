#include <unistd.h>
#include "serial_talk.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs/imgcodecs_c.h"



#include <iostream>
#include <ctype.h>


#include "opencv2/core/core_c.h"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"




using namespace cv;
using namespace std;


Mat image;

bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 10, vmax = 256, smin = 30;

static void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
            trackObject = -1;
        break;
    }
}

static void help()
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
            "You select a color objects such as your face and it tracks it.\n"
            "This reads from video camera (0 by default, or the camera number the user enters\n"
            "Usage: \n"
            "   ./objectfilename [camera number]\n";

    cout << "\n\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tc - stop the tracking\n"
            "\tb - switch to/from backprojection view\n"
            "\th - show/hide object histogram\n"
            "\tp - pause video\n"
            "To initialize tracking, select the object with mouse\n";
}

const char* keys =
{
    "{1|  | 0 | camera number}"

};

int main( int argc, const char** argv )
{

   char portname[20] = "/dev/ttyACM0";
   char baud[20] = "9600";
   port_initialize(portname, baud);			      //intialize port



   string command;
   command = "Serial Communication Testing ";
   cout << "Command: " << command << endl;
   send_via_port(&command, "string", 0);                     //sending string command
   sleep(1);



    help();

    VideoCapture cap;
    Rect prevtrackWindow;
    Rect trackWindow;
    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;
    CommandLineParser parser(argc, argv, keys);
    int camNum = parser.get<int>("1");

    cap.open(camNum);

    if( !cap.isOpened() )
    {
        help();
        cout << "***Could not initialize capturing...***\n";
        cout << "Current parameter's value: \n";
        //parser.printParams();
        return -1;
    }

    namedWindow( "Histogram", 0 );
    namedWindow( "CamShift Demo", 0 );
    setMouseCallback( "CamShift Demo", onMouse, 0 );
    createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
    createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
    createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );

    Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
    bool paused = false;

    for(;;)
    {
        if( !paused )
        {
            cap >> frame;
            if( frame.empty() )
                break;
        }

        frame.copyTo(image);

        if( !paused )
        {
            cvtColor(image, hsv, COLOR_BGR2HSV);

            if( trackObject )
            {
                int _vmin = vmin, _vmax = vmax;

                inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                        Scalar(180, 256, MAX(_vmin, _vmax)), mask);
                int ch[] = {0, 0};
                hue.create(hsv.size(), hsv.depth());
                mixChannels(&hsv, 1, &hue, 1, ch, 1);

                if( trackObject < 0 )
                {
                    Mat roi(hue, selection), maskroi(mask, selection);
                    calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
                    normalize(hist, hist, 0, 255, CV_MINMAX);

                    trackWindow = selection;
                    trackObject = 1;

                    histimg = Scalar::all(0);
                    int binW = histimg.cols / hsize;
                    Mat buf(1, hsize, CV_8UC3);
                    for( int i = 0; i < hsize; i++ )
                        buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
                    cvtColor(buf, buf, CV_HSV2BGR);

                    for( int i = 0; i < hsize; i++ )
                    {
                        int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
                        rectangle( histimg, Point(i*binW,histimg.rows),
                                   Point((i+1)*binW,histimg.rows - val),
                                   Scalar(buf.at<Vec3b>(i)), -1, 8 );
                    }
                }

                calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
                backproj &= mask;
                RotatedRect trackBox = CamShift(backproj, trackWindow,
                                    TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));


        if( trackWindow.area() <= 40000 )   // check tracker window is big enough
        {


             int c1x = (prevtrackWindow.tl().x + prevtrackWindow.br().x)/2 ;
             int c1y = (prevtrackWindow.tl().y + prevtrackWindow.br().y)/2 ;

             int c2x = (trackWindow.tl().x + trackWindow.br().x)/2 ;
             int c2y = (trackWindow.tl().y + trackWindow.br().y)/2 ;


             double prevarea = prevtrackWindow.area();
             double currentarea = trackWindow.area();

             bool right = false, left = false, forward = false, backward = false;

            if((c2x - c1x) >= 10)
            {
                //turn right
                right = true;
		cout << "right: " << right << endl;

            }
            if((c2x - c1x) <= -10)
            {
                //turn left
                left = true;
		cout << "left: " << left << endl;
            }
            if(((currentarea - prevarea)/prevarea) < -0.1)
            {
                //move forward
                forward = true;
		cout << "forward: " << forward << endl;

            }
            if(((currentarea - prevarea)/prevarea) > 0.1)
            {
                //move backward
                backward = true;
		cout << "backward: " << backward << endl;

            }


            //decision module

            /*if (forward)
            {
                command = "f";
                cout << "Command: " << command << endl;
                //send_via_port(&command, "string", 0);                     //sending string command
                //sleep(1);
            }
            else    if (right)
            {
                command = "r";
                cout << "Command: " << command << endl;
                //send_via_port(&command, "string", 0);                     //sending string command
                //sleep(1);
            }
            else    if (left)
            {
                command = "l";
                cout << "Command: " << command << endl;
                //send_via_port(&command, "string", 0);                     //sending string command
                //sleep(1);
            }
            else    if (backward)
            {
                command = "b";
                cout << "Command: " << command << endl;
               //send_via_port(&command, "string", 0);                     //sending string command
                //sleep(1);
            }
            else
            {
                //stay put //stop motors
                command = "s";
                cout << "Command: " << command << endl;
                //send_via_port(&command, "string", 0);                     //sending string command
                //sleep(1);
            }*/



        }
        else if( trackWindow.area() <= 1 )
                {
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                                       trackWindow.x + r, trackWindow.y + r) &
                                  Rect(0, 0, cols, rows);


                                //testing serial
                                //command = "s";
               			//cout << "Command: " << command << endl;
                            	//send_via_port(&command, "string", 0);                     //sending string command
                		//sleep(1);
                }

                if( backprojMode )
                    cvtColor( backproj, image, COLOR_GRAY2BGR );
                ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
            }
        }
        else if( trackObject < 0 )
            paused = false;

        if( selectObject && selection.width > 0 && selection.height > 0 )
        {
            Mat roi(image, selection);
            bitwise_not(roi, roi);
        }

        imshow( "CamShift Demo", image );
        imshow( "Histogram", histimg );

        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'b':
            backprojMode = !backprojMode;
            break;
        case 'c':
            trackObject = 0;
            histimg = Scalar::all(0);
            break;
        case 'h':
            showHist = !showHist;
            if( !showHist )
            paused = !paused;
            break;
        default:
            ;
        }
     prevtrackWindow = trackWindow;

    }

    return 0;
}



//amazon@amazon:~/cvprojects/task1$ g++ -o objtracking `pkg-config --cflags opencv` objtracking.cpp `pkg-config --libs opencv`

