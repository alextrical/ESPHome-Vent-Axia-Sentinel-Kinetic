[![latest version](https://img.shields.io/github/release/alextrical/ESPHome-Vent-Axia-Sentinel-Kinetic?display_name=tag&include_prereleases&label=latest%20version)](https://github.com/alextrical/ESPHome-Vent-Axia-Sentinel-Kinetic/releases)
# ESPHome-Vent-Axia-Sentinel-Kinetic
<a href="https://esphome.io/">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="https://esphome.io/_static/logo-text-on-dark.svg", alt="ESPHome Logo">
    <img src="https://esphome.io/_static/logo-text-on-light.svg" alt="ESPHome Logo">
  </picture>
</a>

## 💬 Feedback & Support

🐞 Found a bug? Let us know via an [Issue](https://github.com/alextrical/ESPHome-Vent-Axia-Sentinel-Kinetic/issues).  
💡 Have a question or suggestion? Join the [Discussion](https://github.com/alextrical/ESPHome-Vent-Axia-Sentinel-Kinetic/discussions)!


# [Vent Axia Sentinel Kinetic](/components/Vent-Axia-Sentinel-Kinetic)
- Vent Axia Sentinel Kinetic is an extended component of ESPHome UART that enables easy integration of a Vent Axia Sentinel Kinetic by emulating a Remote Keypad and utilizing UART data.

## Developing, Testing & Debugging
Create/activate environment by running from project root:
```bash
source scripts/setup_build_env.sh
```

To Build ESP32 firmware on your local machine for running Wokwi:
```bash
esphome compile config/VASK-ESP32.yaml
```
Once copiled run in Wokwi VSCode https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode