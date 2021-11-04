#pragma once
#include <esp_camera.h>

/// Camera pins for diferents boards
namespace CameraPins{
    
    /// Wrover dev kit camera pins 
    constexpr camera_config_t wrover_kit = {
        .pin_pwdn       = -1,
        .pin_reset      = -1,
        .pin_xclk       = 21,
        .pin_sscb_sda   = 26,
        .pin_sscb_scl   = 27,

        .pin_d7         = 35,
        .pin_d6         = 34,
        .pin_d5         = 39,
        .pin_d4         = 36,
        .pin_d3         = 19,
        .pin_d2         = 18,
        .pin_d1         =  5,
        .pin_d0         =  4,
        .pin_vsync      = 25,
        .pin_href       = 23,
        .pin_pclk       = 22,
    };
     
    /// ESP-EYE camera pins 
    constexpr camera_config_t esp_eye = {
        .pin_pwdn       = -1,
        .pin_reset      = -1,
        .pin_xclk       =  4,
        .pin_sscb_sda   = 18,
        .pin_sscb_scl   = 23,

        .pin_d7         = 36,
        .pin_d6         = 37,
        .pin_d5         = 38,
        .pin_d4         = 39,
        .pin_d3         = 35,
        .pin_d2         = 14,
        .pin_d1         = 13,
        .pin_d0         = 34,
        .pin_vsync      =  5,
        .pin_href       = 27,
        .pin_pclk       = 25,
    };

    /// M5 stack camera pins
    constexpr camera_config_t m5stack_psram = {
        .pin_pwdn       = -1,
        .pin_reset      = 15,
        .pin_xclk       = 27,
        .pin_sscb_sda   = 25,
        .pin_sscb_scl   = 23,
        .pin_d7         = 19,
        .pin_d6         = 36,
        .pin_d5         = 18,
        .pin_d4         = 39,
        .pin_d3         =  5,
        .pin_d2         = 34,
        .pin_d1         = 35,
        .pin_d0         = 32,
        .pin_vsync      = 22,
        .pin_href       = 26,
        .pin_pclk       = 21,
    };

    /// M5 stack wide camera pins
    constexpr camera_config_t m5stack_wide = {
        .pin_pwdn       = -1,
        .pin_reset      = 15,
        .pin_xclk       = 27,
        .pin_sscb_sda   = 22,
        .pin_sscb_scl   = 23,
        .pin_d7         = 19,
        .pin_d6         = 36,
        .pin_d5         = 18,
        .pin_d4         = 39,
        .pin_d3         =  5,
        .pin_d2         = 34,
        .pin_d1         = 35,
        .pin_d0         = 32,
        .pin_vsync      = 25,
        .pin_href       = 26,
        .pin_pclk       = 21,
    };

    /// AI Thinker camera pins
    constexpr camera_config_t ai_thinker = {
        .pin_pwdn       = 32,
        .pin_reset      = -1,
        .pin_xclk       =  0,
        .pin_sscb_sda   = 26,
        .pin_sscb_scl   = 27,
        .pin_d7         = 35,
        .pin_d6         = 34,
        .pin_d5         = 39,
        .pin_d4         = 36,
        .pin_d3         = 21,
        .pin_d2         = 19,
        .pin_d1         = 18,
        .pin_d0         =  5,
        .pin_vsync      = 25,
        .pin_href       = 23,
        .pin_pclk       = 22,
    };
};