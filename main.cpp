#include<yolo-trt.h>

#include <dxgi-cap.h>

#include<pid.h>

using cv::Mat;
using std::cout;
using std::endl;



int main(int argc, char** argv) {
    char* model_path = "D:/2/cf/cpp/end2end/build/Release/cf.trt";
    dxgi_cap dxgi;
    Yolo yolo(model_path);
    pid_control pid;
    dxgi.init();
    while (1)
    {
        auto start = std::chrono::system_clock::now();
        cv::Mat frame = dxgi.get_img(do_not_show_windows);
        if (frame.empty())
        {
            break;
        }
        float* Boxes = new float[4000];
        int* BboxNum = new int[1];
        int* ClassIndexs = new int[1000];
        //run inference
        yolo.Infer(frame.cols, frame.rows, frame.channels(), frame.data, Boxes, ClassIndexs, BboxNum);
        auto end = std::chrono::system_clock::now();
        cout << "FPS: " << 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << endl;
        if (BboxNum > 0 && KEY_DOWN(VK_MBUTTON))
        {
            pid.fire(Boxes, ClassIndexs, BboxNum, 0);
        }
        if(!KEY_DOWN(VK_MBUTTON))
        {
            pid.rec();
        }
    }
    dxgi.release();
    return 0;
}
