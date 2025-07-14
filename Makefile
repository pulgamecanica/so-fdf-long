NAME = graphics_app

CC = cc
CFLAGS = -O3 -Wall -Wextra -Werror -Iinclude -IMLX42/include

SRC_MAIN = $(wildcard main/*.c)
SRC_SCENES = $(wildcard scenes/*.c)
SRC_SHARED = $(wildcard shared/*.c)
SRC_SO_LONG = $(wildcard so_long/*.c) $(wildcard so_long/*/*.c) 
SRC_FDF = $(wildcard fdf/*.c) $(wildcard fdf/*/*.c)
SRC_FRACTOL = $(wildcard fractol/*.c) $(wildcard fractol/*/*.c)

OBJS = $(SRC_MAIN:.c=.o) $(SRC_SCENES:.c=.o) $(SRC_SHARED:.c=.o) \
       $(SRC_SO_LONG:.c=.o) $(SRC_FDF:.c=.o) $(SRC_FRACTOL:.c=.o)

SRCS =  $(SRC_MAIN) $(SRC_SCENES) $(SRC_SHARED) $(SRC_SO_LONG) $(SRC_FDF) $(SRC_FRACTOL)

MLX_FLAGS = -LMLX42/build -lmlx42 -ldl -lglfw -lm -lpthread# -L Libft -lft
MLX_NATIVE_LIB = MLX42/build/libmlx42_native.a
MLX_WEB_LIB = MLX42/build_web/libmlx42_web.a
WEB = web/demo.js

all: $(MLX_NATIVE_LIB) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLX_FLAGS)

$(MLX_NATIVE_LIB):
	cd MLX42 && cmake -B build
	cd MLX42 && cmake --build build -j4
	mv MLX42/build/libmlx42.a $(MLX_NATIVE_LIB)

$(MLX_WEB_LIB):
	cd MLX42 && emcmake cmake -B build_web
	cd MLX42 && cmake --build build_web --parallel
	mv MLX42/build_web/libmlx42.a $(MLX_WEB_LIB)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

web: $(WEB)

$(WEB): $(SRCS) $(MLX_WEB_LIB)
	mkdir -p web
	emcc -DWEB -O3 -I include -I MLX42/include -pthread $(SRCS) \
		-o $(WEB) \
		$(MLX_WEB_LIB) \
		-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 \
		-s NO_EXIT_RUNTIME=1 -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
		-s ALLOW_MEMORY_GROWTH \
		--preload-file assets

re: fclean all

.PHONY: all clean fclean re $(WEB)
