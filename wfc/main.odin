package wfc

import "core:fmt"
import "core:time"

import rl "vendor:raylib"

blank_path :: "../res/blank.png"
up_path :: "../res/up.png"
down_path :: "../res/down.png"
left_path :: "../res/left.png"
right_path :: "../res/right.png"

SCREEN_WIDTH :: 1000
SCREEN_HEIGHT :: 1000

PATTERN_SIZE :: 50

NUM_ROW :: 20
NUM_COL :: 20


is_collapsed :: proc( options : u8) -> bool {
	return (options & (options - 1)) == 0
}

pick_pattern :: proc(options : u8) -> u8 {
	arr : [5]u8
	len : i32 = 0

	for cur : u8 = 2; cur <=32; cur *= 2 {
		if( ( options & cur ) == cur ) {
			arr[len] = cur
			len += 1
		}
	}

	random_index := rl.GetRandomValue(0, len - 1)

	return arr[random_index]
}

calc_power_of_two :: proc( num : u8 ) -> u8 {
	power : u8 = 1
	n := num
	for n > 2 {
		n /= 2
		power += 1
	}
	return power
}

Pattern :: enum {Blank = 2, Up = 4, Down = 8, Left = 16, Right = 32 }

main :: proc() {
    // Initialization
    //--------------------------------------------------------------------------------------
	
	rl.InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "wfc game")
 
	blank_image := rl.LoadImage(blank_path)
	up_image := rl.LoadImage(up_path)
	down_image := rl.LoadImage(down_path)
	left_image := rl.LoadImage(left_path)
	right_image := rl.LoadImage(right_path)
	image_arr : [5]rl.Image = { blank_image, up_image, down_image,left_image,right_image }



	rl.SetRandomSeed(u32(time.read_cycle_counter()))
	cur_index := rl.GetRandomValue(0,NUM_ROW*NUM_COL - 1)

	options := u8(Pattern.Blank) + u8(Pattern.Up) + u8(Pattern.Down) + u8(Pattern.Left) + u8(Pattern.Right)
	grids : [NUM_ROW * NUM_COL]u8
	for i := 0; i < NUM_ROW; i += 1 {
		for j := 0; j < NUM_COL; j += 1{		
			cur_index := rl.GetRandomValue(0,NUM_ROW*NUM_COL - 1)
			grids[ j + i * NUM_ROW ] = pick_pattern(options)
		}
	} 

	canvas := rl.GenImageColor(SCREEN_WIDTH,SCREEN_HEIGHT,rl.WHITE)
	srcRec :=rl.Rectangle { 0, 0, PATTERN_SIZE,PATTERN_SIZE };

 	for i := 0; i < NUM_ROW; i += 1 {
		for j := 0; j < NUM_COL; j += 1 {
			image_index := calc_power_of_two(grids[ j + i * NUM_ROW ]) - 1
			image := image_arr[image_index]
			rl.ImageDraw(&canvas,
				image, 
				srcRec, 
				rl.Rectangle { f32(j * PATTERN_SIZE), f32(i * PATTERN_SIZE) , PATTERN_SIZE,PATTERN_SIZE },
				rl.WHITE)
		}
	}
	FRAMEBUFFER := rl.LoadTextureFromImage(canvas)
	//grids[cur_index] = pick_pattern(grids[cur_index])
	
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


