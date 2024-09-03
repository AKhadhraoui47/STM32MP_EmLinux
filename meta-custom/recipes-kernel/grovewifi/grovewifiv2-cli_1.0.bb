SUMMARY = "Grove wifi module command line interface"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://grovewifi_cli.c"
TARGET_CC_ARCH += "${LDFLAGS}"
S = "${WORKDIR}"

do_compile() {
         ${CC} grovewifi_cli.c -o grovewifi_CLI
}

do_install() {
         install -d ${D}${bindir}
         install -m 0755 grovewifi_CLI ${D}${bindir}
}
