/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  

#include <gui/video_player_screen/VideoBottomBar.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

VideoBottomBar::VideoBottomBar() : BottomBar()
{
    onOffBuf[0] = 0;

    // repeat button
    repeatButton.setBitmaps(Bitmap(BITMAP_LOOP_ID), Bitmap(BITMAP_LOOP_ACTIVE_ID));
    repeatButton.setAction(buttonCallback);
    repeatButton.setXY(getWidth() - (20 + 64 * 3), 0);

    //Lower left on/off button
    hwJPEGButton.setBitmaps(Bitmap(BITMAP_TOGGLE_OFF_ID), Bitmap(BITMAP_TOGGLE_ON_ID));
    hwJPEGButton.setXY(0, 0);
    hwJPEGButton.setAction(buttonCallback);
    hwJPEGButton.forceState(true);
    
    //status text
    hwJPEGText.setPosition(hwJPEGButton.getWidth(), 20, 300, background.getHeight() - 20);
    hwJPEGText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    hwJPEGText.setWildcard(onOffBuf);
    hwJPEGText.setTypedText(TypedText(T_MOVIE_HW_HEADLINE));
    Unicode::snprintf(onOffBuf, ON_OFF_LENGTH, "On");

    add(repeatButton);
    add(hwJPEGButton);
    add(hwJPEGText);
}

void VideoBottomBar::buttonClicked(const AbstractButton& btn)
{
    if (&btn == &repeatButton)
    {
        const bool state = repeatButton.getState();
        if (actionCallback) //call external callback
        {
            actionCallback->execute(state ? REPEAT_ON : REPEAT_OFF);
        }
    }
    else if (&btn == &hwJPEGButton)
    {
        const bool state = hwJPEGButton.getState();
        if (actionCallback) //call external callback
        {
            actionCallback->execute(state ? HW_ON : HW_OFF);
        }
        Unicode::snprintf(onOffBuf, ON_OFF_LENGTH, state ? "On" : "Off");
        hwJPEGText.invalidate();
    }
    else
    {
        BottomBar::buttonClicked(btn);
    }
}
