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

## OpenSTLinux🥚🥚كيفية طبخ الكيرنل بدون بيض

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

## Say Cheez 📸  

We will take our first image, or build our first image i should say. First of we will follow the steps provided in this [Distribution Pckg](https://wiki.st.com/stm32mpu/wiki/STM32MPU_Distribution_Package) to setup our environment.  

> If you're not familiar with **OpenSTLinux** distribution i recommend starting with the [Starter Pckg](https://wiki.st.com/stm32mpu/wiki/STM32MP13_Discovery_kits_-_Starter_Package). Note that to deal efficiently with the distribution package, basic knowledge of **Yocto** is required, check this [repo](https://github.com/AKhadhraoui47/Yocto_Rpi_IMU?tab=readme-ov-file#yocto-project) where ressources are provided.  

After setting-up your workspace let's setup our build environment. A provided script in the package will do the work for us, we just need to source it with some parameters.  

```console
ak47@ak47:$ DISTRO=openstlinux-weston MACHINE=stm32mp13-disco IMAGE=st-image-core  source layers/meta-st/scripts/envsetup.sh build-dir-name
```
> Parameters are of my choice according to the hardware available to test everything is OK. Full list of provided parameters is [here](https://wiki.st.com/stm32mpu/wiki/OpenSTLinux_distribution).  

> Mentionning the **build-dir-name** when sourcing the setup file is recommended to avoid any errors caused by path strings length.  

Sourcing the file will take us automatically to the build directory where we launch our build process with **bitbake**.  
```console
ak47@ak47:$ bitbake <image>
```
> In case we didn't mention the **IMAGE** parameter when sourcing our file we will have to mention it when launching the build.  

Once the build is complete, we will flash our **SD-Card**. Two ways will be covered; Our device in DFU Mode through USB OTG port or Flash the image directly to SD-Card from host machine but first let's check some key files under **DIR=build-dir/tmp-glibc/deploy/images/machine/**  

```
DIR --- arm-trusted-firmware/
    --- kernel/ 
    --- optee/
    --- fip/
    --- flashlayout_<IMAGE>/
    --- scripts/ 
```  

These directories contain the binaries of every element of our image, essential for a correct booting. You can check the [boot sequence](https://wiki.st.com/stm32mpu/wiki/Boot_chain_overview#STM32MP13_boot_chain) for the MP13 series.  

> **scripts/** will be covered later. **flashlayout_<IMAGE>/** is used to locate binaries and set the partionning provided by **tsv** file used by **STM32CubeProgrammer**.

### DFU Mode  

To flash our image with our device set in DFU Mode we will need the right combination. An explanatory [table](https://wiki.st.com/stm32mpu/wiki/STM32MP135x-DK_-_hardware_description#Boot_related_switches) is provided. <sub>Forced USB/UART boot
for programming</sub>  

Now let's flash the image using **STM32CubeProgrammer**. First let's check if our device is detected:  

```console
ak47@ak47:$ STM32_Programmer_CLI -l usb
Total number of available STM32 device in DFU mode: 1

  Device Index           : USB1
  USB Bus Number         : 
  USB Address Number     : 
  ...
```  

Now let's pass the **STM32CubeProgrammer** our device and the **tsv** file to flash our image on to the **SD-Card**.  

```console
ak47@ak47:$ STM32_Programmer_CLI -c port=usb1 -w flashlayout_<IMAGE>/extensible/FlashLayout_sdcard_<>.tsv
 
 USB speed   : High Speed (480MBit/s)
 Manuf. ID   : STMicroelectronics
 Product ID  : DFU in HS Mode @Device ID /0x501, @Revision ID /0x1001
 ...
```  

> Common error may occur due to wrong directories in **tsv** file. You may need to move up **two levels** example: ***fip/ -> ../../fip/***

If everything went ok you should see a message on your terminal.  

```console
Start operation done successfully at partition 0x13
Flashing service completed successfully
```  

### Flashing directly with DD 💾

Now let's get back to the **scripts/** folder we mentionned earlier. This folder provides a script that can be used to generate a raw image to be flashed directly to the **micro SD-Car** using the ***disk/data duplicator*** command **(dd)**. Let's change dirctory to the **scripts/** folder then:  

```console
ak47@ak47:$ ./create_sdcard_from_flashlayout.sh ../flashlayout_<IMAGE>/FlashLayout_sdcard_<>.tsv 
```  
This script will create a complete raw image using the partitions and binaries' paths mentionned in the **tsv** file.  

```console
ak47@ak47:$ sudo umount /dev/mmcblkX
```
> Removing the card without unmounting it can result in data loss.  

```console
ak47@ak47:$ sudo dd if=../flashlayout_<>_FlashLayout_sdcard_<>.raw of=/dev/mmcblkX bs=8M conv=fdatasync status=progress
```

And there it is enjoy image is flashed, enjoy the fruit of your work. But to really make it enjoyable will need a powerful to let us **Minicom**.  

Minicom is a popular text-based terminal emulator program primarily used in Unix-like operating systems like Linux. It allows users to communicate with external devices connected via serial ports.

So connect the **STLINK** to your USB port, locate the corresponding **tty** interface under **/dev**. Set the switches so your board boots from **SD-Card** then launch **minicom** on your **PC**.  

```console
ak47@ak47:$ sudo apt install minicom
ak47@ak47:$ minicom -D /dev/ttyACMx
```

Now that we got familiar with our tools and environment let's get to the real deal.  

## Project's Context 💡

Imagine this scenario; You're working on a custom product based on the $\color{Aqua}{STM32MP135}$ $\color{Aqua}{Microprocessor}$. For wireless communication over **Wi-Fi** your custom will be equipped with the [Grove Wifi v2](https://wiki.seeedstudio.com/Grove-UART_Wifi_V2/) from [@seeedstudio](https://github.com/Seeed-Studio). And to easily interact with the module we will develop its own **Kernel Module** and a **Command Line Interface** and make an **in-tree** device.  

- **Key Steps**:
    - 
    **1. Modify the DT: Enable the UART instance connected to the module**   
    **2. Modify the DT: Make the module an in-tree device**   
    **3. Develop the Kernel Module**  
    **4. Develop the Command Line Interface**  

### Enabling UART instance ☑️

Let's understand the data structure we will be modifying which is the $\color{Aqua}{Device}$ $\color{Aqua}{Tree}$. It is a data structure in embedded systems that tells the operating system about the hardware components and their configuration. Similar to how a BIOS provides essential hardware information at boot, the DT helps the OS manage hardware without hardcoding specifics, offering more flexibility across platforms.

The UART instance i am working on is the UART8 instance initially **disabled**. That's why we will be applying a patch.   

**1. Copy the DT device_tree.dts we will be modifying to a tmp/ directory**  
**2. Copy in the same tmp/ device_tree.dts for example device_tree.dts.ref for reference**  
**3. Modify the device_tree.dts to enable UART8**   
**4. Generate the patch and copy to the folder in your custom layer**  

```console
ak47@ak47:$ cp build-mp135/tmp-glibc/work-shared/<machine>/kernel-source/arch/arm/boot/dts/stm32mp135f-dk.dts ~/tmp/stm32mp135f-dk.dts
ak47@ak47:~/tmp/$ cp stm32mp135f-dk.dts stm32mp135f-dk.dts.ref
ak47@ak47:~/tmp/$ nano stm32mp135f-dk.dts 

&uart8 {
	pinctrl-names = "default", "sleep", "idle";
	pinctrl-0 = <&uart8_pins_a>;
	pinctrl-1 = <&uart8_sleep_pins_a>;
	pinctrl-2 = <&uart8_idle_pins_a>;
	/delete-property/dmas;
	/delete-property/dma-names;
	//status = "disabled";
    status = "okay";
}
```
After modifying our file we will use **git** to compare and generate a file marking all differences in a **patch** file that will be applied in order to avoid modifying the original file  

```console
ak47@ak47:~/tmp/$ git diff --no-index stm32mp135f-dk.dts stm32mp135f-dk.dts.ref > 0001-Patch-File.patch
```
And there it is our patch file will look something like this [patch file](meta-custom/recipes-kernel/linux/stm32mp1/0001-enable-uart8.patch)  

> The mentionned files in the **patch** file will be directing to **tmp/** folder change that to:   
*a/arch/arm/boot/dts/stm32mp135f-dk.dts*  
*b/arch/arm/boot/dts/stm32mp135f-dk.dts*  

Now move your patch to your custom layer. Check this repo for more informations about [layers manipulation](https://github.com/AKhadhraoui47/Yocto_Rpi_IMU?tab=readme-ov-file#project-study).  

### Include the grove Module in DT  

Same process as done to enable the UART8 instance will be repeated but the content is different. To tell the kernel that our [Grove Wifi v2](https://wiki.seeedstudio.com/Grove-UART_Wifi_V2/) is wired to UART8 we need to introduce the device as a **child-node** of the UART8 node.  

```
&uart8 {
	pinctrl-names = "default", "sleep", "idle";
	pinctrl-0 = <&uart8_pins_a>;
	pinctrl-1 = <&uart8_sleep_pins_a>;
	pinctrl-2 = <&uart8_idle_pins_a>;
	/delete-property/dmas;
	/delete-property/dma-names;
	status = "okay";
	current-speed = <115200>;

	grovewifi: grovewifiv2 {
		compatible = "seeedstudio,grovewifiv2";
		status = "okay";
	};
};
```
So our final device-tree will look something like this [one](build-mp13/stm32mp135f-dk.dts).

> The **compatible** binding is used to make correspondance between the device and its **kernel module** which we will see later

### Recipe For Patches  

A simple **bbappend** [recipe](meta-custom/recipes-kernel/linux/linux-stm32mp_%25.bbappend) will ensure applying those before compiling the **device-tree** into a **device-tree blob** that will extend the original kernel recipe.  

### Kernel Module  

A detailed comprehensive [documentation](https://github.com/AKhadhraoui47/Kernel_Modules) is provided about kernel development and the structure of the [module](meta-custom/recipes-kernel/grovewifi/files/grovewifiv2.c) i have developed..  

> Note that the correspondance between the hardware declared in the device-tree and the kernel module is ensured through [of_device_id struct](https://github.com/AKhadhraoui47/Kernel_Modules?tab=readme-ov-file#5-open-firmware-device-id).  

Now that our source code is ready we will need two things: 

- [Makefile](meta-custom/recipes-kernel/grovewifi/files/Makefile): This make will process our kernel module to turn it into a **kernel object**  

- [Recipe](meta-custom/recipes-kernel/grovewifi/grovewifiv2-modules_1.0.bb): The recipe ensures that **make** process is done over **cross-compilattion** then installed under the right directory.  

> If you're not familiar with **Makefile** and the **make** build tool check this **[repo](https://github.com/AKhadhraoui47/Yocto_Rpi_IMU?tab=readme-ov-file#makefile)**.  

#### The recipe 🥢  

Let's go through the main components of this particular [Recipe](meta-custom/recipes-kernel/grovewifi/grovewifiv2-modules_1.0.bb).  

```
inherit modules
```  

This line tells Yocto to inherit the module class, which contains common functions and behaviors used for building Linux kernel modules.  

```
EXTRA_OEMAKE:append:task-install = " -C ${STAGING_KERNEL_DIR} M=${S}"
EXTRA_OEMAKE += "KDIR=${STAGING_KERNEL_DIR}"
```  
$\color{aqua}{EXTRA}$ $\color{aqua}{OEMAKE}$ : Additional GNU make options.  
$\color{aqua}{:append:task-install}$: This restricts the additional options to the "install" task (When the compiled kernel module is installed to its final location).  
$\color{aqua}{-C}$ $\color{aqua}{STAGING}$ $\color{aqua}{KERNEL}$ $\color{aqua}{DIR}$: Tells make to change to the kernel's staging directory, which contains the kernel headers and build scripts needed for the module.  
$\color{aqua}{M=}$ : Specifies the module source code directory, so make knows where to look for the module’s source files.  

Now to interact with our module an under-improvement CLI is provided.  

### Command Line Interface  

A [CLI](meta-custom/recipes-kernel/grovewifi/files/grovewifi_cli.c) written in C is provided as a built-in feature in our generated image is provided. This CLI exploits the **[Sysfs](https://github.com/AKhadhraoui47/Kernel_Modules?tab=readme-ov-file#b-sysfs-interface)** entries to communicate with the kernel module. 

This CLI provides the commands below  

```
grovewifi_CLI --chk-state /*Returns if the state is OK or Not*/
grovewifi_CLI --connect <ssid> <pwd> /*Returns connection is established*/
grovewifi_CLI --disconnect /*Returns if operation is successful or not*/
grovewifi_CLI --get-ip /*Returns your IP addr*/
grovewifi_CLI --get-ip <IP or Host> /*Returns ok or not*/ 
```
> Note that this CLI is under improvement and "successful operation" and "unsuccessful operation" are the provided responses. 

#### The recipe 🥢  

Let's go through the main components of this [Recipe](meta-custom/recipes-kernel/grovewifi/grovewifiv2-cli_1.0.bb).  

```
do_compile() {
         ${CC} grovewifi_cli.c -o grovewifi_CLI
}
```

This function is used to cross-compile the source code into binary that will be installed in our target image through:  

```
do_install() {
         install -d ${D}${bindir}
         install -m 0755 grovewifi_CLI ${D}${bindir}
}
```

The first line command installs the **bindir** which references **/usr/bin**   
The second line installs the executable with specific permissions under this directory  

## Demo 🎞️  

Let's go for a demo showcasing the outcome of our hardwork. after building and flashing.  
 

## References :label:  

[DigiKey](https://www.digikey.com/en/maker/projects/intro-to-embedded-linux-part-5-how-to-enable-i2c-in-the-yocto-project/6843bbf9a83c4c96888fccada1e7aedf)  <sub>Shawn Hymel</sub>  

[12 Glossary Variables](https://docs.yoctoproject.org/ref-manual/variables.html)  <sub>Yocto Project Documentation</sub>  

[Yocto Project Manual](https://docs.yoctoproject.org/ref-manual/)  <sub>Yocto Documentation</sub>

[The Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/)  <sub>Peter Jay Salzman, Michael Burian, Ori Pomerantz, Bob Mottram, Jim Huang</sub>  
  
[LINUX DEVICE DRIVERS 3rd Edition](https://www.google.com/url?sa=t&source=web&rct=j&opi=89978449&url=https://lwn.net/Kernel/LDD3/&ved=2ahUKEwjJ4vzx3JyHAxWjT6QEHZg9BcQQFnoECBQQAQ&usg=AOvVaw01bM6Zgwp5iRGPE8AVMxj-) <sub>Jonathan Corbet, Alessandro Rubini, and Greg Kroah-Hartman </sub>

[Serial Device Bus](http://events17.linuxfoundation.org/sites/events/files/slides/serdev-elce-2017-2.pdf) <sub> Johan Hovold </sub>