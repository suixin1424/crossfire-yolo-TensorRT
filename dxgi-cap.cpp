#include<dxgi-cap.h>
HRESULT dxgi_cap::initDXGIResources(ID3D11Device** device, ID3D11DeviceContext** deviceContext)
{
    HRESULT ret = S_OK;
    D3D_DRIVER_TYPE DriverTypes[] =
    {
D3D_DRIVER_TYPE_HARDWARE,
D3D_DRIVER_TYPE_WARP,
D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT NumDriverTypes = ARRAYSIZE(DriverTypes);
    D3D_FEATURE_LEVEL FeatureLevels[] =
    {
D3D_FEATURE_LEVEL_11_0,
D3D_FEATURE_LEVEL_10_1,
D3D_FEATURE_LEVEL_10_0,
D3D_FEATURE_LEVEL_9_1
    };
    UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
    D3D_FEATURE_LEVEL FeatureLevel;
    for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes;
        ++DriverTypeIndex)
    {
        ret = D3D11CreateDevice(nullptr, DriverTypes[DriverTypeIndex],
            nullptr, 0, FeatureLevels, NumFeatureLevels, D3D11_SDK_VERSION, device,
            &FeatureLevel, deviceContext);
        if (ret >= 0)
        {
            break;
        }
    }
    if (ret < 0)
    {
        return S_FALSE;
    }
    return S_OK;
}
void dxgi_cap::clearDXGIResources(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
    if (device)
    {
        device->Release();
        device = nullptr;
    }
    if (deviceContext)
    {
        deviceContext->Release();
        deviceContext = nullptr;
    }
}
HRESULT dxgi_cap::initDuplication(ID3D11Device* device, IDXGIOutputDuplication** dupl, UINT output)
{
    /*IDXGIDevice* DxgiDevice = nullptr;
    HRESULT ret = device->QueryInterface(__uuidof(IDXGIDevice),
    reinterpret_cast<void**>(&DxgiDevice));
    if (ret < 0)
    {
    return S_FALSE;
    }*/
    IDXGIDevice2* DxgiDevice2;
    HRESULT test = device->QueryInterface(__uuidof(IDXGIDevice2), (void
        **)&DxgiDevice2);
    DxgiDevice2->SetMaximumFrameLatency(1);
    //uint k;
    //DxgiDevice2->GetMaximumFrameLatency(&k); 
    //std::cout << k << endl;
    IDXGIAdapter* DxgiAdapter = nullptr;
    HRESULT ret = DxgiDevice2->GetParent(__uuidof(IDXGIAdapter),
        reinterpret_cast<void**>(&DxgiAdapter));
    DxgiDevice2->Release();
    DxgiDevice2 = nullptr;
    if (ret < 0)
    {
        return S_FALSE;
    }
    /*IDXGIFactory2 * pIDXGIFactory;
    HRESULT ret_factory =  DxgiAdapter->GetParent(__uuidof(IDXGIFactory2),
    (void **)&pIDXGIFactory);
    std::cout << ret_factory << endl;*/
    IDXGIOutput* DxgiOutput = nullptr;
    ret = DxgiAdapter->EnumOutputs(output, &DxgiOutput);
    DxgiAdapter->Release();
    DxgiAdapter = nullptr;
    if (ret < 0)
    {
        return S_FALSE;
    }
    IDXGIOutput1* DxgiOutput1 = nullptr;
    ret = DxgiOutput->QueryInterface(__uuidof(DxgiOutput1),
        reinterpret_cast<void**>(&DxgiOutput1));
    DxgiOutput->Release();
    DxgiOutput = nullptr;
    if (ret < 0)
    {
        return S_FALSE;
    }
    ret = DxgiOutput1->DuplicateOutput(device, dupl);
    DxgiOutput1->Release();
    DxgiOutput1 = nullptr;
    if (ret < 0)
    {
        return S_FALSE;
    }
    return S_OK;
}
void dxgi_cap::clearDuplication(IDXGIOutputDuplication* dupl)
{
    if (dupl)
    {
        dupl->Release();
        dupl = nullptr;
    }
}
HRESULT dxgi_cap::getFrame(IDXGIOutputDuplication* dupl, DXGI_OUTDUPL_FRAME_INFO* frameInfo, ID3D11Texture2D** texture2D, bool* timeOut)
{
    IDXGIResource* desktopResource;
    //std::cout << frameInfo->AccumulatedFrames << std::endl; 
    // 超时时间为500毫秒
    HRESULT ret = dupl->AcquireNextFrame(100, frameInfo, &desktopResource);
    // 当画面变化较慢时可能会超时
    if (ret == DXGI_ERROR_WAIT_TIMEOUT)
    {
        *timeOut = true;
        return S_OK;
    }
    *timeOut = false;
    if (ret < 0)
    {
        return S_FALSE;
    }
    if (texture2D && *texture2D)
    {
        (*texture2D)->Release();
        (*texture2D) = nullptr;
    }
    ret = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(texture2D));
    desktopResource->Release();
    desktopResource = nullptr;
    if (ret < 0)
    {
        return S_FALSE;
    }
    return S_OK;
}
HRESULT dxgi_cap::doneWithFrame(IDXGIOutputDuplication* dupl)
{
    HRESULT ret = dupl->ReleaseFrame();
    if (ret < 0)
    {
        return S_FALSE;
    }
    return S_OK;
}
void dxgi_cap::saveDesktopImage(ID3D11Texture2D* data, ID3D11Device* device, ID3D11DeviceContext* deviceContext, cv::Mat* img)
{
    D3D11_TEXTURE2D_DESC dataDesc;
    // 获取纹理(图像)的相关信息
    data->GetDesc(&dataDesc);
    ID3D11Texture2D* copyDesktop = nullptr;
    dataDesc.Usage = D3D11_USAGE_STAGING;
    dataDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    dataDesc.BindFlags = 0;
    dataDesc.MiscFlags = 0;
    dataDesc.MipLevels = 1;
    dataDesc.ArraySize = 1;
    dataDesc.SampleDesc.Count = 1;
    device->CreateTexture2D(&dataDesc, NULL, &copyDesktop);
    deviceContext->CopyResource(copyDesktop, data);
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    deviceContext->Map(copyDesktop, 0, D3D11_MAP_READ, 0, &mappedResource);
    //std::cout << dataDesc.Height << std::endl;
    cv::Mat src_img = cv::Mat(dataDesc.Height, dataDesc.Width, CV_8UC4,
        mappedResource.pData);
    //auto start = std::chrono::system_clock::now();
    int width = 416;
    int height = 416;
    int left = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    int top = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
    cv::Rect rect(left, top, width, height);
    /*auto end = std::chrono::system_clock::now();
    std::cout << ((std::chrono::duration_cast<std::chrono::milliseconds>
    (end - start).count())) << std::endl;*/
    *img = src_img(rect);
    //std::cout << mappedResource.DepthPitch << std::endl; 
    // 创建窗口
    //cv::namedWindow("Display", CV_WINDOW_AUTOSIZE);
    ////显示图像
    //cv::imshow("Display", *img); 
    ///暂停，等待按键结束
    //cv::waitKey(0);
    //saveBMPFile(pictureName, mappedResource.pData, dataDesc.Width, dataDesc.Height);
    copyDesktop->Release();
}
void dxgi_cap::init()
{
    initDXGIResources(&device, &deviceContext);
    initDuplication(device, &desktopDupl, output);
}
void dxgi_cap::release()
{
    clearDuplication(desktopDupl);
    clearDXGIResources(device, deviceContext);
}
cv::Mat dxgi_cap::get_img(int is_show)
{
    ret = getFrame(desktopDupl, &frameInfo, &acquiredDesktopImage, &timeOut);
    cv::Mat img = cv::Mat(416, 416, CV_8UC4);
    // 截屏
    if (acquiredDesktopImage == nullptr)
    {
        return img;
    }
    saveDesktopImage(acquiredDesktopImage, device, deviceContext, &img);
    doneWithFrame(desktopDupl);
    if (acquiredDesktopImage)
    {
        acquiredDesktopImage->Release();
        acquiredDesktopImage = nullptr;
    }
    cv::cvtColor(img, img, cv::COLOR_RGBA2RGB);
    //resize图片
    cv::resize(img, img, cv::Size(416, 416));
    // 创建窗口
    //cv::namedWindow("Display", CV_WINDOW_AUTOSIZE);
    //显示图像
    if (is_show == 1)
    {
        cv::imshow("Display", img);
        ////暂停，等待按键结束
        cv::waitKey(1);
    }
    return img;
}


