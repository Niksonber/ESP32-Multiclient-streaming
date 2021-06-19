#pragma once
#include <esp_camera.h>
#include "camPins.hpp"
#include "streamingProvider.hpp"

/// Interface with camera, providing frame buffer and control methods
class CamProvider : public StreamingProvider{
public:
    /// Same contructor of StreamingProvider
    CamProvider() : StreamingProvider() {};

    /// Configure and init camera, get sensor ptr, set desired fps and create a task to get stream buffer periodically
    /// @param fps desired frame rate
    void begin(uint8_t fps);

    /// Set region of interest (ROI), thus camera only sends this window @param offsetX top left x point of ROI
    /// @param offsetY top left y point of ROI @param totalX botton right x point of ROI @param totalY botton right y point of ROI
    /// @param w width of new image @param h height of new image @return true if operation is sucessfully
    bool crop(int offsetX, int offsetY, int totalX, int totalY, int w, int h);

protected:
    /// sensor pointer
    sensor_t *_sensor;

    /// frame pointer
    camera_fb_t *_fb;
    
    /// Camera config
    camera_config_t _config;

    /// Get pointer to new data and its size. @param ptr At end this variable points to buffer
    /// @param size at end contains buffer size  @return true if buffer is sucessfully captured, false otherwise
    bool _getData(uint8_t ** ptr, size_t * size);
    
    /// Release data buffer to be replaced after reading
    void _releaseData();
};