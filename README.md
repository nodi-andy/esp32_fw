
# Grbl (CNC Controller) For ESP32

- **Motors**
  - Control up to 6 coordinated axes (XYZABC)
  - Each axis can have 1 or 2 motors each for a total of 12 motors
  - Dual motors axes can optionally auto square using a home switch and independent control for each motor.
  - Motor drivers can be dynamically assigned to axes, so a 4 motor XYZA controller could be independent to a XYYZ (dual motor Y axis) without any hardware changes.
  - Step rates up to 120,000 per/second.
  - Trinamic (SPI controlled) stepper motors are supported including StealthChop, CoolStep and StallGuard modes. Sensorless homing can be used.
  - Unipolar stepper motor can be directly driven
  - RC hobby servos can be used as coordinated motors with acceleration and speed control. 
- **Peripherals**
  - Limit/Homing Switches with debouncing
  - User input buttons (hold, resume, reset)
  - Coolant control (Mist, Flood)
  - Z Probe (any axis)
  - Safety Door (open door safely retracts and stops spindle, can be resumed)
  - Additional I/O via shift register and I/O expanders supported (on dev branches)
- **Job Control**
  - Instant feed hold and resume
  - Feed rate override
  - Spindle speed override
- **Spindles**
  - PWM
  - RS485 Modus
  - DAC (analog voltage) 0-10V
  - Relay Based
  - RC type Brushless DC motors using low cost BESCs
  - Laser PWM with power/speed compensation
  - Easy to create custom spindles
- **Connectivity**
  - USB/Serial
  - Bluetooth/Serial Creates a virtual serial port on your phone or PC. Standard serial port applications can use Bluetooth.
  - WIFI
    - Creates its own access point or connects to yours.
    - Built in web server. The server has full featured CNC control app that will run on your phone or PC in a browser. No app required.
    - Telnet sending of gcode
    - Push notifications (like...job done, get a text/email)
    - OTA (over the air) firmware upgrades.
- SD card. Gcode can be loaded and run via WIFI.
- **Compatibility** 
  - Grbl_ESP32 is fully backward compatible with Grbl and can use all gcode senders.
- **Customizable**
  - Easy to map pins to any functions.  
  - Custom machines can be designed without touching the main code.   
  - Custom initialization
    - Kinematics
    - Custom homing
    - Tool changer sequences
    - Button macros (run gcode sequence, etc.)
    - Custom end of Job sequence
    - RTOS Real time operating system allows background monitoring and  control without affecting motion control performance
