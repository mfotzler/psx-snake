# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.21)

project(
	Snake
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "Snake"
	HOMEPAGE_URL "https://github.com/mfotzler/psx-snake"
)

psn00bsdk_add_executable(template GPREL main.c game.c)

psn00bsdk_add_cd_image(
	iso      # Target name
	template # Output file name (= template.bin + template.cue)
	iso.xml  # Path to config file
	DEPENDS template
)
