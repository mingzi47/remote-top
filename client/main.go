package main

import (
	"embed"

	"github.com/wailsapp/wails/v2"
	"github.com/wailsapp/wails/v2/pkg/options"
	"github.com/wailsapp/wails/v2/pkg/options/assetserver"
)

//go:embed all:frontend/dist
var assets embed.FS

func main() {
	// Create an instance of the app structure
	collect := NewCollect()

	// Create application with options
	err := wails.Run(&options.App{
		Title:  "remote-top",
		Width:  1324,
		Height: 868,
		AssetServer: &assetserver.Options{
			Assets: assets,
		},
		BackgroundColour: &options.RGBA{R: 255, G: 255, B: 255, A: 1},
		OnStartup:        collect.startup,
		Bind: []interface{}{
      collect,
		},
	})

	if err != nil {
		println("Error:", err.Error())
	}
}
