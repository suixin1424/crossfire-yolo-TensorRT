#include <hidboot.h>
#include <usbhub.h>
#include <Mouse.h>
#include <Wire.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
USB     Usb;
USBHub     Hub(&Usb);
HIDBoot < USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE > HidComposite(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

// Signed char can be between -128 to 127
int delta[2];
int negMax = -127;
int posMax = 127;

// Mouse
int lmb = 0;
int rmb = 0;
int mmb = 0;


class MouseRptParser : public MouseReportParser
{
  protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);
};

void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
  delta[0] = mi->dX;
  delta[1] = mi->dY;
};
void MouseRptParser::OnLeftButtonUp	(MOUSEINFO *mi)
{
  lmb = 0;
};
void MouseRptParser::OnLeftButtonDown	(MOUSEINFO *mi)
{
  lmb = 1;
};
void MouseRptParser::OnRightButtonUp	(MOUSEINFO *mi)
{
  rmb = 0;
};
void MouseRptParser::OnRightButtonDown	(MOUSEINFO *mi)
{
  rmb = 1;
};
void MouseRptParser::OnMiddleButtonUp	(MOUSEINFO *mi)
{
  mmb = 0;
};
void MouseRptParser::OnMiddleButtonDown	(MOUSEINFO *mi)
{
  mmb = 1;
};

MouseRptParser MousePrs;

void setup()
{
  Mouse.begin();
  Serial.begin( 115200 );
  Usb.Init();
  HidComposite.SetReportParser(1, &MousePrs);
  HidMouse.SetReportParser(0, &MousePrs);
}

void loop()
{
  delta[0] = 0;
  delta[1] = 0;
  Usb.Task();
  // Left Mouse
  if (lmb == 0){
    Mouse.release(MOUSE_LEFT);
  } else if (lmb == 1){
    Mouse.press(MOUSE_LEFT);
  }
  // Right Mouse
  if (rmb == 0){
    Mouse.release(MOUSE_RIGHT);
  } else if (rmb == 1){
    Mouse.press(MOUSE_RIGHT);
  }
  // Middle Mouse
  if (mmb == 0){
    Mouse.release(MOUSE_MIDDLE);
  } else if (mmb == 1){
    Mouse.press(MOUSE_MIDDLE);
  }

  if (Serial.available() > 0)
  {
    // Read Data
    String data = Serial.readStringUntil('x');

    // Gets demarcation between deltaX and DeltaY
    int ohHiMarc = data.indexOf(':');
    Serial.println(data);

    // DeltaX & DeltaY
    delta[0] = data.substring(0, ohHiMarc).toInt();
    delta[1] = data.substring(ohHiMarc + 1).toInt();

    handleX(delta[0]);
    handleY(delta[1]);
  } else{
    Mouse.move(delta[0], delta[1]);
  }
}

// Handle Moving of x 
void handleX(int dx){
  
  int spawns; 
  int remainder;
  
  if(dx < negMax)
  {
    // How many times we move mouse
    spawns = int(dx / negMax); 
    
    // How much we move after for loop
    remainder = int(dx % negMax);

    // Because we can only move 125 at a time,
    // we need a for loop to spawn multiple mouse events.
    for(int i = 0; i < spawns; i++)
    {
      Mouse.move(negMax , 0, 0);
    }
    // Move Remainder
    Mouse.move(remainder, 0, 0);
  } 
  else if (dx >= negMax && dx <= posMax)
  {
    Mouse.move(dx, 0, 0);
  }
  else if (dx > posMax)
  {
    // How many times we move mouse
    spawns = int(dx / posMax); 
    
    // How much we move after for loop
    remainder = int(dx % posMax);
    
    for(int i = 0; i < spawns; i++)
    {
      Mouse.move(posMax , 0, 0);
    }
    // Move Remainder
    Mouse.move(remainder, 0, 0);
  }
  
}

// Handle Moving of x 
void handleY(int dy){
  
  int spawns; 
  int remainder;
  // MindTrip, Neg is pos & Pos is Neg for move, hence the inverted pos & Neg
  if(dy < negMax)
  {
    // How many times we move mouse
    spawns = int(dy / negMax); 
    
    // How much we move after for loop. -1 converts to correct direction on arduino (pos,neg,neg,pos)
    remainder = int(dy % negMax);
    remainder *= -1;
    // Because we can only move 125 at a time,
    // we need a for loop to spawn multiple mouse events.
    for(int i = 0; i < spawns; i++)
    {
      Mouse.move(0, posMax, 0);
    }
    // Move Remainder
    Mouse.move(0, remainder, 0);
  } 
  else if (dy >= negMax && dy <= posMax)
  {
    dy *= -1;
    Mouse.move(0, dy, 0);
  }
  else if (dy > posMax)
  {
    // How many times we move mouse
    spawns = int(dy / posMax); 
    
    // How much we move after for loop
    remainder = int(dy % posMax);
    remainder *= -1;
    
    for(int i = 0; i < spawns; i++)
    {
      Mouse.move(0, negMax, 0);
    }
    // Move Remainder
    Mouse.move(0, remainder, 0);
  }
};