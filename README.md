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
<img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQrXezzB2UCQcziE8tUWDF5VpFNpgzMJXP96w&s"/>
</p>
  
<sub>To check the full functionnalities and features provided by this board check the [user-manual](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://www.st.com/resource/en/user_manual/um2993-discovery-kit-with-1-ghz-stm32mp135fa-mpu-stmicroelectronics.pdf&ved=2ahUKEwioi5n7ybWIAxXl_7sIHfkCOLwQFnoECAoQAQ&usg=AOvVaw2CwRJAdiHNoX7tqw4e1NXW). </sub>  

To be able to manipulate and develop applications on this MPU a Linux Distribution is provided; $\color{Aqua}{OpenSTLinux}$.  

## OpenSTLinux  

The OpenSTLinux package is a Linux distribution specifically designed for STM32MPU microprocessors, based on the **[Yocto Project](https://github.com/AKhadhraoui47/Yocto_Rpi_IMU?tab=readme-ov-file#yocto-project)** utilizing OpenEmbedded framework. To ease-up the learning process this distribution comes in different software Packages:  

- **$\color{Aqua}{Starter Package}$** Provides essential tools and basic environment for initial development. <sub>[Starter Pckg](https://wiki.st.com/stm32mpu/wiki/STM32MP13_Discovery_kits_-_Starter_Package)</sub>  

- **$\color{Aqua}{Developer Package}$** Offers additional tools and libraries for more advanced development. <sub>[Developer Pckg](https://wiki.st.com/stm32mpu/wiki/STM32MPU_Developer_Package)</sub>  

- **$\color{Aqua}{Distribution Package}$** A comprehensive package that includes all necessary components for building and deploying applications. <sub>[Distribution Pckg](https://wiki.st.com/stm32mpu/wiki/STM32MPU_Distribution_Package)</sub>  

### Key Features:  
 
- **Board Support Package (BSP)**: It includes a BSP that integrates essential components such as:

    - **OP-TEE**: A secure operating system that running in a trusted execution environment.
    - **TF-A** (Trusted Firmware-A): Manages the boot process and transitions between secure and non-secure environments.
    - **Linux Kernel**: Runs in non-secure mode, providing the primary operating system functionalities.

- **Application Frameworks**: various application frameworks, including:  

    - **Wayland-Weston**: For graphical user interfaces.  
    - **GStreamer**: For multimedia processing.  
    - **ALSA** (Advanced Linux Sound Architecture): For audio functionalities.




