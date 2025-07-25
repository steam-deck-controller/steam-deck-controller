# OSDC - Open Steam Deck Controller (Name TBD)!

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
- [ ] Create PCB and firmware for 2.4ghz wireless capablility
- [ ] TBD

### 🤔 Why this approach?

- **Driver-Free Operation**: The USB dongle will emulate the original USB descriptors, allowing the controller to work without additional drivers.
- **No Additional Software**: The controller will work out-of-the-box via Steam's Controller support. It should also work without steam on linux based system as the linux kernel has inbuilt drivers for steamdeck controllers.
- **No Hardware Modification**: The only required pcb doesnt require any modifications to the pcbs or firmware on the boards, this makes the project more accessable even if you know nothing about electrionics. It also opens the possibility of us to sell the usbc preassembled for people to create there own.

## 🛠 Parts

- PCB
  - See electronics

- Controller Hardware (Needs to be removed from a preferably broken lcd steamdeck, some parts can be substituted for third party components)
  - All Face Buttons (ABXY, Dpad, View, Menu, Steam, (...),)
  - Triggers And Bumpers
  - Thumbstick Modules (Can use elecgear parts, they sell analog or hall effect. Elecgear is recomended because gilikit has a square deadzone)
  - Back Buttons
  - Trackpad Modules Assemblies
  - Action Button and D-Pad Brackets and Boards
  - Left and Right Controller Daughterboards
    
## 📝 License & Contributions

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
Pull requests are welcome, acceptance is at the sole discression of the OSDC Team.


## 🙏 Acknowledgments

- Steam Deck Controller Team
  - [Roni1993](https://github.com/Roni1993)
  - [Alia5](https://github.com/Alia5)
  - [oaSysFox](https://github.com/oaSysFox)
  - [Kamp](https://github.com/mriankamp)
  - [Instagalactix](https://github.com/instagalactix)
    
- [Valve Corporation](https://www.valvesoftware.com/) - For creating the Steam Deck, the original Steam Controller and Steam Input
- [SlickBuys](slickbuysmodsandrepairs.com/) - For providing parts

## Legal Attributions:
This is a community project not affiliated with Valve Corporation. 
There is always a risk of destroying parts while dissasembling the deck, if you arent comfortable taking this risk please do not partake in this project. 
Modification of parts can possibly void warranty. 
OSDC Team takes no responbility for broken parts or voided warrenty as a result of partaking in this project.

- ©2025 Valve Corporation. Steam and the Steam logo are trademarks and/or registered trademarks of Valve Corporation in the U.S. and/or other countries.
