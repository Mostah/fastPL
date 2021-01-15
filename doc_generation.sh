#!/bin/sh
################################################################################
# Title         : doc_generation.sh
# Date created  : 2020
__AUTHOR__="Thomas Vindard"
# Preconditions:
# - Packages doxygen doxygen-doc doxygen-gui graphviz
#   must be installed.
# - The script must be executed within the root directory.
#
# This script will generate Doxygen documentation and push the documentation to
# the gh-pages.
# 
################################################################################

##### Setup this script and get the current gh-pages branch.               #####
echo 'Setting up the script...'
# Exit with nonzero exit code if anything fails
set -e

# make sure we are in the latest master branch commit
git checkout master
git pull origin master

# remove any files from previous generation
rm -rf doc/html

# create the destination directory of the documentation
cd doc && mkdir html && cd html

# Get the current gh-pages branch
git clone git@github.com:Mostah/fastPL.git .
git checkout gh-pages

# Generate the doxygen documentation into the html file
echo 'Generating Doxygen code documentation...'
cd .. && cmake . && doxygen Doxyfile.Doxygen

# Upload the new doc to gh-pages
echo 'Uploading documentation to the gh-pages branch...'
cd html && git add .
git commit -m "updated doc with the latest version of master"
git push

