"""
Description: A tool to help with 14 segment display font design via pygame.
Author: Jordan Martayn
Date: 14/09/2024
"""
import sys

import pygame

WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600

args = sys.argv
segments = []
segmentBitArray = 0b0000000000000000

#performance options
fps = 30
if len(args) > 1:
    try:
        argInt = int(args[1])
        if 0 < argInt < 145:
            fps = argInt
        else: print('Invalid FPS, needs to be int between 0 & 145')
    except ValueError:
        print('Exception Invalid FPS, needs to be int between 0 & 145')
print('fps: ' + str(fps) )

smallTextures = False
if len(args) > 2 and args[2].lower() == 'true':
    smallTextures = True
    print("Small textures enabled")
else: print('Regular textures enabled')
#asset dir 
assets = "./assets/"
if smallTextures:
    assets = "./assets/small/"

# pygame setup
pygame.init()
screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
screenRect = pygame.Rect(0,0,WINDOW_WIDTH,WINDOW_HEIGHT)
clock = pygame.time.Clock()
running = True

#colours
litColor = pygame.Color(175, 0, 0)
fillColor = pygame.Color(50,50,50)



#fonts
font = pygame.font.Font("./assets/RobotoMono-Regular.ttf" , 30)

#always present Images 
seg_all_dim_img = pygame.image.load( assets + "all-dim.png" ).convert_alpha()
seg_all_dim_img_scaledup = pygame.transform.scale(seg_all_dim_img, (WINDOW_WIDTH, WINDOW_HEIGHT))


#clear Button
clearTextImg = font.render( 'Clear', True, litColor)

class Segment:



    def __init__(self, name, bitPos, posSize, imagePath):
        self.name = name
        self.litState = False
        self.posSize = posSize
        self.imagePath = imagePath
        self.bitPos = bitPos
        image = pygame.image.load( assets + self.imagePath).convert_alpha()
        self.image = pygame.transform.scale(image, (WINDOW_WIDTH, WINDOW_HEIGHT))
        self.rect = pygame.Rect(self.posSize)
    def blitImage(self):
        screen.blit(self.image , screenRect.topleft)
    
    def toggle(self):
        global segmentBitArray

        self.litState = not self.litState
        if self.litState:
            segmentBitArray |= (1 << self.bitPos )
        else:
            segmentBitArray &= ~(1 << self.bitPos )

    def clear(self):
        global segmentBitArray
        if self.litState:
            segmentBitArray &= ~(1 << self.bitPos )
            self.litState = False

#methods
def createSegments():
    global segments
    segments = [
        Segment( "a",   0,  (110,45,225,50),    "a-lit.png" ),
        Segment( "b",   1,  (339,100,50,172),   "b-lit.png" ),
        Segment( "c",   2,  (339,329,50,172),   "c-lit.png" ),
        Segment( "d",   3,  (110,502,225,50),   "d-lit.png" ),
        Segment( "e",   4,  (60,329,50,172),    "e-lit.png" ),
        Segment( "f",   5,  (60,100,50,172),    "f-lit.png" ),
        Segment( "g1",  6,  (110,275,87,50),    "g1-lit.png" ),
        Segment( "g2",  7,  (255,275,87,50),    "g2-lit.png" ),
        Segment( "h",   8,  (122,107,62,150),   "h-lit.png" ),
        Segment( "i",   9,  (200,107,45,150),   "i-lit.png" ),
        Segment( "j",   10, (265,107,62,150),   "j-lit.png" ),
        Segment( "k",   11, (265,339,62,150),   "k-lit.png" ),
        Segment( "l",   12, (200,339,45,150),   "l-lit.png" ),
        Segment( "m",   13, (122,339,62,150),   "m-lit.png" ),
        Segment( "dp",  14, (407,500,60,65),    "dp-lit.png" ),
    ]

def clearAllSegments():
    global segments
    for segment in segments:
        segment.clear()   

#Pre game loop executions
createSegments()

#game loop
while running:

    #clear screen and fill with bg
    screen.fill(fillColor)

    #render non conditional images
    screen.blit(seg_all_dim_img_scaledup, screenRect.topleft)

    #render clear button & text
    clearBtn = pygame.draw.rect(screen, litColor, pygame.Rect(450,300,110,60), 4)
    screen.blit(clearTextImg, (461, 310) )

    #render shown 'lit' segments
    for segment in segments:
        if segment.litState:
            segment.blitImage()
    
    # poll for events
    for event in pygame.event.get():

        #mouse click detection and toggle segment
        if event.type == pygame.MOUSEBUTTONUP:
            pos = event.pos

            if clearBtn.collidepoint(pos):
                clearAllSegments()
                break

            #Check if a segment has been clicked
            for segment in segments:
                if segment.rect.collidepoint(pos):
                    segment.toggle()
                    break

        #window quit detection
        if event.type == pygame.QUIT:
            running = False
        
    #render output text
    formattedBin = format(segmentBitArray, '016b')
    textImg = font.render( formattedBin, True, litColor)
    screen.blit(textImg, (450, 100) )
    
    formattedHex = "0x"+format(segmentBitArray, '04X')
    textImg = font.render( formattedHex, True, litColor)
    screen.blit(textImg, (450, 200) )

    # flip() the display to put your work on screen
    pygame.display.flip()

    #set fps with first argument or 15 as default.

    clock.tick(fps)

pygame.quit()