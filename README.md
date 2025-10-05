# The Tarnished's Ledger

An Elden Ring-themed todo list app for the LilyGO T5-4.7" E-Paper display that I made for my best friend who is obsessed with everything Elden Ring.
<p align="center">
  <img src="./images/webInterface.png"/>
  <img src="./images/emptyLedger.png"/>
  <img src="./images/fullLedger.png"/>
</p>

## You'll need
The **LilyGO T5-4.7" E-Paper ESP32-S3 Board**.
PlatformIO VS Code extension.
LilyGO-EPD47-esp32s3 library.

## Installation

### 1. Install PlatformIO extension.
### 2. Download the LilyGo EPD47 library, I used this one:

```
https://github.com/Xinyuan-LilyGO/LilyGo-EPD47
```

### 3. Remove the `boards` folder from the lib.
### 4. Run `pio run` in the project directory to download and install all required dependencies.
### 5. Update WiFi Credentials.
Create a new `env.h` file, copy the content of `env.h.example` and change WiFi configuration respectively.
### 6. Build and upload
Connect your LilyGO board and click the Upload button (→) in the PlatformIO toolbar at the bottom of VS Code.
PlatformIO will compile and upload automatically.

## Usage

After uploading, open the Serial Monitor and wait for the board to connect to WiFi.
Note the IP address displayed.

Open browser on any device connected to the same WiFi network.
Navigate to the IP address shown in Serial Monitor and you should see "The Tarnished's Ledger" interface.

The display automatically updates when you add, complete, or delete tasks.