Mat dc_cap::CaptureScreen(int is_show_windows)
{
    // 获取屏幕设备上下文
    HDC hScreenDC = GetDC(NULL);
    // 创建内存设备上下文
    HDC hMemDC = CreateCompatibleDC(hScreenDC);

    // 创建位图对象
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    // 将位图选入内存设备上下文
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // 将屏幕内容复制到内存设备上下文
    BitBlt(hMemDC, 0, 0, width, height, hScreenDC, x, y, SRCCOPY);

    // 恢复位图对象
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

    // 创建OpenCV Mat对象并将位图数据复制到其中
    BITMAPINFO bitmapInfo;
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -height; // 负值表示顶部-左下角为原点
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 24; // 每个像素3个字节（RGB）
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    bitmapInfo.bmiHeader.biSizeImage = 0;
    bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    bitmapInfo.bmiHeader.biClrUsed = 0;
    bitmapInfo.bmiHeader.biClrImportant = 0;

    cv::Mat mat(height, width, CV_8UC3);
    GetDIBits(hMemDC, hBitmap, 0, height, mat.data, &bitmapInfo, DIB_RGB_COLORS);

    if (is_show_windows)
    {
        imshow("img", mat);
        waitKey(1);
    }


    // 清理资源
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);

    // 翻转图像以适应OpenCV的坐标系
    //cv::flip(mat, mat, 0);

    return mat;
}

void dc_cap::init()
{
    x = 960 - 416 / 2;
    y = 540 - 416 / 2;
}