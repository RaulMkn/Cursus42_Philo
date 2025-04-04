# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/05 19:45:33 by rmakende          #+#    #+#              #
#    Updated: 2025/03/27 11:20:47 by rmakende         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=thread

SRCS = philo.c utils.c rutine_utils.c rutines.c
OBJS = $(SRCS:.c=.o)
NAME = philo

GREEN = \033[1;32m
YELLOW = \033[1;33m
RED = \033[1;31m
BLUE = \033[1;34m
RESET = \033[0m

all: $(NAME)
	@echo "(•_•)"
	@sleep 0.5
	@echo "( •_•)>⌐■-■"
	@sleep 0.5
	@echo "(⌐■_■)   ✅"
	@sleep 0.5
	@echo "\(^_^)/$(GREEN) Compilación exitosa: $(NAME) está listo para ejecutarse 🚀$(RESET)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "(•_•) ⊃🗑️$(RED)  Eliminado exitosamente $(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

