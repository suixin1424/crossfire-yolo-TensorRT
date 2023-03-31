# cf-yolo-trt  
理论支持yolo全系列模型  
基于yolo-trt的穿越火线ai自瞄  
使用方法：
需自备arduino leonardo设备  
刷入arduino文件夹内文件  
修改mouse.cpp第94行，将串口改为你的arduino设备串口  
目前有三个类  
dxgi_cap类，负责截图  
Yolo类，负责推理  
mouse_control类，负责鼠标移动  
pt转trt可参考https://github.com/Linaom1214/TensorRT-For-YOLO-Series  
cmake编译可参考https://github.com/Monday-Leo/Yolov5_Tensorrt_Win10  
