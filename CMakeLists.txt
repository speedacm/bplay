CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(bplay)

SET(bplay_src
    bplay/main.cpp
    bplay/PCSpeaker.cpp
    bplay/token.cpp
)

ADD_EXECUTABLE(bplay ${bplay_src})
