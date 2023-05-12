# crossfire-yolo-TensorRT   
理论支持yolo全系列模型  
基于yolo-trt的穿越火线ai自瞄  
使用方法： 
需自备arduino leonardo设备  
刷入arduino文件夹内文件  
修改auto-fire.cpp第32行，将串口改为你的arduino设备串口  
修改config.h第11行，将trt文件路径改为你自己的路径  
修改config.cpp第7，8行，调节pid参数  
main函数中有四个线程，分别为截图+推理，x方向鼠标移动，y方向鼠标移动，自动开火  
  
当前版本取消自动开火功能，因为基本秒封  
自动开火相关代码已注释，可自行修改  
自动开火功能必须要求有arduino leonardo设备，否则无法使用  
  
  
下一步目标是增加目标追踪，以解决多个目标之间乱晃的问题  
欢迎各位开发者共同改进  
pt转trt可参考https://github.com/Linaom1214/TensorRT-For-YOLO-Series  
cmake编译可参考https://github.com/Monday-Leo/Yolov5_Tensorrt_Win10  
cf被封机器码了，解封再更新  
