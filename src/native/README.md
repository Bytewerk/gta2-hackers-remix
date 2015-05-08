`G2HR` officially supports Windows and Linux (via Wine). To get good
controller support on Linux, that does not need to go through the wine
layer, this `native` component will be created.

Here is a more detailed list of tasks:
* host a TCP server for native<->menu communication
* start the menu (on Linux: start the virtual wine desktop with the menu
	inside)
* parse controller mappings from a config file (this will come later,
	once the basic stuff is working)
* bind all connected (xbox 360 compatible) game controllers and
	interpret the keys
* send toolkit events (eg. TK_ACTION_ENTER) as well as the count of
	connected controllers (initially and when it changes) to the menu,
	while it is active
* when the game is running (meta gets a notification from the menu),
	connect to the `injected` components of all running instances via
	TCP and generate/send the movement bytes from the controller input
* some advanced stuff: rumble support (definatelly planned!),
	buffer/proxy the current score and maybe some other stats from the
	`injected` components to the `menu` component
* (maybe integrate video recording by calling ffmpeg?)


folder|description
------|-------
net | network code (server talks to 'meta', (up to 6) clients talk to 'injected')
pads | controller code (Xbox360 controllers and compatible supported)
