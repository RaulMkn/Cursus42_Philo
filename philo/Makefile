# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rmakende <rmakende@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/05 19:45:33 by rmakende          #+#    #+#              #
#    Updated: 2025/03/08 17:34:05 by rmakende         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -pthread -fsanitize=address

SRCS = philo.c utils.c
OBJS = $(SRCS:.c=.o)
NAME = philo

GREEN = \033[1;32m
YELLOW = \033[1;33m
RED = \033[1;31m
BLUE = \033[1;34m
RESET = \033[0m

all: $(NAME)
	@echo "(•_•)"
	@sleep 1
	@echo "( •_•)>⌐■-■"
	@sleep 1
	@echo "(⌐■_■)   ✅"
	@sleep 1
	@echo "\(^_^)/$(GREEN) Compilación exitosa: $(NAME) está listo para ejecutarse 🚀$(RESET)"

$(NAME): $(OBJS)
	@echo "$(BLUE)🔧 Compilando y enlazando archivos...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	@echo "$(YELLOW)📄 Compilando $< ...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)🗑️ Eliminando archivos objeto...$(RESET)"
	@rm -f $(OBJS)

fclean: clean
	@echo "$(RED)🚮 Eliminando $(NAME)...$(RESET)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

