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

Pattern :: enum {None = 0, Blank = 2, Up = 4, Down = 8, Left = 16, Right = 32 }
Direction :: enum{Up, Down, Left, Right}

is_collapsed :: proc( options : u8) -> bool {
	return (options & (options - 1)) == 0
}

pick_one_pattern :: proc(options : u8) -> u8 {
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



	//Make Rules
	AdjacentPatterns :: map[Direction]u8
	blank := make(AdjacentPatterns)
	defer delete(blank)
	blank[Direction.Up] = u8(Pattern.Up)
	blank[Direction.Down] = u8(Pattern.Down)
	blank[Direction.Left] = u8(Pattern.Left)
	blank[Direction.Right] = u8(Pattern.Right)

	down := make(AdjacentPatterns)
	defer delete(down)
	down[Direction.Up] = u8(Pattern.Up) | u8(Pattern.Blank)
	down[Direction.Down] = u8(Pattern.Up) | u8(Pattern.Left) | u8(Pattern.Right)
	down[Direction.Left] = u8(Pattern.Right) | u8(Pattern.Up)
	down[Direction.Right] = u8(Pattern.Left) | u8(Pattern.Up)

	left := make(AdjacentPatterns)
	defer delete(left)
	left[Direction.Up] = u8(Pattern.Down) | u8(Pattern.Right)
	left[Direction.Down] = u8(Pattern.Right) | u8(Pattern.Up)
	left[Direction.Left] = u8(Pattern.Right) | u8(Pattern.Up) | u8(Pattern.Down)
	left[Direction.Right] = u8(Pattern.Right) | u8(Pattern.Blank)

	right := make(AdjacentPatterns)
	defer delete(right)
	right[Direction.Up] = u8(Pattern.Down) | u8(Pattern.Left)
	right[Direction.Down] = u8(Pattern.Left) | u8(Pattern.Up)
	right[Direction.Left] = u8(Pattern.Blank) | u8(Pattern.Left)
	right[Direction.Right] = u8(Pattern.Down) | u8(Pattern.Up) | u8(Pattern.Left)

	up := make(AdjacentPatterns)
	defer delete(up)
	up[Direction.Up] = u8(Pattern.Down) | u8(Pattern.Left) | u8(Pattern.Right)
	up[Direction.Down] = u8(Pattern.Down) | u8(Pattern.Blank)
	up[Direction.Left] = u8(Pattern.Right) | u8(Pattern.Down)
	up[Direction.Right] = u8(Pattern.Left) | u8(Pattern.Down)


	Rules := make(map[Pattern]AdjacentPatterns)
	defer delete(Rules)
	Rules[Pattern.Blank] = blank
	Rules[Pattern.Down] = down
	Rules[Pattern.Left] = left
	Rules[Pattern.Right] = right
	Rules[Pattern.Up] = up


	rl.SetRandomSeed(u32(time.read_cycle_counter()))
	default_options := u8(Pattern.Blank) + u8(Pattern.Up) + u8(Pattern.Down) + u8(Pattern.Left) + u8(Pattern.Right)
	NUM_GRID :: NUM_ROW * NUM_COL
	grids : [NUM_GRID]u8 = { 0..< NUM_GRID = default_options }
	random_start_index := rl.GetRandomValue(NUM_ROW + 1,(NUM_ROW - 1) * (NUM_COL - 1) - 1)
	cur_index := random_start_index
	for i in 0..<NUM_GRID {
		
		grids[ cur_index ] = pick_one_pattern(grids[ cur_index ])
		pattern := Pattern(grids[ cur_index ])
		indices : [4]i32 = { 0..<4 = -1 }
		count_propagate := 0 
		if(cur_index > 0 && is_collapsed(grids[ cur_index - 1]) == false )  {
			grids[ cur_index - 1] &= Rules[ pattern ][Direction.Left]
			indices[count_propagate] = cur_index - 1
			count_propagate += 1
		}
		if(cur_index % (NUM_COL - 1) != 0 && is_collapsed(grids[ cur_index + 1]) == false) {
			grids[ cur_index + 1] &= Rules[ pattern ][Direction.Right]
			indices[count_propagate] = cur_index + 1
			count_propagate += 1
		}
		if(cur_index < NUM_GRID - NUM_COL && is_collapsed(grids[ cur_index  + NUM_COL]) == false) {
			grids[ cur_index + NUM_COL ] &= Rules[pattern][Direction.Down]
			indices[count_propagate] = cur_index + NUM_COL
			count_propagate += 1
		}
		if(cur_index >= NUM_COL && is_collapsed(grids[ cur_index  - NUM_COL]) == false) {
			grids[ cur_index - NUM_COL ] &= Rules[pattern][Direction.Up]
			indices[count_propagate] = cur_index - NUM_COL
			count_propagate += 1
		}
		
		min_num : u8 = 255
		min_index : i32 = 0
		for idx in indices {
			if(idx < 0) {
				continue;
			}
			if ( min_num > grids[idx]) {
				min_num = grids[idx]
				min_index = idx;
			}
		}
		cur_index = min_index		
	}


	canvas := rl.GenImageColor(SCREEN_WIDTH,SCREEN_HEIGHT,rl.WHITE)
	srcRec :=rl.Rectangle { 0, 0, PATTERN_SIZE,PATTERN_SIZE };

 	for i := 0; i < NUM_ROW; i += 1 {
		for j := 0; j < NUM_COL; j += 1 {
			if ( is_collapsed(grids[ j + i * NUM_ROW ]) == false)
			{
				continue;
			}
			image_index := calc_power_of_two(grids[ j + i * NUM_ROW ]) - 1
			image := image_arr[image_index]
			destRec := rl.Rectangle { f32(j * PATTERN_SIZE), f32(i * PATTERN_SIZE) , PATTERN_SIZE,PATTERN_SIZE }
			rl.ImageDraw(&canvas,
				image, 
				srcRec, 
				destRec,
				rl.WHITE)
		}
	}
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


