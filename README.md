# crossfire-yolo-TensorRT   
理论支持yolo全系列模型  
基于yolo-trt的穿越火线ai自瞄  
使用方法： 
修改config.h第14行，将trt文件路径改为你自己的路径  
修改config.cpp第7行，调节pid参数  
main函数中有三个线程，分别为截图+推理，鼠标移动，平滑鼠标移动  
  
程序大幅度精简，去除自动开火和arduino设备通讯（基本没用），代码阅读性提高  
当前版本通过两个线程的鼠标移动相互补偿，实测能较好的过行为检测（庆祝！！！）  
  
  
下一步目标是程序根据本机自动设置最优pid参数    
欢迎各位开发者共同改进  
pt转trt可参考https://github.com/Linaom1214/TensorRT-For-YOLO-Series  
cmake编译可参考https://github.com/Monday-Leo/Yolov5_Tensorrt_Win10  
 
