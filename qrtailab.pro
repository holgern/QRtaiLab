# -*- mode: sh -*- ###########################
# QRtailab
# Copyright (C) 2008   Holger Nahrstaedt
#
# This programm is free software; you can redistribute it and/or
# modify it under the terms of the LGPL
##############################################

include( qrtailab.config )

TEMPLATE = subdirs

SUBDIRS = \
    QRtaiLab

contains(CONFIG, QRLtests ) {
    SUBDIRS += tests
}


