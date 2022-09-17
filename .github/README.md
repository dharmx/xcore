```
xcore-0.0.1-alpha
dharmx

USAGE:

	xcore -[a|x|g|c|v|h]
	
	xcore --[active|version|geo|geosh|center|help]
	
	xcore --normal=<wid1> --normal=<wid2> ...
	
	xcore --normal=<wid1> --stick=<wid2> -g ...
	
	xcore --[geosh|geo|center]=<wid> / --[desktop]=<delim>

OPTIONS:

	-a, --active               print active window wid
	
	-p, --pid                  print window pid
	
	-g, --geo                  active window geometry, format: x y w h border depth wxh+x+y wid
	
	-G, --geosh                similar to -g but can be used with shell eval
	
	-d, --desktop              print desktop labels
	
	-k, --close                close window
	
	-k, --kill                 kill window
	
	-A, --focus                focus window
	
	-b, --activate             activate window
	
	-e, --get-border           get window border size
	
	-n, --class                print window class, format: wm_name wm_class
	
	-N, --title                print window title, format: encoding length value
	
	-i, --hide, --icon         hide a window
	
	-m, --map, --show          show a window
	
	-M, --unmap                unmap a window
	
	-s, --stick                make a window sticky
	
	-o, --proto                display window protocols
	
	-S, --unstick              return a window to normal state from sticky
	
	-f, --fullscreen           fullscreen window
	
	-F, --normal               return a window to normal state from fullscreen
	
	-D, --total-desktops       get the number of desktops
	
	-I, --unhide, --unicon     uniconify a window
	
	-c, --center               move pointer to the middle of the window
	
	-z, --kc2ksym              convert keycode to keysym string
	
	-v, --version              print the version number
	
	-h, --help                 print this message
```
