#pragma once

// Dog's body
/*
....##..########..##....................
..##  ##        ##  ##..................
..##                ##..................
..##                  ##............##..
##                    ####........##  ##
##                        ####....##  ##
##                            ######  ##
##                                    ##
##                                    ##
##                                    ##
##                                    ##
##                                    ##
##                                  ##..
..##                                ##..
..##    ####    ########    ####    ##..
..##    ####    ##....##    ####    ##..
..##  ##..##  ##......##  ##..##  ##....
....##......##..........##......##......
*/
Uint8 IdleData8b[20 * 19]{
	0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,
	0,1,2,1,2,2,2,2,1,2,1,0,0,0,0,0,0,0,0,0,
	0,1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,
	0,1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,1,0,
	1,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,1,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,1,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,
	0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,
	0,1,2,2,1,1,2,2,1,1,1,1,2,2,1,1,2,2,1,0,
	0,1,2,2,1,1,2,2,1,0,0,1,2,2,1,1,2,2,1,0,
	0,1,2,1,0,1,2,1,0,0,0,1,2,1,0,1,2,1,0,0,
	0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,
};

// Dog's face
/*
..##....##..
............
....####....
##..##....##
..########..
*/
Uint8 FaceData8b[6 * 5]{
	0,1,0,0,1,0,
	0,0,0,0,0,0,
	0,0,1,1,0,0,
	1,0,1,0,0,1,
	0,1,1,1,1,0,
};

// Dog's scared face
/*
..######......######..
##      ##..##      ##
##  ##  ##..##  ##  ##
##      ##..##      ##
..######......######..
..........####........
......##..##....##....
........########......
*/
Uint8 ScaredData8b[11 * 8]{
	0,1,1,1,0,0,0,1,1,1,0,
	1,2,2,2,1,0,1,2,2,2,1,
	1,2,1,2,1,0,1,2,1,2,1,
	1,2,2,2,1,0,1,2,2,2,1,
	0,1,1,1,0,0,0,1,1,1,0,
	0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,1,0,1,0,0,1,0,0,
	0,0,0,0,1,1,1,1,0,0,0
};

// Dog's barking face
/*
..##....##..
............
....####....
##..##....##
..########..
....####....
*/
Uint8 BarkData8b[6 * 6]{
	0,1,0,0,1,0,
	0,0,0,0,0,0,
	0,0,1,1,0,0,
	1,0,1,0,0,1,
	0,1,1,1,1,0,
	0,0,1,1,0,0,
};

// Dog's body
Uint8 DogData2b[95]{
	0b00010000,
	0b01010101,
	0b00000100,
	0b00000000,
	0b00000000,
	0b01100100,
	0b10101010,
	0b00011001,
	0b00000000,
	0b00000000,
	0b10100100,
	0b10101010,
	0b00011010,
	0b00000000,
	0b00000000,
	0b10100100,
	0b10101010,
	0b01101010,
	0b00000000,
	0b00010000,
	0b10101001,
	0b10101010,
	0b01101010,
	0b00000001,
	0b01100100,
	0b10101001,
	0b10101010,
	0b10101010,
	0b00010110,
	0b01100100,
	0b10101001,
	0b10101010,
	0b10101010,
	0b01101010,
	0b01100101,
	0b10101001,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01101010,
	0b10101001,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01101010,
	0b10101001,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01101010,
	0b10101001,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01101010,
	0b10101001,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01101010,
	0b10101001,
	0b10101010,
	0b10101010,
	0b10101010,
	0b01101010,
	0b10101001,
	0b10101010,
	0b10101010,
	0b10101010,
	0b00011010,
	0b10100100,
	0b10101010,
	0b10101010,
	0b10101010,
	0b00011010,
	0b10100100,
	0b10100101,
	0b01010101,
	0b01011010,
	0b00011010,
	0b10100100,
	0b10100101,
	0b01000001,
	0b01011010,
	0b00011010,
	0b01100100,
	0b01100100,
	0b01000000,
	0b01000110,
	0b00000110,
	0b00010000,
	0b00010000,
	0b00000000,
	0b00000001,
	0b00000001,
};

// Dog's face
Uint8 FaceData2b[8]{
	0b00000100,
	0b00000001,
	0b00000000,
	0b01010000,
	0b00010000,
	0b01000001,
	0b01010100,
	0b00000001
};

// Dog's scared face
Uint8 ScaredData2b[22]{
	0b01010100,
	0b01000000,
	0b01000101,
	0b01101010,
	0b10100100,
	0b10010110,
	0b00011001,
	0b10011001,
	0b10100101,
	0b01000110,
	0b01101010,
	0b01010100,
	0b01000000,
	0b00000101,
	0b00000000,
	0b00000101,
	0b00000000,
	0b01000100,
	0b00010000,
	0b00000000,
	0b01010100,
	0b00000001
};

// Dog's barking face
Uint8 BarkData2b[9]{
	0b00000100,
	0b00000001,
	0b00000000,
	0b01010000,
	0b00010000,
	0b01000001,
	0b01010100,
	0b00000001,
	0b00000101
};

// Walk cycle (frame 1)
/*
....##..########..##........................
..##  ##        ##  ##......................
..##                ##......................
..##                  ##....................
##                    ####..................
##                        ####..............
##                            ############..
##                                        ##
##                                    ####..
##                                    ##....
##                                    ##....
##                                    ##....
##                                    ##....
##                                  ##......
..##                                ##......
..##    ####    ########    ####    ##......
..##    ####  ##......##  ##..##    ##......
..##  ##....##..........##....##  ##........
....##..........................##..........
*/
Uint8 Walk1Data8b[22 * 19]{
	0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,2,1,2,2,2,2,1,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,
	1,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
	0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
	0,1,2,2,1,1,2,2,1,1,1,1,2,2,1,1,2,2,1,0,0,0,
	0,1,2,2,1,1,2,1,0,0,0,1,2,1,0,1,2,2,1,0,0,0,
	0,1,2,1,0,0,1,0,0,0,0,0,1,0,0,1,2,1,0,0,0,0,
	0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
};

// Walk cycle (frame 2)
/*
....##..########..##........................
..##  ##        ##  ##......................
..##                ##......................
..##                  ##....................
##                    ####..........##......
##                        ####....##  ##....
##                            ######  ##....
##                                    ##....
##                                    ##....
##                                    ##....
##                                    ##....
##                                    ##....
##                                    ##....
##                                  ##......
..##                                ##......
..##    ####    ########    ####    ##......
..##    ####  ##......##  ##..##    ##......
..##  ##....##..........##....##  ##........
....##..........................##..........
*/
Uint8 Walk2Data8b[22 * 19]{
	0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,2,1,2,2,2,2,1,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,
	0,1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,
	1,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,0,1,0,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,1,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
	0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
	0,1,2,2,1,1,2,2,1,1,1,1,2,2,1,1,2,2,1,0,0,0,
	0,1,2,2,1,1,2,1,0,0,0,1,2,1,0,1,2,2,1,0,0,0,
	0,1,2,1,0,0,1,0,0,0,0,0,1,0,0,1,2,1,0,0,0,0,
	0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
};