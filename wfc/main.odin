package wfc

import "core:fmt"

import rl "vendor:raylib"

main :: proc() {
	fmt.println("Hello World!")
	
	rl.InitWindow(1280, 720, "wfc game")

	for !rl.WindowShouldClose() {
		rl.BeginDrawing()
		rl.ClearBackground(rl.BLUE)
		rl.EndDrawing()
	}


	rl.CloseWindow()
} 
