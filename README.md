# freertos_counter_demo
A small test task

This app is developed for ESP32 targets. ESP-IDF framework should be usedto run and load it on the target.

Application contains 2 tasks under FreeRTOS. 
1st one runs continiously incrementing a counter each 5 seconds
2nd one receives the conter value using a queue and calls the function which logs the current output.

## Startup

- Configure esp-idf frwmework in accordance with [this manual](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/).
- Do ```. $HOME/esp/esp-idf/export.sh``` in the root directory of this project.
- Build and flash like the standarte esp-idf project. 
