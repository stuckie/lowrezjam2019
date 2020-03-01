#!/bin/bash

# Bare minimum for Debian Package structure
mkdir -p debian/usr/share/doc/lowrezfishn
mkdir -p debian/usr/share/man/man1
mkdir -p debian/DEBIAN

# Changelog is required, it's the changelog for the package maintainer
cp changelog.Debian debian/usr/share/doc/lowrezfishn/changelog.Debian
# Copyright is required, though doesn't appear to need changed
cp copyright debian/usr/share/doc/lowrezfishn/copyright
# Manpage is required, and follows a specific layout
cp man.1 debian/usr/share/man/man1/lowrezfishn.1

# Control file details dependencies, version, etc.. version must match changelog
cp control debian/DEBIAN/control
# Script called on uninstall
cp prerm debian/DEBIAN/prerm
# Script called on post install
cp postinst debian/DEBIAN/postinst

# changelog requires gzipping
gzip --no-name --force --best debian/usr/share/doc/lowrezfishn/changelog.Debian
# as does manual
gzip --no-name --force --best debian/usr/share/man/man1/lowrezfishn.1

# must use fakeroot to ensure ids are set appropriately
fakeroot dpkg-deb --build debian
# lintian will complain about anything missing
lintian debian.deb
