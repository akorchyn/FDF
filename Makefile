.PHONY: all re fcleam clean create_dir

NAME = fdf

UNAME_S = $(shell uname -s)

CC = clang

FLAGS = -Wall -Wextra -Werror -c

LIBFT = libft/libft.a

MLX = -lmlx

ifeq ($(UNAME_S),Linux)
	MLX += -lXext -lX11
endif
ifeq ($(UNAME_S),Darwin)
	MLX += -framework AppKit -framework OpenGL
endif

INCLUDES = -I includes

FILE = draw_line key main parse point

OBJ_LIST = $(addprefix obj/, $(addsuffix .o, $(FILE)))

all: create_dir $(NAME)

$(NAME): $(OBJ_LIST) $(LIBFT)
	@echo "\x1b[33mFDF linking\x1b[0m"
	@$(CC) $(OBJ_LIST) $(LIBFT) -o $(NAME) $(MLX) -lm
	@echo "\x1b[32mSuccessful\x1b[0m"

create_dir:
	@mkdir -p obj

$(LIBFT):
	@make -C libft

obj/%.o: src/%.c
	@printf "\x1b[33mCompiling %s. \x1b[0m" $<
	@$(CC) $(FLAGS) $(INCLUDES) -o $@ $<
	@echo "\x1b[32mDone.\x1b[0m"

clean:
	@make clean -C libft
	@/bin/rm -rf $(OBJ_LIST)

fclean: clean
	@/bin/rm -rf $(NAME) $(LIBFT)

re: fclean all
