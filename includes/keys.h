#ifndef KEYS_H
# define KEYS_H

#ifdef __APPLE__

# define SPACE	49
# define ESC	53
# define KEY_1	18
# define KEY_2	19
# define KEY_I	34
# define MOUSE_SCROLL_UP	5
# define MOUSE_SCROLL_DOWN	4
# define MOUSE_LEFT_BUTTON	1
# define MOUSE_RIGHT_BUTTON	2

#elif __linux__

# define SPACE	32
# define ESC	65307
# define KEY_1	49
# define KEY_2	50
# define KEY_I	105
# define MOUSE_SCROLL_UP	4
# define MOUSE_SCROLL_DOWN	5
# define MOUSE_LEFT_BUTTON	1
# define MOUSE_RIGHT_BUTTON	3

#endif

#endif
