# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/26 19:52:48 by toshota           #+#    #+#              #
#    Updated: 2023/09/24 12:58:52 by toshota          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	pipex
CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror
AR				=	ar rc
RM				=	rm -f
INCS			=	pipex.h
LIBFT_DIR		=	libft/

SRCS_DIR		=	src/
SRCS_FILES		=	check_arg.c check_func_is_worked.c end_pipex.c \
					get_cmd_absolute_path.c get_cmd_absolute_path_utils.c \
					get_data.c get_file.c get_utils.c here_doc.c is_utils.c \
					main.c pipex.c utils1.c utils2.c

SRCS			=	$(addprefix $(SRCS_DIR), $(SRCS_FILES))
OBJS			=	$(SRCS:.c=.o)



all:		$(NAME)

$(NAME):	$(OBJS) $(INCS) Makefile
	@ make -C $(LIBFT_DIR)
	@ $(CC) $(CFLAGS) $(LIBFT_DIR)libft.a $(OBJS) -o $(NAME)

bonus:		all

.c.o:		$(OBJS)
	@ $(CC) $(CFLAGS) -I $(INCS) -c $< -o $@

clean:
	@ make clean -C $(LIBFT_DIR)
	@ $(RM) $(OBJS)

fclean:
	@ make fclean -C $(LIBFT_DIR)
	@ $(RM) $(OBJS)
	@ $(RM) $(NAME) $(B_NAME)

re:			fclean all

.PHONY:		all bonus clean fclean re
