#include "camProvider.hpp"

void CamProvider::begin(uint8_t fps){
    /// config pins of esp32 AI-Thinker
    _config = CameraPins::ai_thinker;
    _config.ledc_timer   = LEDC_TIMER_0;
    _config.ledc_channel = LEDC_CHANNEL_0;
    _config.pixel_format = PIXFORMAT_JPEG;
    _config.xclk_freq_hz = 20000000;

    if ( psramFound() ){
        log_i("PSRAM found");
        psramInit();
        _config.frame_size = FRAMESIZE_XGA;
        _config.jpeg_quality = 15;
        _config.fb_count = 2;
    } else{
        log_i("PSRAM not found");
        _config.frame_size = FRAMESIZE_VGA;
        _config.jpeg_quality = 17;
        _config.fb_count = 1;
    }
    
    if( esp_camera_init(&_config) != ESP_OK ){
        log_e("Fail in camera init");
        esp_restart();
    }
    
    _sensor = esp_camera_sensor_get();
    /// crop image
    crop(0, 0, 400, 400, 400, 400);

    /// Create a task to read camera
    StreamingProvider::begin(fps);
}

bool CamProvider::crop(int offsetX, int offsetY, int totalX, int totalY, int w, int h){
    if (_sensor->id.PID == OV2640_PID){
        _sensor->set_res_raw(_sensor, 1, 0, 0, 0, offsetX, offsetY, totalX, totalY, w, h, false, false);
        return true;
    }
    log_e("Crop is not available for sensor type yet");
    return false;
}

bool CamProvider::_getData(uint8_t **ptr, size_t *size){
    _fb = esp_camera_fb_get();
    if ( _fb != nullptr ){
        if ( _fb->format == PIXFORMAT_JPEG ){
            *ptr = _fb->buf;
            *size = _fb->len;
            return true;
        }
        return frame2jpg(_fb, 15, ptr, size);
    } 
    log_i("error in frame capture");
    return false;
}

void CamProvider::_releaseData(){
    esp_camera_fb_return(_fb);
}