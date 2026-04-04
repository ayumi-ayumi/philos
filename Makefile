# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: asato <asato@student.42berlin.de>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/07 19:31:56 by asato             #+#    #+#              #
#    Updated: 2026/04/04 16:58:31 by asato            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo
CC	=	cc
CFLAGS	=	-Wall -Wextra -Werror -ggdb3
# CFLAGS	=	-fsanitize=leak -Wall -Wextra -Werror
# CFLAGS	=	-fsanitize=thread -Wall -Wextra -Werror
SRC_DIR	=	src
OBJ_DIR	=	obj
INC_DIR	=	includes
INC		=	-I$(INC_DIR)

SRC 	=	main.c \
			init.c \
			utils.c \
			monitor.c \
			threads.c \
			actions.c \
			cleanup.c \
			libft.c \

SRCS	=	$(addprefix $(SRC_DIR)/, $(SRC))
OBJ	=	$(SRC:.c=.o)
OBJS	=	$(addprefix $(OBJ_DIR)/, $(OBJ))

MAKEFLAGS += --no-print-directory

# Colors
COLOR_RESET = \033[0m
COLOR_GREEN = \033[1;32m
COLOR_YELLOW = \033[1;33m
COLOR_BLUE = \033[1;34m

# Status message banners
BUILD_LINE = ==========[ PHILO BUILD ]==========
CLEAN_LINE = ==========[ PHILO CLEAN ]==========

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) \
	&& printf '%b%s\n[OK] philo compiled successfully\n%s%b\n' "$(COLOR_GREEN)" "$(BUILD_LINE)" "$(BUILD_LINE)" "$(COLOR_RESET)"

#compile .o and create their dir if they dont exist
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) \
	&& printf '%b%s\n[OK] philo fully cleaned\n%s%b\n' "$(COLOR_BLUE)" "$(CLEAN_LINE)" "$(CLEAN_LINE)" "$(COLOR_RESET)"

re: fclean all

.PHONY: all clean fclean re
