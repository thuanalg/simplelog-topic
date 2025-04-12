# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=22cdd382a6275cb4c2e75c517952ac7c"

SRC_URI = "git://github.com/thuanalg/simplelog-topic.git;branch=main;protocol=https"

# Modify these as desired
PV = "1.0+git"
SRCREV = "ff30ba71807db1d7e1962dd014c2bb0a060572a3"
#ff30ba71807db1d7e1962dd014c2bb0a060572a3

S = "${WORKDIR}/git"

inherit cmake

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE += "-DUNIX_LINUX=1"


