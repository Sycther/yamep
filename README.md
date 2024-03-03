# YAMEP (Yet Another Multi Effects Pedal)

This project aims to create a digital menu system for a multi effects pedal.
Each pedal is an analog component that will be controlled via a digital potentiometer. 
 
# PEDALS:
- Tremelo
    - Depth 500k
    - Volume 20k
    - Tone 20K
- Delay
    - Sustain 10k
    - Tone 10K
    - Volume 10K
- Fuzz
    - Sustain 10k
    - Tone 10k
    - Volume 10k

# Menu Interface

Dials : Upper, Lower, 3 x Item
Buttons: Back(Home)

# Pinout

- 1. 
- 2. ! 
- 3. 
- 4. 
- 5. ! Back/Home Button
- 12. ! 
- 13. 
- 14. Lower Dial - Select
- 15. !
- 16. Lower Dial - RE: DT
- 17. Lower Dial - RE: CLK
- 18. 
- 19. 
- 21. I2C - SDA
- 22. I2C - SCL
- 23. 
- 25. 
- 26. 
- 27. 
- 32. 
- 33. 
- 34. 
- 35. 
- 36. 
- 39. 

# PCF(x38) Rotary Encoders

- 0. Item Dial 1 - RE: CLK
- 1. Item Dial 1 - RE: DT
- 2. Item Dial 2 - RE: CLK
- 3. Item Dial 2 - RE: DT
- 4. Item Dial 3 - RE: CLK
- 5. Item Dial 3 - RE: DT
- 6. Page Dial - RE: DT
- 7. Page Dial - RE: CLK

# PCF(x3D) Buttons

- 0. Item Dial 1 - Select
- 1. Item Dial 2 - Select
- 2. Item Dial 3 - Select
- 3. Page Dial - Select
- 4. 
- 5. 
- 6. 
- 7. 

# TODO:
- [x] BIND CONFIGS TO MIN?MAX
- [ ] PRESET PAGE SET VALUES BEFORE DRAWING
- [x] - When selecting second preset, pedals appear to be out of range
- [X] - Configs active levels need to be set to presetlevels
- [ ] KEEP COPY OF PRESET/PEDAL IN "PedalPage"
- [ ] - copy values back to menu when saving
- [ ] Save Menu Configs
- [x] Read Menu Configs
- [X] Save Menu State
- [X] - Go to last preset loaded on bootup
- [X] PRESET MODIFICATIONS
- [ ] - Naming
- [ ] - Moving
- [ ] - Deleting
- [ ] - Add Preset OR Set number of Presets


