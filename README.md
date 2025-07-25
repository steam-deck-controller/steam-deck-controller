# Open Steam Deck Controller (Name TBD)!

<img src="./media/Preview.jpg" width="250" align="right">

Convert your spare Steam Deck parts into a fully functional standalone controller! This project aims to repurpose original Steam Deck components into a custom-designed handheld controller.

For the time being, this controller is wired only. A full wireless solution may come at some point in the future!

## 🎯 Project Goals

- Create a standalone controller using original Steam Deck (LCD-Deck) parts
- Design and provide 3D printable shell (hopefully compatible with resin / FDM printers)
- Provide possibility for a basic wired-only controller build
- Create a wireless version, which the would substitute the currect ffc to usb-c breakout.

## ✅ Current Progress

- [x] USB pinout reverse engineering completed ([details](./research/Controller_Boards.md))
- [x] Basic wired proof-of-concept showing the possibility ([demo video](https://youtu.be/daWLg8Y8ThU))
- [x] Design FFC to USB-C Breakout PCB
  - For development purposes
  - [Optional] for wired-only controller builds
- [x] Design 3D printable shell prototype (_Resin / FDM / SLS is TDB_)
- [x] Add BOM for Electronics and Steamdeck parts
- [ ] Add levers for rear button functionality
- [ ] Create detailed assembly guide
- [ ] TBD

### 🤔 Why this approach?

- **Driver-Free Operation**: The USB dongle will emulate the original USB descriptors, allowing the controller to work without additional drivers.
- **No Additional Software**: The controller will work out-of-the-box via Steam.
- **No Firmware Modification**: The controller will work with the original Steam Deck firmware.
- **No Hardware Modification**: The only required pcb doesnt require any modifications to the pcbs, this makes the project more accessable even if you know nothing about electrionics. It also opens the possibility of us to sell the usbc preassembled for people to create there own.

## 🛠️ Getting Started

- BOM: PCB
  - See electronics

- BOM: SteamDeck Parts
  - All Face Buttons (ABXY, Dpad, View, Menu, Steam, QA)
  - R1/2/4/5 and L1/2/4/5 Buttons
  - Thumbsticks and Thumbstick Boards
  - Trackpad Assemblies
  - Facebutton Brackets and Flexes
  - Left and Right Controller Daughterboards


[Coming Soon]

## 📋 Requirements

- Original Steam Deck controller parts (specific list coming soon)
- [Additional requirements TBD]

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Steam Deck Controller Team
  - [Roni1993](https://github.com/Roni1993)
  - [Alia5](https://github.com/Alia5)
  - [oaSysFox](https://github.com/oaSysFox)
  - [Kamp](https://github.com/mriankamp)
  - [Instagalactix](https://github.com/instagalactix)
    
- [Valve Corporation](https://www.valvesoftware.com/) - For creating the Steam Deck, the original Steam Controller and Steam Input
- [SlickBuys](slickbuysmodsandrepairs.com/) - For providing parts

## ⚠️ Disclaimers

This is a community project not affiliated with Valve Corporation. Use at your own risk.

## Legal Attributions:
- ©2025 Valve Corporation. Steam and the Steam logo are trademarks and/or registered trademarks of Valve Corporation in the U.S. and/or other countries.
