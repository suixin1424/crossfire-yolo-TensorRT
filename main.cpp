#include<config.h>

using namespace std;



int main(int argc, char** argv){
    
    
    string model_file = "cf.FP16.trtmodel";
    string onnx_file = "cf.onnx";

    auto int8process = [=](int current, int count, const vector<string>& files, shared_ptr<TRT::Tensor>& tensor) {

        //INFO("Int8 %d / %d", current, count);

        for (int i = 0; i < files.size(); ++i) {
            auto image = cv::imread(files[i]);
            Yolo::image_to_tensor(image, tensor, Yolo::Type::V5, i);
        }
    };


    if (not iLogger::exists(model_file)) {
        TRT::compile(
            TRT::Mode::FP16,                       // FP32、FP16、INT8
            16,            // max batch size
            onnx_file,                  // source 
            model_file,                 // save to
            {},
            int8process,
            "inference"
        );
    }

    options option;
    option.init();
    cout << "is_show_windows?" << endl;
    cin >> option.is_show_windows;
    cout << "ok" << endl;

    std::thread main_Thread(&options::sendWrapper, &option);
    std::thread move_Thread(&pid_move::receiveWrapper2, &option.mouse.pid);
    std::thread smooth_Thread(&pid_move::receiveWrapper3, &option.mouse.pid);

    main_Thread.join();
    move_Thread.join();
    smooth_Thread.join();

    return 0;
}
