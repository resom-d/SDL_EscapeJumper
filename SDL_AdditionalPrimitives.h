#pragma once
#include "UI_Types.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <list>

void SDL_RenderSetPixel(SDL_Renderer* renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

void SDL_RenderDrawCircle(SDL_Renderer* renderer, int n_cx, int n_cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

void SDL_RenderFillCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

void SDL_RenderDrawBorder(SDL_Renderer* rend, SDL_Rect* rect, Uint16 borderWidth, SDL_Color* color);

SDL_Texture* SD_RenderLoadTexture(SDL_Renderer* renderer, std::string path);
