#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp> 
#include <sstream>
#include <ctime>

using namespace cv; 
using namespace std;

string time() {
    std::time_t now = std::time(nullptr);
    std::tm timeinfo = *std::localtime(&now);

    char buffer[80];
    std::strftime(buffer, 80, "%d/%m/%Y", &timeinfo);
    std::string dateStr = buffer;

    std::replace(dateStr.begin(), dateStr.end(), '/', '-');

    std::cout << dateStr << std::endl;

    return dateStr;
}

cv::Point start_pt(-1, -1);
cv::Point end_pt(-1, -1);

cv::Point end_start_pt(-1, -1);
cv::Point end_end_pt(-1, -1);
int width_px = -1;
int height_px = -1;
int flag = 0;
int key = 0;

bool lbutton_down = false;

cv::Mat draw_rectangle(cv::Mat input, cv::Point p1, cv::Point p2) {
    cv::Mat output = input.clone();
    int thickness = 2;
    cv::Scalar color(0, 255, 0);
    int lineType = cv::LINE_8;
    int shift = 0;
    cv::rectangle(output, p1, p2, color, thickness, lineType, shift);

    return output;
}

void rec(){
    int x = end_start_pt.x;
    int y = end_start_pt.y;

    string str = "ffmpeg -f x11grab -framerate 25 -video_size " + to_string(width_px) + "x" + to_string(height_px) + " -i :0.0+" + to_string(x) +"," + to_string(y) + " output-"+ time() +".mp4";
    
    cout << str << "\n";
    const char* myCString = str.c_str();
    system(myCString);
}

void on_mouse(int event, int x, int y, int flags, void* userdata) {
    static cv::Mat original_desktop = *((cv::Mat*)userdata);
    cv::Mat desktop = *((cv::Mat*)userdata);

    if (event == cv::EVENT_MOUSEMOVE) {
        if (lbutton_down) {
            end_pt = cv::Point(x, y);
            desktop = original_desktop.clone();
            desktop = draw_rectangle(desktop, start_pt, end_pt);
            cv::imshow("Screen", desktop);
        }
    }
    if (event == cv::EVENT_LBUTTONDOWN) {  
        start_pt = cv::Point(x, y); 
        lbutton_down = true;
    }
    else if (event == cv::EVENT_LBUTTONUP) {  
        end_pt = cv::Point(x, y); 
        original_desktop = desktop.clone();
        desktop = draw_rectangle(desktop, start_pt, end_pt);
        lbutton_down = false;
        cv::imshow("Screen", desktop);
        *((cv::Mat*)userdata) = desktop.clone();
        end_start_pt = start_pt;
        end_end_pt = end_pt; 
        

        width_px = std::abs(end_end_pt.x - end_start_pt.x);
        height_px = std::abs(end_end_pt.y - end_start_pt.y);
        std::cout << "Rozmiar prostokąta: " << width_px << "x" << height_px << " pikseli." << std::endl;
        key = 27;
        cv::destroyAllWindows();
        rec();
    }
}

int main() {
    system("rm screenshot.png");
    system("scrot -d 1 screenshot.png");

    cv::Mat desktop = cv::imread("screenshot.png", cv::IMREAD_UNCHANGED);
    cv::Mat original_desktop = desktop.clone();

    cv::namedWindow("Screen", cv::WINDOW_NORMAL);
    cv::setWindowProperty("Screen", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
    cv::setWindowProperty("Screen", cv::WND_PROP_AUTOSIZE, cv::WINDOW_AUTOSIZE);
    cv::imshow("Screen", desktop);

    cv::setMouseCallback("Screen", on_mouse, &desktop);

    while (key != 27) {
        key = cv::waitKey(0);
    }

    return 0;
}
