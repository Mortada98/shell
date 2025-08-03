NAME = minishell

#HELLO 	WORLD

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRC = src/main.c	src/Handle_command.c	src/Handle_token.c	src/Helper.c  src/prompt.c	src/print.c	src/tools.c	src/meta_char.c	src/utils.c	src/excute.c  src/parse_herdoc.c	\
	src/garbage_collector.c src/manual_realloc.c	src/open_redirection.c	src/set_global_value.c	src/exit_command.c	src/dup_parent_and_child.c	src/lexer.c  src/enviroment.c src/cd.c src/export_unset.c

LIBFT_SRC = my_libft/ft_isdigit.c my_libft/ft_isalnum.c my_libft/ft_isascii.c my_libft/ft_isprint.c my_libft/ft_strlen.c my_libft/ft_memset.c my_libft/ft_isalpha.c \
	my_libft/ft_bzero.c my_libft/ft_memcpy.c my_libft/ft_memmove.c my_libft/ft_strlcpy.c my_libft/ft_strlcat.c my_libft/ft_toupper.c my_libft/ft_tolower.c \
	my_libft/ft_strchr.c my_libft/ft_strrchr.c my_libft/ft_strncmp.c my_libft/ft_strcmp.c my_libft/ft_memchr.c my_libft/ft_memcmp.c my_libft/ft_strnstr.c my_libft/ft_atoi.c \
	my_libft/ft_calloc.c my_libft/ft_strdup.c my_libft/ft_substr.c my_libft/ft_strjoin.c my_libft/ft_strtrim.c my_libft/ft_split.c my_libft/ft_itoa.c \
	my_libft/ft_putchar_fd.c my_libft/ft_putendl_fd.c my_libft/ft_putnbr_fd.c my_libft/ft_putstr_fd.c my_libft/ft_strmapi.c 	my_libft/ft_atoi_utils.c \
	my_libft/ft_lstnew_bonus.c my_libft/ft_lstadd_back_bonus.c my_libft/ft_lstadd_front_bonus.c my_libft/ft_lstsize_bonus.c \
	my_libft/ft_lstclear_bonus.c my_libft/ft_lstlast_bonus.c my_libft/ft_lstdelone_bonus.c my_libft/ft_lstiter_bonus.c

OBJ = $(SRC:.c=.o)
LIBFT_OBJ = $(LIBFT_SRC:.c=.o)

# Colors
GREEN = \033[0;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RED = \033[0;31m
NC = \033[0m

# Spinner
SPINNER = spin() { \
	sp="/-\|"; \
	while kill -0 $$1 2>/dev/null; do \
		printf "\r[$(YELLOW)$$sp$(NC)] Building..."; \
		sp=$${sp#?}$${sp%???}; \
		sleep 0.1; \
	done; \
	printf "\r[$(GREEN)✔$(NC)] Build complete!         \n"; \
}

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_OBJ)
	@echo "$(BLUE)🔧 Compiling $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_OBJ) -o $(NAME) -lreadline
	@echo "$(GREEN)✅ $(NAME) built successfully!$(NC)"

clean:
	@rm -f $(OBJ) $(LIBFT_OBJ)
	@echo "$(YELLOW)🧹 Object files cleaned.$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)🧨 Executable removed.$(NC)"

re: fclean all

.PHONY: all clean fclean re
.INTERMEDIATE: $(LIBFT_OBJ) $(OBJ)

# NAME = minishell
#
# CC = cc
# CFLAGS = -Wall -Wextra -Werror -g
#
# SRC = src/main.c	src/Handle_command.c	src/Handle_token.c	src/Helper.c  src/prompt.c	src/print.c
#
# LIBFT_SRC = my_libft/ft_isdigit.c my_libft/ft_isalnum.c my_libft/ft_isascii.c my_libft/ft_isprint.c my_libft/ft_strlen.c my_libft/ft_memset.c my_libft/ft_isalpha.c \
# 	my_libft/ft_bzero.c my_libft/ft_memcpy.c my_libft/ft_memmove.c my_libft/ft_strlcpy.c my_libft/ft_strlcat.c my_libft/ft_toupper.c my_libft/ft_tolower.c \
# 	my_libft/ft_strchr.c my_libft/ft_strrchr.c my_libft/ft_strncmp.c my_libft/ft_memchr.c my_libft/ft_memcmp.c my_libft/ft_strnstr.c my_libft/ft_atoi.c \
# 	my_libft/ft_calloc.c my_libft/ft_strdup.c my_libft/ft_substr.c my_libft/ft_strjoin.c my_libft/ft_strtrim.c my_libft/ft_split.c my_libft/ft_itoa.c \
# 	my_libft/ft_putchar_fd.c my_libft/ft_putendl_fd.c my_libft/ft_putnbr_fd.c my_libft/ft_putstr_fd.c my_libft/ft_strmapi.c \
# 	my_libft/ft_lstnew_bonus.c my_libft/ft_lstadd_back_bonus.c my_libft/ft_lstadd_front_bonus.c my_libft/ft_lstsize_bonus.c \
# 	my_libft/ft_lstclear_bonus.c my_libft/ft_lstlast_bonus.c my_libft/ft_lstdelone_bonus.c my_libft/ft_lstiter_bonus.c
#
# OBJ = $(SRC:.c=.o)
# LIBFT_OBJ = $(LIBFT_SRC:.c=.o)
#
# all: $(NAME)
#
# $(NAME): $(OBJ) $(LIBFT_OBJ)
# 	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_OBJ) -o $(NAME) -lreadline
#
# clean:
# 	rm -f $(OBJ) $(LIBFT_OBJ)
#
# fclean: clean
# 	rm -f $(NAME)
#
# re: fclean all
#
# .PHONY: all clean fclean re
# .INTERMEDIATE: $(LIBFT_OBJ) $(OBJ)
