# Steam Deck Standalone Controller

<img src="./media/preview.png" width="250" align="right">

Convert your spare Steam Deck parts into a fully functional standalone controller! This project aims to repurpose original Steam Deck components into a custom-designed handheld controller.

This project is very much in its early stages, so please be patient as we work on developing the controller.  
We will be updating this README with more information as we progress.

## 🎯 Project Goals

- Create a standalone controller using original Steam Deck (LCD-Deck) parts
- Design and provide 3D printable shell (hopefully compatible with resin / FDM printers)
- Provide possibility for a basic wired-only controller build
- Implement wireless communication capabilities (TBD)

Additions like a chatpad, additional buttons, or other features are currently not the focus of the project  
as that would introduce additional complexity and require more custom parts.

Once wireless communication is implemented, we are open to exploring additional features, however.

## ✅ Current Progress

- [x] USB pinout reverse engineering completed ([details](./research/Controller_Boards.md))
- [x] Basic wired proof-of-concept showing the possibility ([demo video](https://youtu.be/daWLg8Y8ThU))
- [ ] Design FFC to USB-C Breakout PCB
  - For development purposes
  - [Optional] for wired-only controller builds
- [ ] Design 3D printable shell prototype (_Resin / FDM / SLS is TDB_)
- [ ] Develop firmware for wireless communication
- [ ] Create detailed assembly guide
- [ ] Design and test power management system
- [ ] TBD

## 🛜🔋 Wireless Approach

Our current approach for wireless functionality involves:

1. Custom MCU Board Design:
   - Acts as a USB-Host to the controller
   - Handles power management
   - Processes and forwards HID reports wirelessly

2. Custom USB Dongle:
   - Receives wireless signals from the controller
   - Emulates the original USB descriptors
   - Forwards HID reports to the computer

### 🤔 Why this approach?

- **Driver-Free Operation**: The USB dongle will emulate the original USB descriptors, allowing the controller to work without additional drivers.
- **No Additional Software**: The controller will work out-of-the-box via Steam.
- **No Firmware Modification**: The controller will work with the original Steam Deck firmware.

### ⚙️ Technical Considerations

- MCU chip selection
- Wireless protocol (considering 2.4GHz proprietary vs Bluetooth LE)
- Battery management

## 🛠️ Getting Started

[Coming Soon]

## 🗂️ Repository Structure

```dir
├── 3d-models/
├── pcb-designs/
│   └── ffc-usb-breakout/
├── firmware/
│   ├── controller-mcu/
│   └── dongle-mcu/
├── research/
└── media/
```

## 📋 Requirements

- Original Steam Deck controller parts (specific list coming soon)
- [Additional requirements TBD]

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### 💬 Discord

Join our Discord server to discuss the project, ask questions, or share your ideas!  
[![Discord](https://img.shields.io/discord/1284933328470016031?color=7289DA&label=Discord&logo=discord&logoColor=white)](https://discord.gg/rduBamsN49)

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Steam Deck Controller Team
  - [Roni1993](https://github.com/Roni1993)
  - [Alia5](https://github.com/Alia5)
  - [oaSysFox](https://github.com/oaSysFox)
  - [Additional Members here]
- [Valve Corporation](https://www.valvesoftware.com/) - For creating the Steam Deck, the original Steam Controller and Steam Input
- [SlickBuys](slickbuysmodsandrepairs.com/) - For providing parts

## ⚠️ Disclaimer

This is a community project not affiliated with Valve Corporation. Use at your own risk.