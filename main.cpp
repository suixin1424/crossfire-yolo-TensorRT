#include<yolo-trt.h>

#include <dxgi-cap.h>

#include<mouse.h>

using cv::Mat;
using std::cout;
using std::endl;



int main(int argc, char** argv) {
    char* model_path = "D:/2/cf/cpp/end2end/build/Release/cf.trt";
    dxgi_cap dxgi;
    Yolo yolo(model_path);
    mouse_control pid;
    dxgi.init();
    int is_show_windows = 0;
    int where = 0;//默认为身
    cout << "是否显示窗口" << endl;
    cin >> is_show_windows;
    cout << "开始游戏吧" << endl;
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
        //cout << "FPS: " << 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << endl;
        if (KEY_DOWN(VK_UP) && where == 0) {
            where = 1;
            std::cout << "头"<< std::endl;
        }
        else if (KEY_DOWN(VK_DOWN) && where == 1) {
            where = 0;
            std::cout << "身" << std::endl;
        }
        if (BboxNum > 0 && KEY_DOWN(VK_MBUTTON))
        {
            pid.fire(frame,Boxes, ClassIndexs, BboxNum, where);
        }
        if (is_show_windows)
        {
            yolo.draw_objects(frame, Boxes, ClassIndexs, BboxNum,where);
            putText(frame, "fps:" + std::to_string(1000 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()), Point(10, 50), FONT_HERSHEY_PLAIN, 1.6, Scalar(0, 0, 255), 2);
            imshow("img", frame);
            HWND hWnd = (HWND)cvGetWindowHandle("img");
            HWND hRawWnd = ::GetParent(hWnd);

            if (NULL != hRawWnd)
            {
                BOOL bRet = ::SetWindowPos(hRawWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                assert(bRet);
            }
            waitKey(1);
        }
        delete []Boxes;
        delete []BboxNum;
        delete []ClassIndexs;
    }
    dxgi.release();
    return 0;
}
