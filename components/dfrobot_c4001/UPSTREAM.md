# dfrobot_c4001 (fork)

Kopie komponentu z https://github.com/mikelawrence/esphome-component-dfrobot-c4001
(commit f0e12be055f5819d2b08a9b964bc8f48a86c145f), licence MIT (c) 2026 Big Mike, viz LICENSE.

Duvod forku: v rezimu SPEED_AND_DISTANCE nejde vypnout LED na modulu radaru (SEN0610 / C4001).

## Zmeny oproti upstreamu
- `getGpioMode`/`setGpioMode` se posila jen v rezimu PRESENCE. V SPEED_AND_DISTANCE na ne radar odpovi `Error`
  (3 opakovani + `Command Failure` pri kazdem startu a ulozeni). `out_led_enable` switch je v SPEED_AND_DISTANCE
  zakazan validaci.

## Overeno na zarizeni (SEN0610, SW JYSJ_01.00.04.040220)
- RUN LED (`setLedMode`) funguje v obou rezimech.
- OUT LED (`setGpioMode`) funguje jen v PRESENCE, v SPEED_AND_DISTANCE vraci `Error`.
- Mikropohyby (`setMicroMotion`/`getMicroMotion`) funguji jen v SPEED_AND_DISTANCE, v PRESENCE vraci `Error`
  (upstreamovy zakaz `micro_motion_enable` v PRESENCE je tedy spravne a zustava).
