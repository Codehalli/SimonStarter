/*
 *  main.c
 *
 *  Created on: Dec 10, 2019
 *      Author: Pranav Rao
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "game.h"
#include "init.h"
#include "graphics.h"
#include "stdio.h"
#include "timer.h"
#include "HAL_I2C.h"
#include "HAL_OPT3001.h"

#define PRESSED 0

int main(void)
{
    simon_t simon;//setting up racing game
    init_simon(&simon); //setting up front initial screen
    init_Graphics(&simon); //setting the graphics
    initialize(); //initializing the timers
    init_scores(&simon);
    while (1)
    {
        if (!Launch_Left_pressed()) //Non-blocking LED
            TurnOff_Launch_LED1();
        else
            TurnOn_Launch_LED1();

        getSampleJoystick(&simon.vx, &simon.vy); // x and y joy stick
        simon.lux = OPT3001_getLux();
        choose_gameMode(&simon);

        if(simon.currentMode == splashSceen){
           drawSplash(&simon); //display the splash screen
           splashClock(40000000); //timer for splash scren
           Graphics_Context *g_sContext_p = &simon.g_sContext;
           Graphics_clearDisplay(g_sContext_p);//clear the screen
           simon.currentMode = mainMenu; //change to main menu
        }
        if(simon.currentMode == mainMenu){
             Disp_Menu(&simon); //display the main menu
             Disp_Select(&simon);//show the cursor on the menu

             menu_selection(&simon); //decide where the cursor is on the menu
             menu_clicking(&simon); //decide what the user is selecting on the menu
        }
        if(simon.currentMode == playGame) //if the user selected to play the game
        {
            if(simon.currentGame==lightMode){
                Disp_gameScreen(&simon);
                play_sensor(&simon);
                input_sensor(&simon);
            }
            if(simon.currentGame==buttonMode){
                Disp_gameScreen(&simon);
                play_buttons(&simon);
                input_buttons(&simon);
            }
            if(!Get_Status_Joystick_Sel()){
                 simon.currentMode = mainMenu;
                 init_Graphics(&simon);
             }
        }
        if(simon.currentMode == highScores){ //view the highscores
            highscore_select(&simon);
            if(!Get_Status_Joystick_Sel())
            {
               simon.currentMode = mainMenu;
               Graphics_clearDisplay(&simon.g_sContext);
            }
        }
        if(simon.currentMode == howTo) // view how-to-play menu
        {
           Disp_How(&simon);
           howTo_Selection(&simon);
           splashClock(100000);

           if(!Get_Status_Joystick_Sel()){
               simon.currentMode = mainMenu;
               Graphics_clearDisplay(&simon.g_sContext);
            }
        }
        if(simon.currentMode == gameOver) // view the high scores
        {
          endGame(&simon);
          if(!Get_Status_Joystick_Sel())
          {
            Graphics_clearDisplay(&simon.g_sContext);
            init_simon(&simon);
            backto_Main(&simon);
          }
        }
        if(simon.currentMode == controls) // Views the interactive mode
        {
           Disp_Controls(&simon);
           if(simon.currentGame==lightMode){
             interactmodeL(&simon);
           }
          if(simon.currentGame==buttonMode){
              interactmodeB(&simon);
            }
           if(!Get_Status_Joystick_Sel()){
               simon.currentMode = mainMenu;
               Graphics_clearDisplay(&simon.g_sContext);
            }
        }

    }
}
