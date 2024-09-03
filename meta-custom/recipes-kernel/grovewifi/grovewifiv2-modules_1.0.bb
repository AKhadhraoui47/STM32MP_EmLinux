SUMMARY = "Kernel module for Grove Wifi V2"
SECTION = "custom"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://grovewifiv2.c file://Makefile"

S = "${WORKDIR}"

inherit module

EXTRA_OEMAKE:append:task-install = " -C ${STAGING_KERNEL_DIR} M=${S}"
EXTRA_OEMAKE += "KDIR=${STAGING_KERNEL_DIR}"
