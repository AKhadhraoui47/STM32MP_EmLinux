# STM32MP_EmLinux
This repo will cover my journey in working on STM32MP135 microprocessor as Embedded Linux intern @STMicrocoelectronics.

Through this comprehensive README we will be going through the main aspects of my work during this project, but first we need to introduce some key tools and useful concepts.  

## STM32MP135 MPU  

The STM32MP135 is a microprocessor (MPU) developed by STMicroelectronics, based on a single $\color{Aqua}{Arm}$ $\color{Aqua}{Cortex-A7}$ core that operates at speeds up to 1GHz. This MPU is designed for various applications, particularly in industrial settings, and supports **entry-level Linux**, **bare metal**, or **Real-Time Operating Systems (RTOS)**, including Microsoft Azure RTOS.  

### Key Features  
- **Architecture:** A single-core Cortex-A7 processor operating at speeds up to 1GHz.
- **Display and Camera Interfaces:** Equipped with a dedicated LCD-TFT parallel display interface and a 16-bit parallel camera interface.  
- **Connectivity:** Includes dual Ethernet ports, providing enhanced capabilities for networked applications.  
- **Security:**  Cryptographic accelerator, Memory protection, Code isolation for runtime protection. Supports OP-TEE (Open Portable Trusted Execution Environment) and TF-A (Trusted Firmware-A),  

This is the $\color{Aqua}{STM32MP135f-DK}$ board based on the $\color{Aqua}{MP135}$ that we will be working on, featuring user buttons, Wi‐Fi®, Bluetooth® Low Energy, a 2-megapixel CMOS camera module and an STLINK-V3E is integrated on the board.  
  
<p align="center">
<img src="https://www.st.com/content/ccc/fragment/product_related/rpn_information/recommended_cards_logos/group0/73/c4/13/8d/91/03/47/f9/STM32MP135F-DK/files/stm32mp135f-dk-recommended-card.png/jcr:content/translations/en.stm32mp135f-dk-recommended-card.png" />
</p>
  
<sub>To check the full functionnalities and features provided by this board check the [user-manual](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.st.com/resource/en/user_manual/um2993-discovery-kit-with-1-ghz-stm32mp135fa-mpu-stmicroelectronics.pdf&ved=2ahUKEwioi5n7ybWIAxXl_7sIHfkCOLwQFnoECAoQAQ&usg=AOvVaw2CwRJAdiHNoX7tqw4e1NXW). </sub>  



