#!/bin/bash
# Need a key.. so do this first
# gpg2 --full-gen-key
# As a test, assume key id is: 0D68081AD08AE1B1

# Assume that a repo is already done
# mkdir repo
# mkdir repo/amd64

# These need to be performed in repo otherwise things get confused ( apt-ftparchive )
cd repo

# Export key to repo
rm -rf KEY.gpg
gpg --output KEY.gpg --armor --export 0D68081AD08AE1B1

# Update package list
apt-ftparchive packages amd64 > Packages
gzip -k -f Packages

# Update Release list
apt-ftparchive release . > Release

# Sign it
rm -fr Release.gpg; gpg --default-key 0D68081AD08AE1B1 -abs -o Release.gpg Release
rm -fr InRelease.gpg; gpg --default-key 0D68081AD08AE1B1 --clearsign -o InRelease.gpg Release

# Back out
cd ..
