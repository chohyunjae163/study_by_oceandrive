package wfc

import "core:fmt"
import "core:strings"

import rl "vendor:raylib"

path :: "../res/"
blank_filename :: "black.png"

main :: proc() {
	fmt.println("Hello World!")
	
	rl.InitWindow(1280, 720, "wfc game")


	blank_fullpath := path + blank_filename
	image := rl.LoadImage(strings.clone_to_cstring(blank_fullpath))
	blank := rl.LoadTextureFromImage(image)
	rl.UnloadImage(image)
	for !rl.WindowShouldClose() {
		rl.BeginDrawing()
			rl.ClearBackground(rl.BLUE)
		rl.EndDrawing()
	}


	rl.CloseWindow()
} 
