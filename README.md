# crossfire-yolo-TensorRT   
理论支持yolo全系列模型  
基于yolo-trt的穿越火线ai自瞄  
  
    
使用方法： 
参考 https://github.com/shouxieai/tensorRT_Pro 先成功编译并运行tensorrt_pro  
然后将本程序中的cpp文件复制到tensorrt_pro的src目录下，将include文件夹中内容复制到src\application文件夹下  
成功编译后请将model文件夹下的onnx复制到tensorrt_pro的workspace目录下，然后运行你编译好的程序  
  
  
main函数中有三个线程，分别为截图+推理，鼠标移动，平滑鼠标移动  
  
  
  
下一步目标是程序根据本机自动设置最优pid参数    
欢迎各位开发者共同改进   

 
