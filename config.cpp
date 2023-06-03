#include<config.h>

static const char* labels[] = {
    "body", "head"
};

void options::init()
{
    cout << "请选择截图方式：0、1" << endl;
    cin >> capture;
    if (capture)
        dxgi.init();
    else
        dc.init();
    mouse.pid.init(0.25, 0.05, 0.1);
}

void options::main_function()
{
    auto engine = Yolo::create_infer(
        model_file,                // engine file
        Yolo::Type::V5,                       // yolo type, Yolo::Type::V5 / Yolo::Type::X
        0,                   // gpu id
        0.25f,                      // confidence threshold
        0.45f,                      // nms threshold
        Yolo::NMSMethod::FastGPU,   // NMS method, fast GPU / CPU
        1024,                       // max objects
        true                       // preprocess use multi stream
    );


    while (1)
    {

        auto start = std::chrono::system_clock::now();
        cv::Mat frame;
        if (capture)
            frame = dxgi.get_img(do_not_show_windows);
        else
            frame = dc.CaptureScreen(do_not_show_windows);
        if (frame.empty())
        {
            continue;
        }
        

        auto box = engine->commit(frame).get();


        auto end = std::chrono::system_clock::now();
        //cout << "FPS: " << 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << endl;
        if (KEY_DOWN(VK_UP) && mouse.isHead == 0) {
            mouse.isHead = 1;
            std::cout << "头" << std::endl;
        }
        else if (KEY_DOWN(VK_DOWN) && mouse.isHead == 1) {
            mouse.isHead = 0;
            std::cout << "身" << std::endl;
        }
        if ((!box.empty()) && KEY_DOWN(VK_MBUTTON))
        {
            mouse.fire(frame, box);
        }
        else
        {
            mouse.pid.refresh();
        }
        if (is_show_windows)
        {
            draw_objects(frame, box, mouse.isHead);
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
        if (KEY_DOWN(VK_HOME) && mouse.is_use_hardware == 0)
        {
            dxgi.release();
            dxgi.init();
        }



    }
    engine.reset();
    dxgi.release();
}


void options::draw_objects(Mat img, ObjectDetector::BoxArray box, int is_head)
{
    for (auto& obj : box) {
        if (obj.class_label != is_head)
            continue;
        cv::rectangle(img, cv::Point(obj.left, obj.top), cv::Point(obj.right, obj.bottom), cv::Scalar(0,255,255));

        auto name = labels[obj.class_label];
        auto caption = iLogger::format("%s %.2f", name, obj.confidence);
        cv::putText(img, caption, cv::Point(obj.left, obj.top - 5), 0, 1, cv::Scalar(0,255,255));
    }
}