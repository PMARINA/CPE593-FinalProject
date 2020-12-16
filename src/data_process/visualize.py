output_file = "Reformatted_Input.txtOUTPUT"
lines = []
with open(output_file) as f:
    lines = f.readlines()
import pygame

pygame.init()

SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

disp = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
