# Arduino_Music_Piano_Synthesizer

Project for IEEE hackathon

## Description
This piano systhesizer is controlled by 8 buttons with 256 combos  but only output with one cable.
- All combos can be programmable with music nodes. 
- Used r2r ladder DAC in order to minimize inputs used (only 1 input), make programming easier and make it possible to detect key pressed without any delay. (No need for shift register nor 8 individual Digital Input but one analog input).

In addition, 2 potentiometers are used for controlling upper and lower frequency bounds. Meanwhile, 2 black buttons(software enabled latch system) function as record and play button. The wireless control module on right side is used for controlling pitch, delay, and MUTE during playing.

Hardwares Used:
- 1. nRF24L01+ 2.4GHz wireless module x 2
- 2. Gyro. + Acc. module
- 3. 2 x Arduino UNO
- 4. 1k resistors and other resistors
- 5. LEDs
- 6. push buttons
- 7. wires
- 8. speaker
- 9. battery packs x 2

##GUI Photos of hardware
![alt tag](https://github.com/JXproject/Arduino_Music_Piano_Synthesizer/blob/master/Pictures/IMG_7909.JPG)
![alt tag](https://github.com/JXproject/Arduino_Music_Piano_Synthesizer/blob/master/Pictures/IMG_7930.JPGs)

##WIN 2nd Place for IEEE hackathon
![alt tag](https://github.com/JXproject/Arduino_Music_Piano_Synthesizer/blob/master/Pictures/IMG_7933.JPG)
![alt tag](https://github.com/JXproject/Arduino_Music_Piano_Synthesizer/blob/master/Pictures/IMG_7936.JPG)
![alt tag](https://github.com/JXproject/Arduino_Music_Piano_Synthesizer/blob/master/Pictures/IMG_7929.JPG)
