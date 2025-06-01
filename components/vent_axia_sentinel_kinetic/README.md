## vent_axia_sentinel_kinetic
```
external_components:
  - source: github://alextrical/ESPHome-Vent-Axia-Sentinel-Kinetic@latest
    components: [ vent_axia_sentinel_kinetic ]
    refresh: always
```

## vent_axia_sentinel_kinetic.switch
```
<!-- packet on) 0x02 0x01 0x02 0x03 0x01 (add)checksum 0x0D 0x0A
   offset) head head 0    1    2
packet on ack) 0x02 0x01 0x02 0x13 0x01 (add)checksum 0x0D 0x0A
packet off) 0x02 0x01 0x02 0x03 0x00 (add)checksum 0x0D 0x0A
packet off ack) 0x02 0x01 0x02 0x03 0x00 (add)checksum 0x0D 0x0A -->

switch:
  - platform: vent_axia_sentinel_kinetic
    up:
      name: "Up Switch"
      id: sw_up
    down:
      name: "Down Switch"
      id: sw_down
    set:
      name: "Set Switch"
      id: sw_set
    main:
      name: "Main Switch"
      id: sw_main
```
<hr/>


## vent_axia_sentinel_kinetic.text_sensor
```
vent_axia_sentinel_kinetic:
  - id: vask
    uart_id: vask_uart
    line1:
      id: line1
      name: "Line 1"
    line2:
      id: line2
      name: "Line 2"
    diagnostic0:
      name: "Diagnostic 0"
      entity_category: "diagnostic"
    diagnostic1:
      name: "Diagnostic 1"
      entity_category: "diagnostic"
    diagnostic2:
      name: "Diagnostic 2"
      entity_category: "diagnostic"
    diagnostic3:
      name: "Diagnostic 3"
      entity_category: "diagnostic"
    diagnostic4:
      name: "Diagnostic 4"
      entity_category: "diagnostic"
    diagnostic5:
      name: "Diagnostic 5"
      entity_category: "diagnostic"
    diagnostic6:
      id: diagnostic6
    diagnostic7:
      id: diagnostic7
    diagnostic8:
      id: diagnostic8
    diagnostic9:
      id: diagnostic9
    diagnostic10:
      id: diagnostic10
    diagnostic11:
      id: diagnostic11
    diagnostic12:
      id: diagnostic12
    diagnostic13:
      id: diagnostic13
    diagnostic14:
      id: diagnostic14
    diagnostic15:
      id: diagnostic15
    diagnostic16:
      id: diagnostic16
    diagnostic17:
      id: diagnostic17
    diagnostic18:
      id: diagnostic18
    diagnostic19:
      id: diagnostic19
    diagnostic20:
      id: diagnostic20
    diagnostic21:
      id: diagnostic21
    diagnostic22:
      id: diagnostic22
    diagnostic23:
      id: diagnostic23
    diagnostic24:
      id: diagnostic24
    diagnostic25:
      id: diagnostic25
    diagnostic26:
      id: diagnostic26
    diagnostic27:
      id: diagnostic27
    diagnostic28:
      id: diagnostic28
```
<hr/>
