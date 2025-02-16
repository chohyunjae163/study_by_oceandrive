package wfc

import "core:fmt"
import "core:strings"

import rl "vendor:raylib"

blank_path :: "../res/blank.png"
up_path :: "../res/up.png"
down_path :: "../res/down.png"
left_path :: "../res/left.png"
right_path :: "../res/right.png"

SCREEN_WIDTH :: 1280
SCREEN_HEIGHT :: 720

PATTERN_SIZE :: 50

main :: proc() {
	fmt.println("Hello World!")
	
	rl.InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "wfc game")

 
	blank_image := rl.LoadImage(blank_path)
	up_image := rl.LoadImage(up_path)
	down_image := rl.LoadImage(down_path)
	left_image := rl.LoadImage(left_path)
	right_image := rl.LoadImage(right_path)

	canvas := rl.GenImageColor(SCREEN_WIDTH,SCREEN_HEIGHT,rl.WHITE)
	srcRec :=rl.Rectangle { 0, 0, PATTERN_SIZE,PATTERN_SIZE };
	rl.ImageDraw(&canvas,blank_image, srcRec, rl.Rectangle { 0,0, PATTERN_SIZE,PATTERN_SIZE },rl.WHITE)
	rl.ImageDraw(&canvas,up_image,    srcRec, rl.Rectangle { 50,0, PATTERN_SIZE,PATTERN_SIZE },rl.WHITE)
	rl.ImageDraw(&canvas,down_image,  srcRec, rl.Rectangle { 100,0,PATTERN_SIZE,PATTERN_SIZE },rl.WHITE)
	rl.ImageDraw(&canvas,left_image,  srcRec, rl.Rectangle { 150,0,PATTERN_SIZE,PATTERN_SIZE },rl.WHITE)
	rl.ImageDraw(&canvas,right_image, srcRec, rl.Rectangle { 200,0,PATTERN_SIZE,PATTERN_SIZE },rl.WHITE)
	FRAMEBUFFER := rl.LoadTextureFromImage(canvas)

	for !rl.WindowShouldClose() {

        // Update
        //----------------------------------------------------------------------------------


        //----------------------------------------------------------------------------------
      	

      	// Draw
        //----------------------------------------------------------------------------------		
        rl.BeginDrawing()
			rl.ClearBackground(rl.BLUE)
			rl.DrawTexture(FRAMEBUFFER,0,0,rl.WHITE)
		rl.EndDrawing()
        //----------------------------------------------------------------------------------		
	}


    // De-Initialization
    //--------------------------------------------------------------------------------------
    rl.UnloadImage(blank_image)
    rl.UnloadImage(up_image)
    rl.UnloadImage(down_image)
    rl.UnloadImage(left_image)
    rl.UnloadImage(right_image)
    rl.UnloadTexture(FRAMEBUFFER);       // Texture unloading

    rl.CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
} 


