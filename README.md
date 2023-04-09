# cf-yolo-trt  
理论支持yolo全系列模型  
基于yolo-trt的穿越火线ai自瞄  
使用方法： 
需自备arduino leonardo设备  
刷入arduino文件夹内文件  
修改auto-fire.cpp第32行，将串口改为你的arduino设备串口  
修改config.h第11行，将trt文件路径改为你自己的路径
main函数中有三个线程，分别为截图+推理，鼠标移动，自动开火  
  
  
自动开火功能必须要求有arduino leonardo设备，否则无法使用  
  
  
pt转trt可参考https://github.com/Linaom1214/TensorRT-For-YOLO-Series  
cmake编译可参考https://github.com/Monday-Leo/Yolov5_Tensorrt_Win10  
