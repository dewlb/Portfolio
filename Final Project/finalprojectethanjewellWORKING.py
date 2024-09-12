# Ethan Jewell
# 6/22/2021
# Final Project: Card Matching

#Imports
import pygame, sys
import random
import math
import time
from pygame.locals import *

# Setup
pygame.init()
DISPLAYSURF = pygame.display.set_mode((1000, 600))
pygame.display.set_caption("Card Matching")
framecount = 0

# Colors
BLUE = pygame.Color(0,0,255)
GREEN = pygame.Color(4,150,7)
RED = pygame.Color(209,45,54)
GREY = pygame.Color(100,100,100)
ORANGE = pygame.Color(255,69,0)
BLACK = pygame.Color(0, 0, 0)
WHITE = pygame.Color(255, 255, 255)

# Load images
cardback = pygame.image.load("cardback.xcf")
card0 = pygame.image.load("rat.xcf")
card1 = pygame.image.load("ox.xcf")
card2 = pygame.image.load("tiger.xcf")
card3 = pygame.image.load("rabbit.xcf")
card4 = pygame.image.load("dragon.xcf")
card5 = pygame.image.load("snake.xcf")
card6 = pygame.image.load("horse.xcf")
card7 = pygame.image.load("goat.xcf")
card8 = pygame.image.load("monkey.xcf")
card9 = pygame.image.load("rooster.xcf")
card10 = pygame.image.load("dog.xcf")
card11 = pygame.image.load("pig.xcf")

# Card constants
cardwidth = 86
cardlength = 120
cardx = 160
cardy = 30

left = 1

font = pygame.font.SysFont(None, 20)

flippedcards = 0

TURN = 0
player0 = 0
player1 = 0

# Lists
images = [card0, card1, card2, card3, card4, card5, card6, card7, card8, card9, card10, card11, cardback]
cards = []
values = [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11]

while True:
    
    # Events
    for event in pygame.event.get():

        if event.type == QUIT:
            pygame.quit()
            sys.exit()

        # Check for card being clicked
        if event.type == pygame.MOUSEBUTTONDOWN and event.button == left:
            x = pygame.mouse.get_pos()[0]
            y = pygame.mouse.get_pos()[1]
            # Flip cards that are clicked
            for card in cards:
                if x < (card[0] + cardwidth) and x > card[0] and y < (card[1] + cardlength) and y > card[1]:
                    new = (card[0], card[1], card[2], 1)
                    cards.remove(card)
                    cards.append(new)
                    flippedcards += 1
                    break

    DISPLAYSURF.fill(GREEN)
    
    # Append cards
    if framecount == 0:
        for col in range(4):
            for row in range(6):
                    pos = random.randint(0,(len(values) - 1))
                    value = values[pos]
                    values.remove(values[pos])
                    xpos = cardx + (row * 120)
                    ypos = cardy + (col * 140)
                    cards.append([xpos, ypos, value, 0])

    # Create the cards
    for card in cards:
        if card[3] == 0:
            DISPLAYSURF.blit(images[12], (card[0], card[1], cardwidth, cardlength))
        elif card[3] == 1:
            DISPLAYSURF.blit(images[card[2]], (card[0], card[1], cardwidth, cardlength))
            
    # Displays who turn it is
    if TURN == 0:
        img = font.render("It is player one's turn", True, WHITE)
        DISPLAYSURF.blit(img, (435, 576))
    elif TURN == 1:
        img = font.render("It is player two's turn", True, WHITE)
        DISPLAYSURF.blit(img, (435, 576))
    # Update
    pygame.display.update()
    framecount += 1

    # Test the flipped cards
    if flippedcards >= 2:
        totest = []
        time.sleep(1)
        # Find the two cards
        for i in range(len(cards)):
            if cards[i][3] == 1:
                totest.append(cards[i])
        # If the cards are the same kind
        if totest[0][2] == totest[1][2]:
            for i in range(len(cards)):
                if cards[i][3] == 1:
                    cards.remove(cards[i])
                    break
            for i in range(len(cards)):
                if cards[i][3] == 1:
                    cards.remove(cards[i])
                    break
            if TURN == 0:
                player0 += 1
            elif TURN == 1:
                player1 += 1
        # If they aren't the same kind
        else:
            for i in range(len(cards)):
                if cards[i][3] == 1:
                    cards[i] = (cards[i][0], cards[i][1], cards[i][2], 0)
            if TURN == 0:
                TURN = 1
            elif TURN == 1:
                TURN = 0
        flippedcards = 0
    
    # Win condition
    if len(cards) == 0:
        if player0 > player1:
            print("Player One Wins!")
            print("Player One:", player0)
            print("Player Two:", player1)
            pygame.quit()
            sys.exit()
            break
        elif player0 < player1:
            print("Player Two Wins!")
            print("Player One:", player0)
            print("Player Two:", player1)
            pygame.quit()
            sys.exit()
            break
        elif player0 == player1:
            print("It was a tie!")
            print("Player One:", player0)
            print("Player Two:", player1)
            pygame.quit()
            sys.exit()
            break
