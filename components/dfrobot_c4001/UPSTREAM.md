# dfrobot_c4001 (fork)

Kopie komponentu z https://github.com/mikelawrence/esphome-component-dfrobot-c4001
(commit f0e12be055f5819d2b08a9b964bc8f48a86c145f), licence MIT (c) 2026 Big Mike, viz LICENSE.

Duvod forku: v rezimu SPEED_AND_DISTANCE nejde vypnout LED na modulu radaru (SEN0610 / C4001).

## Zmeny oproti upstreamu
- `micro_motion_enable` switch je povolen i v rezimu PRESENCE (upstream ho tam zakazuje); get/setMicroMotion se posila v obou rezimech. Jestli to radar v PRESENCE opravdu provede, je potreba overit na zarizeni.
- `getGpioMode`/`setGpioMode` se posila jen v PRESENCE (radar na ne v SPEED_AND_DISTANCE odpovi Error); `out_led_enable` switch je v SPEED_AND_DISTANCE zakazan validaci.
